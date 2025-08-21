/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:55 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/21 20:55:27 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Модуль prompt отвечает за формирование и отображение приглашения (prompt)
		для пользователя в интерактивном режиме работы minishell.

	Задачи:
	- Формировать строку приглашения, отображающую, например, имя пользователя,
		текущую директорию, статус последней команды
		и другие элементы (по желанию).
	- Выводить prompt и ожидать ввода команды от пользователя.
	- Использовать библиотеку readline для удобного ввода,
		поддержки истории и редактирования командной строки.
	- Обеспечивать корректную работу с сигналами
		(например, обновлять prompt при Ctrl+C).

	Особенности:
	- Модуль должен быть изолирован и предоставлять функции для вывода prompt
		и получения строки ввода.
	- Должен корректно работать как в интерактивном,
		так и неинтерактивном режимах.
*/

#include "../../include/prompt/prompt.h"
#include "../../include/lexer/lexer.h"
#include "../../include/parser/parser.h"

void		print_tokens(t_list *tokens);

void	get_prompt_line(void)
{
	char		*line;
	t_list		*tokens;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (line[0] != '\0')
			add_history(line);
		tokens = lexical_analyze(line);
		if (tokens == NULL)
		{
			continue ;
		}
		else if (tokens != NULL)
		{
			parsing(tokens);
			print_tokens(tokens);
		}
		ft_lstclear(&tokens, del_token);
		free(line);
	}
}
