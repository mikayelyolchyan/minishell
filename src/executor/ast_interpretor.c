/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_interpretor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madlen <madlen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:41:22 by madlen            #+#    #+#             */
/*   Updated: 2025/12/01 10:04:27 by madlen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor/executor.h"
#include <fcntl.h>

/*
Simple commands → execute immediately
Pipes → set up inter-process communication
Logical operators → execute conditionally based on exit status
Subshells → execute in isolated environment
*/


// Sofis split 
/*#include "libft.h"

static size_t	word_count(char const *s, char c, int i)
{
	size_t	count;

	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			break ;
		count++;
		if (s[i] == '\'')
		{
			i++;
			while (s[i] && s[i] != '\'')
				i++;
			if (s[i] == '\'')
				i++;
		}
		else
		{
			while (s[i] && s[i] != c && s[i] != '\'')
				i++;
		}
	}
	return (count);
}

static int	ft_malloc(char **result, int position, size_t len, char *start)
{
	result[position] = malloc(len + 1);
	if (!result[position])
	{
		while (position >= 0)
			free(result[position--]);
		free(result);
		return (0);
	}
	ft_strlcpy(result[position], start, len + 1);
	return (1);
}
int	split_2(char **s, char c, int *qu)
{
	size_t	len;

	len = 0;
	while (**s && ((*qu && **s != '\'') || (!*qu && **s != c && **s != '\'')))
	{
		++len;
		(*s)++;
	}
	if (!*qu && len == 0)
		return (-1);
	if (*qu && **s == '\'')
	{
		*qu = 0;
		(*s)++;
	}
	return (len);
}

int	split(char **result, char *s, char c, int i)
{
	char	*start;
	int		qu;
	long	len;

	qu = 0;
	while (*s)
	{
		while (*s == c && *s)
			++s;
		if (*s == '\'')
		{
			qu = 1;
			s++;
		}
		start = (char *)s;
		len = split_2(&s, c, &qu);
		if (len != -1)
		{
			if (ft_malloc(result, i, len, start) == 0)
				return (0);
			i++;
		}
	}
	result[i] = NULL;
	return (1);
}

char	**ft_split_2(char *s, char c)
{
	char	**res;
	size_t	len;

	if (!s)
		return (NULL);
	len = word_count(s, c, 0);
	res = (char **)malloc(sizeof(char *) * (len + 1));
	if (!res)
		return (NULL);
	if (split(res, s, c, 0) == 0)
	{
		free(res);
		return (NULL);
	}
	return (res);
}*/
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
	cmd = cmd_node->command->argument[0]; // 0 ? 
	
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
	if(cmd_node->command == NULL)
	{
		write(1, "cmd_node->command == NULL\n", 22);
		return (1); // add error message ? 
	}
	//printf("cmd_node %s\n", cmd_node->command->argument[0]);
	if(is_bulit_in_cmd(cmd_node) == true)
		//return(execute_builtin(cmd_node, shell));
		return (fprintf(stderr, "Builtin execution not implemented\n"), 1);
	else
	{
		//printf("%s is not a built-in command\n", cmd_node->command->argument[0]);
		cmd_path = find_cmd_path(cmd_node->command->argument[0], shell->env_list);
		//printf("cmd_path: %s\n", cmd_path);
		if(!cmd_path)
		{
			shell->last_exit_status = 127;
      		return (shell->last_exit_status); // add error message ? 
		} 
		//hendel redirs 
		pid = fork();
		if(pid < 0)
		{
			write( 1 , "fork failed", 12);
			return(free(cmd_path), 1 );
		} // add error message ? is the exit status correct ? 
		if(pid == 0) // child
		{
			//write(1, "In child process\n", 17);
			//printf("I'm the child, PID: %d\n", getpid());
			execute_cmd_child(cmd_node, cmd_path, shell);
		}
		else // parent
		{
			//write(1, "In parent process\n", 18);
			//printf("I'm the parent, child PID: %d\n", pid);
			waitpid(pid,&status, 0);
			shell->last_exit_status = 0;
			free(cmd_path);
		}
	}
	//free(cmd_path);
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

