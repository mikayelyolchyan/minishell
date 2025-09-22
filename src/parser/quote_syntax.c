/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:00:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/22 16:31:13 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

static bool	is_quote_char(char c)
{
	return (c == '"' || c == '\'');
}

static int	handle_open_quote(char c, char *quote_char)
{
	*quote_char = c;
	return (1);
}

static int	handle_close_quote(char c, char quote_char, int open)
{
	if (open == 1 && c == quote_char)
		return (0);
	return (open);
}

static int	process_quote_char(char c, char *quote_char, int open)
{
	if (is_quote_char(c) && open == 0)
		return (handle_open_quote(c, quote_char));
	else if (is_quote_char(c))
		return (handle_close_quote(c, *quote_char, open));
	return (open);
}

bool	check_closed_quotes(t_list *tokens)
{
	int			i;
	int			open;
	char		*current_data;
	char		quote_char;
	t_token		*current_token;

	current_token = (t_token *)tokens->content;
	current_data = (char *)current_token->value;
	i = 0;
	open = 0;
	while (current_data[i])
	{
		open = process_quote_char(current_data[i], &quote_char, open);
		i++;
	}
	if (open != 0)
		return (false);
	return (true);
}
