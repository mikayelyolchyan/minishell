/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyze.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:58 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/20 20:51:56 by madlen           ###   ########.fr       */
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


bool check_subshell_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;
	
	current = (t_token *)current_token->content;
	next = (t_token *)current_token->next->content;
	if(current->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
	{
		if (next && (next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN || next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE))
			return (check_subshell_syntax(current_token->next));
		if (next && (next->token_type == TYPE_CONTROL_OPERATOR || 
            next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE))
        {
            print_syntax_error(next->value);
            return false;
        }
	}
	else if (current->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
	{
		if (next && next->token_type == TYPE_WORD)
        {
            print_syntax_error(next->value);
            return false;
        }
        
	}
    return true;
}

bool chek_ctrl_operator_syntax(t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	next = (t_token *)current_token->next->content;
    if (!next)
    {
        print_syntax_error("newline");
        return (false);
    }
	else if (current->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
		return check_subshell_syntax(current_token);
    else if (next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN || 
            next->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
            return check_subshell_syntax(current_token);
    else if(next && next->token_type == TYPE_CONTROL_OPERATOR)
	{
				print_syntax_error(next->value);
				return(false);
	}
    return (true);
}
   
bool check_token_syntax( t_list *current_token)
{
	t_token	*current;
	t_token	*next;

	current = (t_token *)current_token->content;
	next = (t_token *)current_token->next->content;
	if (current->token_type == TYPE_CONTROL_OPERATOR )
		return (chek_ctrl_operator_syntax(current_token));
	else if (current->token_type == TYPE_REDIRECTION_OPERATOR)
	{
		if(!next || next->token_type != TYPE_WORD)
		{
			if (next)
                print_syntax_error(next->value);
            else
                print_syntax_error("newline");
            return false;
		}
	}
	else if (current->token_type == TYPE_WORD)
    {
        if (next && next->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
        {
            print_syntax_error(next->value);
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

bool chek_closed_quotes(t_list *tokens)
{
	int i = 0;
	char *current_data;

	t_token *current_token;
	current_token = (t_token *)tokens->content;
	current_data = (char *)current_token->value;
	
	char c; 
	int open = 0;
	while(current_data[i])
	{
		if((current_data[i] == '"' || current_data[i] == '\'') && open == 0)
		{
			c = current_data[i];
			open = 1;
		}
		else if ((current_data[i] == '"' || current_data[i] == '\'') && 
			open == 1 && current_data[i] == c)
		{
			open = 0;
		}
		i++;
	}
	if(open != 0)
		return (false);
	return true;
}

bool chek_here_doc_count(t_list *tokens)
{
	t_token *current_token;
	t_token *last_token;

	int here_doc_count = 0;
	while(tokens && tokens->next)
	{
		current_token = (t_token *)tokens->content;
		if(current_token->redir_op_type == REDIR_OP_HERE_DOC)
			here_doc_count++;
		tokens = tokens->next;
	}
	if(tokens)
	{
		last_token = (t_token *)tokens->content;
		if(last_token->redir_op_type == REDIR_OP_HERE_DOC)
			here_doc_count++;
	}
	if(here_doc_count >= 16)
		return(false);
	return true;
}
bool syntax_analyze(t_list *tokens)
{
	t_token *current_token;
	t_list *current_list;

	if(!tokens)
		return(true);
	if(!chek_here_doc_count(tokens))
		return(ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2), false);
	if (!check_subshell_balance(tokens))
        return false;
	if (!check_operator_combinations_for_redirection(tokens))
        return false;
	current_token = (t_token *)tokens->content;
	if (current_token->token_type == TYPE_CONTROL_OPERATOR  
		&& current_token->ctrl_op_type != CTRL_OP_SUBSHELL_OPEN)
		return(print_syntax_error(current_token->value), false);
	current_list = tokens;
	while (current_list && current_list->next)
    {
        if (!check_token_syntax(current_list))
			return false;
		if(!chek_closed_quotes(current_list))
				 return (print_syntax_error("newline"), false);
        current_list = current_list->next;
    }
	if (current_list)
    {
        current_token = (t_token *)current_list->content;
        if (current_token->token_type == TYPE_REDIRECTION_OPERATOR || (current_token->token_type == TYPE_CONTROL_OPERATOR && current_token->ctrl_op_type != CTRL_OP_SUBSHELL_CLOSE) || (current_token->token_type == TYPE_WORD && !chek_closed_quotes(current_list)))
            	return (print_syntax_error("newline"), false);
    }
	return (true);
}