/*int handle_cmd_redir(t_ast_node *cmd_node, t_shell *shell)
{
	if(cmd_node->command->redir->redir_type == REDIR_OP_IN \
	|| cmd_node->command->redir->redir_type == REDIR_OP_HERE_DOC)
		return(set_in_fd(cmd_node, shell));
	else if(cmd_node->command->redir->redir_type == REDIR_OP_OUT \
	|| cmd_node->command->redir->redir_type == REDIR_OP_APPEND)
		return(set_out_fd(cmd_node, shell));
	return (0);
}*/
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
/*int set_in_fd(t_ast_node *cmd_node , t_shell *t_shell)
{
	int fd;
	if(cmd_node->command->redir->redir_type == REDIR_OP_IN)
	{
		fd = open(cmd_node->command->redir->filename, O_RDONLY);
		
		cmd_node->command->redir->in_fd = fd;	
	}
	if(cmd_node->command->redir->redir_type == REDIR_OP_HERE_DOC)
	{
		fd = handle_heredock(cmd_node->command->redir->filename);
		fd = open(fd, O_RDONLY);
		if (fd == -1)
		{
    		perror("minishell"); // to do error massage idk 
    		return (0);
		}		
		// Do I need to close it here ? 
	}
		if(cmd_node->command->redir->in_fd  != STDIN_FILENO && fd != -1)
			dup2(cmd_node->command->redir->in_fd, STDIN_FILENO);
		else
		// I dont knpow what shoild happen here 
	return(0);
}


int set_out_fd(t_ast_node *cmd_node, t_shell *shell)
{
	int fd;
	if(cmd_node->command->redir->redir_type == REDIR_OP_APPEND)
	{
		fd = open(cmd_node->command->redir->filename, O_WRONLY | O_CREAT | O_APPEND , 0644);
		if (fd == -1)
		{
    		perror("minishell"); // to do error massage idk 
    		return (0);
		}		
		cmd_node->command->redir->out_fd = fd;
	}
	if(cmd_node->command->redir->redir_type == REDIR_OP_OUT)
	{
		fd = handle_heredock(cmd_node->command->redir->filename);
		fd = open(cmd_node->command->redir->filename,O_WRONLY | O_CREAT | O_TRUNC , 0644);
		if(fd == -1)
		{
			return(close(fd), 0);// I nned to do something here 
		}
		cmd_node->command->redir->out_fd = fd;
		// Do I need to close it here ? 
	}
	if(cmd_node->command->redir->out_fd  != STDOUT_FILENO && fd != -1)
			dup2(cmd_node->command->redir->out_fd, STDOUT_FILENO);
		else
		// I dont knpow what shoild happen here 
	return(0);
}*/

int handle_single_redir(t_redir *r, t_shell *shell)
{
    int fd;
    if (r->redir_type == REDIR_OP_IN || r->redir_type == REDIR_OP_HERE_DOC)
        fd = open(r->filename, O_RDONLY);
    else if (r->redir_type == REDIR_OP_OUT)
        fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (r->redir_type == REDIR_OP_APPEND)
        fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
     if (fd < 0)
    {
        perror("minishell");
        shell->last_exit_status = 1;
        return 0;
    }// to do idk what error or exit status haha 
    if (r->redir_type == REDIR_OP_IN || r->redir_type == REDIR_OP_HERE_DOC)
    {
        if (dup2(fd, STDIN_FILENO) < 0)
        {
            perror("minishell");
            close(fd);
            shell->last_exit_status = 1;
            return 0;
        }
    }
    else
    {
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("minishell");
            close(fd);
            shell->last_exit_status = 1;
            return 0;
        }
    }
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



