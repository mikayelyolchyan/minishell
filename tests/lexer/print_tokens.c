/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:58:52 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/17 18:32:30 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"
#include <stdio.h>

static void	print_type(t_token *tok)
{
	if (tok->type == TYPE_OPERATOR)
		printf("TYPE_OPERATOR, op_type=");
	if (tok->op_type == OP_PIPE)
		printf("OP_PIPE\n");
	else if (tok->op_type == OP_AND)
		printf("OP_AND\n");
	else if (tok->op_type == OP_OR)
		printf("OP_OR\n");
	else if (tok->op_type == OP_SUBSHELL_OPEN)
		printf("OP_SUBSHELL_OPEN\n");
	else if (tok->op_type == OP_SUBSHELL_CLOSE)
		printf("OP_SUBSHELL_CLOSE\n");
	else if (tok->op_type == OP_END)
		printf("OP_END\n");
	else if (tok->op_type == OP_BACKGROUND)
		printf("OP_BACKGROUND\n");
}

static void	print_redir(t_token *tok)
{
	if (tok->type == TYPE_REDIRECTION)
		printf("TYPE_REDIRECTION, redir_type=");
	if (tok->redir_type == REDIR_IN)
		printf("REDIR_IN\n");
	else if (tok->redir_type == REDIR_OUT)
		printf("REDIR_OUT\n");
	else if (tok->redir_type == REDIR_APPEND)
		printf("REDIR_APPEND\n");
	else if (tok->redir_type == REDIR_HERE_DOC)
		printf("REDIR_HERE_DOC\n");
}

void	print_tokens(t_list *tokens)
{
	t_list	*current;
	t_token	*tok;

	current = tokens;
	while (current)
	{
		tok = current->content;
		printf("Token: ");
		if (tok->type == TYPE_WORD)
		{
			printf("TYPE_WORD, value=\"%s\"\n", tok->value);
		}
		else
		{
			print_type(tok);
			print_redir(tok);
		}
		current = current->next;
	}
	printf("\n");
}
