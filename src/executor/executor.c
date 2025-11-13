/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:02:23 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/25 14:40:03 by madlen           ###   ########.fr       */
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

bool execute_ast(t_ast_node *ast, char **enviroment)
{
	if(!ast)
		return (false);
	if(ast->command)
	{
		//expand();
		return(execute_command(ast, enviroment));
	}
	if(ast->value)
	{
		if(ast->value->ctrl_op_type == CTRL_OP_PIPE)
			return (execute_pipe(ast, enviroment));
		else if (ast->value->ctrl_op_type == CTRL_OP_AND)
			return (execute_and(ast, enviroment));
		else if (ast->value->ctrl_op_type == CTRL_OP_OR)
			return (execute_or(ast, enviroment));
		else if(ast->value->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
			return (execute_subshell(ast, enviroment));
	}
	return (false);
}