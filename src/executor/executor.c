/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:02:23 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/01 22:11:06 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



/*
	Модуль executor отвечает за выполнение команд,
		которые были разобраны парсером.
	
	Основные задачи:
	- Запуск внешних программ с помощью fork и execve.
	- Организация пайпов между процессами,
		чтобы реализовать конвейеры команд (|).
	- Обработка редиректов ввода/вывода (<, >, >>, <<),
		настройка файловых дескрипторов.
	- Ожидание завершения дочерних процессов, сбор их кода возврата.
	- Корректная обработка ошибок запуска,
		завершения и передачи данных между процессами.
	- Вызов встроенных команд (builtins)
		в нужном контексте (основной или дочерний процесс).
	
	Особенности:
	- Executor должен уметь работать с цепочками команд,
		соединённых пайпами, и корректно перенаправлять потоки данных.
	- Должен обеспечивать совместимость с поведением bash
		для всех поддерживаемых конструкций.
	- Должен корректно обрабатывать коды возврата для поддержки $?.
*/

#include "../../include/executor/executor.h"

#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

bool execute_ast(t_ast_node *ast, t_shell *shell)
{
	if(!ast)
		return (false);
		
	/*if (!prepare_heredocs(ast, shell))
    {
        free_ast(ast);
        return false;   // to do add an error  heredoc cancelled or failed
    }*/
   
	if(ast->command)
	{
		return(execute_command(ast, shell));
	}
	/*if(ast->value)
	{
		if(ast->value->ctrl_op_type == CTRL_OP_PIPE)
			return (execute_pipe(ast, shell));
		else if (ast->value->ctrl_op_type == CTRL_OP_AND)
			return (execute_and(ast, shell));
		else if (ast->value->ctrl_op_type == CTRL_OP_OR)
			return (execute_or(ast, shell));
		else if(ast->value->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
			return (execute_subshell(ast, shell));
	}*/
	return (false);
}
