/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:25:50 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/22 16:32:22 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

static bool	check_consecutive_redirections(t_token *token1, t_token *token2)
{
	if (token1->token_type == TYPE_REDIRECTION_OPERATOR
		&& token2->token_type == TYPE_REDIRECTION_OPERATOR)
	{
		print_syntax_error(token2->value);
		return (false);
	}
	return (true);
}

bool	check_operator_combinations_for_redirection(t_list *tokens)
{
	t_list	*current;
	t_token	*token1;
	t_token	*token2;

	current = tokens;
	while (current && current->next)
	{
		token1 = (t_token *)current->content;
		token2 = (t_token *)current->next->content;
		if (!check_consecutive_redirections(token1, token2))
			return (false);
		current = current->next;
	}
	return (true);
}

static int	count_here_docs_in_loop(t_list *tokens)
{
	t_token	*current_token;
	int		count;

	count = 0;
	while (tokens && tokens->next)
	{
		current_token = (t_token *)tokens->content;
		if (current_token->redir_op_type == REDIR_OP_HERE_DOC)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

static bool	count_last_here_doc(t_list *tokens)
{
	t_token	*last_token;

	if (tokens)
	{
		last_token = (t_token *)tokens->content;
		if (last_token->redir_op_type == REDIR_OP_HERE_DOC)
			return (true); // 1
	}
	return (false); // 0
}

bool	check_here_doc_count(t_list *tokens)
{
	int	here_doc_count;

	here_doc_count = count_here_docs_in_loop(tokens);
	here_doc_count += count_last_here_doc(tokens);
	if (here_doc_count >= 16)
		return (false);
	return (true);
}
