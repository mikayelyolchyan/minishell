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

/*bool	parsing(t_list *tokens)
{
	if (syntax_analyze(tokens) == false)
		return (false);
	hande_env_variables(tokens);
	
	return (true);
}*/

bool	parsing(t_list *tokens, t_shell *shell)
{
    t_ast_node	*ast;
	
	(void)shell;
    if (!tokens)
        return (false);
    if (!syntax_analyze(tokens))
        return false;
    ast = build_ast(tokens);
    if (!ast)
    {
        fprintf(stderr, "AST build failed\n");
        return false;
    }
	if (!prepare_heredocs(ast, shell))
    {
        free_ast(ast);
        return false;   // to do add an error handeling heredoc cancelled or failed
    }
	//print_ast(ast);
	execute_ast(ast, shell);
    free_ast(ast);
    return true;
}
