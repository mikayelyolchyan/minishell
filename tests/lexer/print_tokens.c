/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:58:52 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/20 20:01:37 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"
#include <stdio.h>

static void	print_type(t_token *tok)
{
	if (tok->type == TYPE_CONTROL_OPERATOR)
		printf("TYPE_OPERATOR, op_type=");
	if (tok->op_type == CTRL_OP_PIPE)
		printf("OP_PIPE");
	else if (tok->op_type == CTRL_OP_AND)
		printf("OP_AND");
	else if (tok->op_type == CTRL_OP_OR)
		printf("OP_OR");
	else if (tok->op_type == CTRL_OP_SUBSHELL_OPEN)
		printf("OP_SUBSHELL_OPEN");
	else if (tok->op_type == CTRL_OP_SUBSHELL_CLOSE)
		printf("OP_SUBSHELL_CLOSE");
	else if (tok->op_type == CTRL_OP_END)
		printf("OP_END");
	else if (tok->op_type == CTRL_OP_BACKGROUND)
		printf("OP_BACKGROUND");
}

static void	print_redir(t_token *tok)
{
	if (tok->type == TYPE_REDIRECTION_OPERATOR)
		printf("TYPE_REDIRECTION, redir_type=");
	if (tok->redir_type == REDIR_OP_IN)
		printf("REDIR_IN");
	else if (tok->redir_type == REDIR_OP_OUT)
		printf("REDIR_OUT");
	else if (tok->redir_type == REDIR_OP_APPEND)
		printf("REDIR_APPEND");
	else if (tok->redir_type == REDIR_OP_HERE_DOC)
		printf("REDIR_HERE_DOC");
}

//static void	print_quote(t_token *tok)
//{
//	printf(", quote_type=");
//	if (tok->quote_type == QUOTE_NONE)
//		printf("QUOTE_NONE");
//	else if (tok->quote_type == QUOTE_SINGLE)
//		printf("QUOTE_SINGLE");
//	else if (tok->quote_type == QUOTE_DOUBLE)
//		printf("QUOTE_DOUBLE");
//}

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
			printf("TYPE_WORD, value=%s", tok->value);
			//print_quote(tok);
		}
		else
		{
			print_type(tok);
			print_redir(tok);
			//print_quote(tok);
		}
		printf("\n");
		current = current->next;
	}
	printf("\n");
}
