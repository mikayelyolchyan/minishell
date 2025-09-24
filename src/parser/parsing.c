/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 20:58:28 by miyolchy          #+#    #+#             */
/*   Updated: 2025/09/24 23:00:56 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser/parser.h"

/*bool	parsing(t_list *tokens)
{
	if (syntax_analyze(tokens) == false)
		return (false);
	hande_env_variables(tokens);
	
	return (true);
}*/

bool	parsing(t_list *tokens)
{
    t_ast_node	*ast;

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
    print_ast(ast);
    free_ast(ast);
    return true;
}
