/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:00:00 by madlen            #+#    #+#             */
/*   Updated: 2025/12/03 18:00:00 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"

static void	execute_node_logical(t_ast_node *node, t_shell *shell)
{
	if (node->value && node->value->ctrl_op_type == CTRL_OP_AND)
		execute_and(node, shell);
	else if (node->value && node->value->ctrl_op_type == CTRL_OP_OR)
		execute_or(node, shell);
	else if (node->value
		&& node->value->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
		execute_subshell(node, shell);
	else if (node->command)
		execute_command(node, shell);
	else
		execute_pipe(node, shell);
}

static void	execute_and_left(t_ast_node *ast, t_shell *shell)
{
	if (!ast->left)
		return ;
	execute_node_logical(ast->left, shell);
}

static void	execute_and_right(t_ast_node *ast, t_shell *shell)
{
	if (shell->last_exit_status != 0 || !ast->right)
		return ;
	execute_node_logical(ast->right, shell);
}

bool	execute_and(t_ast_node *ast, t_shell *shell)
{
	if (!ast || !shell)
		return (false);
	execute_and_left(ast, shell);
	execute_and_right(ast, shell);
	return (true);
}

bool	execute_or(t_ast_node *ast, t_shell *shell)
{
	if (!ast || !shell)
		return (false);
	if (ast->left)
		execute_node_logical(ast->left, shell);
	if (shell->last_exit_status != 0 && ast->right)
		execute_node_logical(ast->right, shell);
	return (true);
}
