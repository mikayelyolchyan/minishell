/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 00:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/05 00:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expansion/expansion.h"

static void	process_quote_char(char *str, int *i, char *quote, int *had_q)
{
	if ((str[*i] == '\'' || str[*i] == '"') && !(*quote))
	{
		*quote = str[*i];
		*had_q = 1;
		(*i)++;
	}
	else if (str[*i] == *quote)
	{
		*quote = 0;
		(*i)++;
	}
}

char	*remove_quotes_from_string(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;
	int		had_quotes;

	result = malloc(ft_strlen(str) + 2);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	had_quotes = 0;
	while (str[i])
	{
		if (((str[i] == '\'' || str[i] == '"') && !quote) || str[i] == quote)
			process_quote_char(str, &i, &quote, &had_quotes);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	if (j == 0 && had_quotes)
		result[j++] = 1;
	result[j] = '\0';
	return (result);
}
