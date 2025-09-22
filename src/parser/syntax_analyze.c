/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyze.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:58 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/22 16:38:35 by miyolchy         ###   ########.fr       */
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

static bool	check_initial_tokens(t_list *tokens)
{
	t_token	*current_token;

	if (!tokens)
		return (true);
	current_token = (t_token *)tokens->content;
	if (current_token->token_type == TYPE_CONTROL_OPERATOR && \
		current_token->ctrl_op_type != CTRL_OP_SUBSHELL_OPEN)
		return (print_syntax_error(current_token->value), false);
	return (true);
}

static bool	check_final_tokens(t_list *current_list)
{
	t_token	*current_token;

	if (!current_list)
		return (true);
	current_token = (t_token *)current_list->content;
	if (current_token->token_type == TYPE_REDIRECTION_OPERATOR || \
		(current_token->token_type == TYPE_CONTROL_OPERATOR && \
		current_token->ctrl_op_type != CTRL_OP_SUBSHELL_CLOSE) || \
		(current_token->token_type == TYPE_WORD && \
		!check_closed_quotes(current_list)))
		return (print_syntax_error("newline"), false);
	return (true);
}

static bool	check_pre_conditions(t_list *tokens)
{
	if (!check_here_doc_count(tokens))
		return (ft_putstr_fd("bash: maximum here-document count exceeded\n",
				2), false);
	if (!check_subshell_balance(tokens))
		return (false);
	if (!check_operator_combinations_for_redirection(tokens))
		return (false);
	return (true);
}

static bool	check_tokens_loop(t_list *tokens)
{
	t_list	*current_list;

	current_list = tokens;
	while (current_list && current_list->next)
	{
		if (!check_token_syntax(current_list))
			return (false);
		if (!check_closed_quotes(current_list))
			return (print_syntax_error("newline"), false);
		current_list = current_list->next;
	}
	return (check_final_tokens(current_list));
}

bool	syntax_analyze(t_list *tokens)
{
	if (!tokens)
		return (true);
	if (!check_pre_conditions(tokens))
		return (false);
	if (!check_initial_tokens(tokens))
		return (false);
	return (check_tokens_loop(tokens));
}
