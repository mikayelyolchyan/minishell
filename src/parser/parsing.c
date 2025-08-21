/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 20:58:28 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/21 20:59:17 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

bool	parsing(t_list *tokens)
{
	if (syntax_analyze(tokens) == false)
		return (false);
	return (true);
}
