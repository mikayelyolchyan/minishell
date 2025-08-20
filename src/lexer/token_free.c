/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:07:22 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/20 20:15:02 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"

void	del_token(void *content)
{
	t_token	*tok;

	tok = content;
	if (!tok)
		return ;
	free(tok->value);
	free(tok);
}
