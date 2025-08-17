/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:02:13 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/17 19:35:23 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Модуль lexer отвечает за низкоуровневую токенизацию строки,
		введённой пользователем.
	
	Задачи:
	- Разделение строки на токены: слова, операторы (|, <, >, >>, <<),
		кавычки, переменные окружения.
	- Учёт экранирования, вложенных кавычек, спецсимволов.
	- Передача списка токенов парсеру для дальнейшей обработки.
	
	Особенности:
	- Lexer должен быть максимально простым и быстрым,
		не анализируя структуру команд, а только выделяя токены.
	- Должен корректно обрабатывать все допустимые символы и конструкции,
		поддерживаемые minishell.
*/

#include "../../include/lexer/lexer.h"
#include "../../include/lexer/utils.h"
#include <stddef.h>

t_list *tokenize(const char *line)
{
	size_t		index;
	t_list		*head;
	t_list		*current;
	t_token		*new_token;

	if (!line)
		return (NULL);
	head = NULL;
	index = 0;
	while (line[index])
	{
		while (is_space(line[index]) == true)
			index++;
		if (line[index] == '\0')
			break ;
		new_token = ft_calloc(1, sizeof(t_token));
		if (new_token == NULL)
			return (ft_lstclear(&head, del_token), NULL);
		if (is_operator(line[index]) == true)
			set_operator(new_token, line, &index);
		else if (is_redirection(line[index]) == true)
			set_redirection(new_token, line, &index);
		else
		{
			if (set_word(new_token, line, &index) == false)
				return (del_token(new_token), \
					ft_lstclear(&head, del_token), NULL);
		}
		current = ft_lstnew(new_token);
		if (current == NULL)
			return (del_token(new_token), ft_lstclear(&head, del_token), NULL);
		ft_lstadd_back(&head, current);
	}
	return (head);
}
