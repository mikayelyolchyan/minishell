/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 20:58:28 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 11:30:54 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"
#include "../../include/executor/executor.h"
#include "../../include/expansion/expansion.h"

static bool	handle_parse_error(t_ast_node *ast, char *msg)
{
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
	if (ast)
		free_ast(ast);
	return (false);
}

static bool	process_heredocs(t_ast_node *ast, t_shell *shell)
{
	int	heredoc_index;

	heredoc_index = 0;
	if (!prepare_heredocs_recursive(ast, shell, &heredoc_index))
		return (handle_parse_error(ast, NULL));
	return (true);
}

bool	parsing(t_list *tokens, t_shell *shell)
{
	t_ast_node	*ast;

	if (!tokens)
		return (false);
	if (!syntax_analyze(tokens))
	{
		shell->last_exit_status = 2;
		return (false);
	}
	ast = build_ast(tokens);
	if (!ast)
		return (handle_parse_error(NULL, "AST build failed\n"));
	if (!process_heredocs(ast, shell))
		return (false);
	expand_ast_node(ast, shell);
	execute_ast(ast, shell);
	free_ast(ast);
	return (true);
}
