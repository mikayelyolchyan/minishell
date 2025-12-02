/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:01:43 by miyolchy          #+#    #+#             */
/*   Updated: 2025/11/30 17:58:58 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"
#include "../include/executor/executor.h"
#include "../include/prompt/prompt.h"
#include "../include/signals/signals.h"
#include "../lib/libft/libft.h"

void init_shell(t_shell *shell, t_env *env_list)
{
	shell->env_list = env_list;
	shell->env_array = NULL;
	shell->last_exit_status = 0;
	shell->should_exit = 0;
}

t_env *init_node_env(char *name, char *value)
{
	t_env *new_nod;
	new_nod = malloc(sizeof(t_env));
	if(!new_nod)
		return(NULL);
	new_nod->name = name;
	new_nod->value = value;
	new_nod->next = NULL;
	return(new_nod);
}
void append_node_env(t_env **list, t_env *new_node)
{
    t_env *cur;

    if (!*list)
    {
        *list = new_node;
        return ;
    }
    cur = *list;
    while (cur->next)
        cur = cur->next;
    cur->next = new_node;
}


char *dup_substr(const char *src, int len)
{
    char *dst;
    int i = 0;

    dst = malloc(len + 1);
    if (!dst)
        return (NULL); //add error message
    while (i < len)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}


void split_env_entry(char *entry, char **name, char **value)
{
    int i = 0;

    while (entry[i] && entry[i] != '=')
        i++;

    if (entry[i] == '\0')
    {
        *name = ft_strdup(entry);
        *value = NULL;
    }
    else
    {
        *name = dup_substr(entry, i);
        *value = ft_strdup(entry + i + 1);
    }
}
t_env *get_env_list(char **envp)
{
    int i = 0;
    char *name;
    char *value;
    t_env *list = NULL;

    while (envp[i])
    {
        split_env_entry(envp[i], &name, &value);
        append_node_env(&list, init_node_env(name, value));
        i++;
    }
    return list;
}
void print_env_list(t_env *list)
{
    t_env *cur = list;

    while (cur)
    {
        if (cur->value)
            printf("%s=%s\n", cur->name, cur->value);
        else
            printf("%s=\n", cur->name);   // handle variables with no value
        cur = cur->next;
    }
}

int	main(int argc, char **argv, char **envp)
{
	t_shell shell;
	t_env *env_list;
	
	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("?\n", 2);
		return (1);
	}
	env_list = get_env_list(envp);
	init_shell(&shell, env_list);
	setup_signals_interactive();
	//print_env_list(shell.env_list);
	get_prompt_line(&shell);
	return (0);
}


