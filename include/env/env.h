/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:00:49 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/15 18:00:50 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdio.h>
# include <stdlib.h>

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env_list;
	char	**env_array;
	int		last_exit_status;
	int		should_exit;
}	t_shell;

void	init_shell(t_shell *shell, t_env *env_list);
t_env	*init_node_env(char *name, char *value);
void	append_node_env(t_env **list, t_env *new_node);
char	*dup_substr(const char *src, int len);
void	split_env_entry(char *entry, char **name, char **value);
t_env	*get_env_list(char **envp);
void	print_env_list(t_env *list);

#endif