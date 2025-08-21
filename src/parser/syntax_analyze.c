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

static bool	check_token_syntax(t_list *token, t_token *current_token, t_token *prev_token)
{
	if (current_token->token_type == TYPE_CONTROL_OPERATOR)
	{
		
	}
	else if (current_token->token_type == TYPE_REDIRECTION_OPERATOR)
	{
		
	}
	return (true);	
}

bool	syntax_analyze(t_list *tokens)
{
	t_token	*current_token; 
	t_token	*prev_token;

	if (!tokens)
		return (true);
	current_token = NULL;
	while (tokens->content != NULL)
	{
		prev_token = current_token;
		current_token = (t_token*)tokens->content;
		if (check_token_syntax(tokens, current_token, prev_token) == false)
			return (false);
		tokens = tokens->next;
	}
	return (true);
}
