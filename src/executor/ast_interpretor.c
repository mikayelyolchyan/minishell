/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_interpretor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:41:22 by madlen            #+#    #+#             */
/*   Updated: 2025/09/27 14:51:37 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"

/*
Simple commands → execute immediately
Pipes → set up inter-process communication
Logical operators → execute conditionally based on exit status
Subshells → execute in isolated environment
*/
bool execute_command(t_ast_node *ast, char **enviroment)
{
	
}

bool handle_cmd_redir(t_ast_node *ast, char **enviroment)
{
	
}
/*
For pipe operators:

Create a pipe using pipe() system call

This gives you two file descriptors: read and write ends


Fork two child processes

Left child: connects its stdout to pipe's write end
Right child: connects its stdin to pipe's read end


Parent process:

Closes both pipe ends (children handle them)
Waits for both children to complete
Returns exit status of the last command
*/

bool execute_pipe(t_ast_node *ast, char **enviroment)
{
	
}

/*&&: only execute right side if left succeeded (exit code 0)
||: only execute right side if left failed (exit code != 0)
*/

bool execute_and(t_ast_node *ast, char **enviroment)
{
	
}

bool ecute_or(t_ast_node *ast, char **enviroment)
{
	
}

bool execute_subshell(t_ast_node *ast, char **enviroment)
{
	
}


