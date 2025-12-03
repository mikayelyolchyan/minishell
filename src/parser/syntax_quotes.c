/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 20:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 20:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

static bool	check_quote_char(char c, char *open_quote, int *is_open)
{
	if ((c == '"' || c == '\'') && *is_open == 0)
	{
		*open_quote = c;
		*is_open = 1;
		return (true);
	}
	if ((c == '"' || c == '\'') && *is_open == 1 && c == *open_quote)
	{
		*is_open = 0;
		return (true);
	}
	return (false);
}

bool	chek_closed_quotes(t_list *tokens)
{
	int		i;
	char	*current_data;
	t_token	*current_token;
	char	open_quote;
	int		is_open;

	current_token = (t_token *)tokens->content;
	current_data = (char *)current_token->value;
	is_open = 0;
	i = 0;
	while (current_data[i])
	{
		check_quote_char(current_data[i], &open_quote, &is_open);
		i++;
	}
	if (is_open != 0)
		return (false);
	return (true);
}
