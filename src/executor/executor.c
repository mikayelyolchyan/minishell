/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:02:23 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/03 17:37:25 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"

bool	execute_ast(t_ast_node *ast, t_shell *shell)
{
	if (!ast)
		return (false);
	if (ast->command)
		return (execute_command(ast, shell));
	if (ast->value)
	{
		if (ast->value->ctrl_op_type == CTRL_OP_PIPE)
			return (execute_pipe(ast, shell));
		else if (ast->value->ctrl_op_type == CTRL_OP_AND)
			return (execute_and(ast, shell));
		else if (ast->value->ctrl_op_type == CTRL_OP_OR)
			return (execute_or(ast, shell));
		else if (ast->value->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
			return (execute_subshell(ast, shell));
	}
	return (false);
}
