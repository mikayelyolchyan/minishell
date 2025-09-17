/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyze.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:58 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/21 20:46:21 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
	Модуль parser отвечает за разбор (парсинг) строки, введённой пользователем,
		на отдельные команды, аргументы, пайпы, редиректы,
		кавычки и переменные окружения.
	
	Основные задачи:
	- Токенизация строки: разделение на слова, учёт кавычек,
		спецсимволов, пайпов, редиректов.
	- Построение структуры команд: создание списка или дерева команд с
		их аргументами, редиректами, пайпами.
	- Обработка кавычек:
	  - Одинарные кавычки ('...') — всё внутри воспринимается как литерал.
	  - Двойные кавычки ("...") — всё внутри воспринимается как литерал,
	  	кроме знака доллара ($).
	- Обработка переменных окружения ($VAR, $?) и их подстановка.
	- Проверка на незакрытые кавычки и синтаксические ошибки.
	- Подготовка данных для executor-а.
	
	Особенности:
	- Парсер должен быть устойчив к ошибкам и выдавать информативные
		сообщения при некорректном вводе.
	- Должен поддерживать все синтаксические конструкции, требуемые subject-ом.
*/

#include "../../include/parser/parser.h"


bool check_subshell_syntax(t_token *current_token, t_token *next_token)
{
	
	if(current_token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
	{
		if (next_token && (next_token->token_type == TYPE_CONTROL_OPERATOR || 
            next_token->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE))
        {
            print_syntax_error(next_token->value);
            return false;
        }
	}
	else if (current_token->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
	{
		if (next_token && next_token->token_type == TYPE_WORD)
        {
            print_syntax_error(next_token->value);
            return false;
        }
        
	}
    return true;
}
bool chek_ctrl_operator_syntax(t_token *current_token, t_token *next_token)
{
    if (!next_token)
    {
        print_syntax_error("newline");
        return (false);
    }
    else if (next_token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN || 
            next_token->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
            return check_subshell_syntax(current_token, next_token);
    else if(next_token && next_token->token_type == TYPE_CONTROL_OPERATOR)
	{
				print_syntax_error(next_token->value);
				return(false);
	}
    return (true);
}
   
bool check_token_syntax( t_token *current_token, t_token *next_token)
{
	if (current_token->token_type == TYPE_CONTROL_OPERATOR )
	{
		return (chek_ctrl_operator_syntax(current_token, next_token));
	}
	else if (current_token->token_type == TYPE_REDIRECTION_OPERATOR)
	{
		if(!next_token || next_token->token_type != TYPE_WORD)
		{
			if (next_token)
                print_syntax_error(next_token->value);
            else
                print_syntax_error("newline");
            return false;
		}
	}
	else if (current_token->token_type == TYPE_WORD)
    {
        if (next_token && next_token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        {
            print_syntax_error(next_token->value);
            return false;
        }
    }
	return (true);	
}

bool check_operator_combinations_for_redirection(t_list *tokens)
{
    t_list *current = tokens;
    t_token *token1, *token2;
    
    while (current && current->next)
    {
        token1 = (t_token *)current->content;
        token2 = (t_token *)current->next->content;
        
        if(token1->token_type == TYPE_REDIRECTION_OPERATOR
		&& token2->token_type == TYPE_REDIRECTION_OPERATOR)
        {
            print_syntax_error(token2->value);
            return false;
        }
        current = current->next;
    }
    return true;
}


bool syntax_analyze(t_list *tokens)
{
	t_token *current_token;
	t_token *next_token;
	t_token *first_token;
	t_list *current_list;

	if(!tokens)
		return(true);
	if (!check_subshell_balance(tokens))
        return false;
	if (!check_operator_combinations_for_redirection(tokens))
        return false;
	first_token = (t_token *)tokens->content;
	if(first_token->token_type == TYPE_CONTROL_OPERATOR  
		&& first_token->ctrl_op_type != CTRL_OP_SUBSHELL_OPEN)
		return(print_syntax_error(first_token->value), false);
	current_list = tokens;
	while (current_list && current_list->next)
    {
        current_token = (t_token *)current_list->content;
        next_token = (t_token *)current_list->next->content;
        if (!check_token_syntax(current_token, next_token))
            return false;   
        current_list = current_list->next;
    }
	if (current_list)
    {
        current_token = (t_token *)current_list->content;
        if (current_token->token_type == TYPE_REDIRECTION_OPERATOR ||
            (current_token->token_type == TYPE_CONTROL_OPERATOR && 
             current_token->ctrl_op_type != CTRL_OP_SUBSHELL_CLOSE))
            	return (print_syntax_error("newline"), false);
    }
	return (true);
}
