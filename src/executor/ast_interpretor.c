/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_interpretor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:41:22 by madlen            #+#    #+#             */
/*   Updated: 2025/12/02 12:55:32 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"
#include <fcntl.h>


#include <stddef.h>
//---------------------------------------cmd---------------------------------------
void free_arr(char **arr)
{
    int i = 0;
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

// Sofis split end

bool is_bulit_in_cmd(t_ast_node *cmd_node)
{
	char *cmd;
	
	if (!cmd_node || !cmd_node->command || !cmd_node->command->argument)
		return (false);
	
	cmd = cmd_node->command->argument[0];
	if (!cmd)
		return (false);
	
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	else if (ft_strcmp(cmd, "env") == 0)
		return (true);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	else if (ft_strcmp(cmd, "export") == 0)
		return (true);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	return (false);
}

char	*check_if_cmd_is_path(char *cmd)
{
	if (!cmd || !cmd[0])
		return (NULL);
		
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	return (NULL);
}
char *find_path(t_env *env_list)
{
    t_env *current = env_list;

    /*while (current && current->next != NULL)
    {
        if (ft_strcmp(current->name, "PATH") == 0)
            return (current->value);
        current = current->next;
    }*/
   while (current) // to doI get seg like this 
    {
        if (ft_strcmp(current->name, "PATH") == 0)
            return (current->value);
        current = current->next;
    }
	
    return (NULL);
}
/*void print_2d_array(char **array, int size) {
    if (array == NULL) {
        printf("Array is NULL\n");
        return;
    }
    
    for (int i = 0; i < size; i++) {
        if (array[i] == NULL) {
            printf("[%d] (NULL)\n", i);
        } else {
            printf("[%d] %s\n", i, array[i]);
        }
    }
}*/

char *find_cmd_path(char *cmd_name, t_env *env_list)
{
	char **paths;
	char *path_value;
	char *tmp;
	char *full_path;
	
	if(!cmd_name)
		return (NULL);
	full_path = check_if_cmd_is_path(cmd_name);
    if(full_path)
	{
        return (full_path);
	}
	path_value = find_path(env_list);
	//printf("PATH value: %s\n", path_value);
	if (!path_value || !path_value[0])
		return (NULL); // add error message ? "command not found "
	paths =  ft_split(path_value, ':');
	//print_2d_array(paths, 14); // Debug print
	if (!paths)
        return (NULL);
	int i = 0;
	while(paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (access(full_path, X_OK) == 0)
			return (free_arr(paths),full_path);
		free(full_path);
		i++;
	}
	free_arr(paths);
    return (NULL);
}

int execute_command(t_ast_node *ast, t_shell *shell)
{
	char *cmd_path;
	pid_t	pid;
	t_ast_node *cmd_node;
	int status;
	
	cmd_node = ast;
	
	/* Early null checks - silently fail if no valid command */
	if (!cmd_node)
		return (1);
	if (!cmd_node->command)
		return (1);
	if (!cmd_node->command->argument || !cmd_node->command->argument[0])
		return (1);
	
	if(is_bulit_in_cmd(cmd_node) == true)
		//return(execute_builtin(cmd_node, shell));
		return (fprintf(stderr, "Builtin execution not implemented\n"), 1);
	else
	{
		cmd_path = find_cmd_path(cmd_node->command->argument[0], shell->env_list);
		if(!cmd_path)
		{
			shell->last_exit_status = 127;
      		return (shell->last_exit_status);
		} 
		pid = fork();
		if(pid < 0)
		{
			write(STDERR_FILENO, "minishell: fork failed\n", 23);
			return(free(cmd_path), 1);
		}
		if(pid == 0) // child
		{
			execute_cmd_child(cmd_node, cmd_path, shell);
		}
		else // parent
		{
			waitpid(pid,&status, 0);
			shell->last_exit_status = 0;
			free(cmd_path);
		}
	}
	return (shell->last_exit_status);
}


void execute_cmd_child(t_ast_node *cmd_node, char *cmd_path, t_shell *shell)
{
	//handle redirs
	if (cmd_node->command->redir)
	{
		if(!apply_redir(cmd_node, shell))
		{
			free(cmd_path);
			exit(1); // add error message ?
		}
	}

	execve(cmd_path, cmd_node->command->argument, shell->env_array);
	free(cmd_path);
	exit(127); // add error message ?	
}
//---------------------redir--------------------------


int apply_redir(t_ast_node *cmd_node, t_shell *shell)
{
    t_redir *redir = cmd_node->command->redir;

    while (redir)
    {
        if (!handle_single_redir(redir, shell))
            return (0);
        redir = redir->next;
    }
    return (1);
}


int handle_single_redir(t_redir *r, t_shell *shell)
{
    int fd;
    const char *filename;
    
    if (!r)
        return (0);
    if (r->redir_type == REDIR_OP_HERE_DOC)
        filename = r->heredoc_tmpfile;
    else
        filename = r->filename;
    if (!filename)
    {
        fprintf(stderr, "minishell: error: redirection filename is NULL\n");
        shell->last_exit_status = 1;
        return (0);
    }
    if (r->redir_type == REDIR_OP_IN || r->redir_type == REDIR_OP_HERE_DOC)
        fd = open(filename, O_RDONLY);
    else if (r->redir_type == REDIR_OP_OUT)
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (r->redir_type == REDIR_OP_APPEND)
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        return (0);
    if (fd < 0)
    {
        perror("minishell");
        shell->last_exit_status = 1;
        return (0);
    }
    if (r->redir_type == REDIR_OP_IN || r->redir_type == REDIR_OP_HERE_DOC)
    {
        if (dup2(fd, STDIN_FILENO) < 0)
        {
            perror("minishell");
            close(fd);
            shell->last_exit_status = 1;
            return (0);
        }
    }
    else
    {
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("minishell");
            close(fd);
            shell->last_exit_status = 1;
            return (0);
        }
    }
    close(fd);
    return (1);
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

/*bool execute_pipe(t_ast_node *ast, t_shell *shell)
{
	
}*/

/*&&: only execute right side if left succeeded (exit code 0)
||: only execute right side if left failed (exit code != 0)
*/

/*bool execute_and(t_ast_node *ast, t_shell *shell)
{
	
}

bool ecute_or(t_ast_node *ast, t_shell *shel)
{
	
}

bool execute_subshell(t_ast_node *ast, t_shell *shell)
{
	
}*/



