/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:45:00 by miyolchy          #+#    #+#             */
/*   Updated: 2025/12/02 22:45:00 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include <stdlib.h>
# include "../executor/executor.h"
# include "../parser/parser.h"
# include "../../lib/libft/libft.h"

char	*expand_variables(char *str, t_shell *shell);
void	expand_ast_node(t_ast_node *node, t_shell *shell);
char	*join_and_free(char *s1, char *s2);

#endif
