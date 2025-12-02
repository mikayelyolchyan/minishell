
#include "../../include/parser/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>


#define TMP_DIR "/tmp/.heredoc_"


/*int	ft_strcmp(const char *s1, const char *s2);

bool prepare_heredocs(t_ast_node *node, t_shell *shell)
{
    if (!node)
        return (true);
    
    // Leaf node (command)
    if (node->command)
        return (apply_heredoc(node->command, shell));
    
    // Operator node - process both children
    return (prepare_heredocs(node->left, shell) && 
            prepare_heredocs(node->right, shell));
}

bool apply_heredoc(t_command *cmd, t_shell *shell)
{
	t_redir *redir;

	if (!cmd)
		return true;
	redir = cmd->redir;
	while (redir)
	{
		if (redir->redir_type == REDIR_OP_HERE_DOC)
		{
			if (!make_heredoc(redir, shell))
				return false;
		}
		redir = redir->next;
	}
	return true;
}
bool make_heredoc(t_redir *redir , t_shell *shell)
{
	char *comand_line;
	while(1)
	{
		comand_line = readline("> ");
		if(!comand_line)
		{
			print_heredoc_error(redir->filename);
			break;
		}
		else if(ft_strcmp(redir->filename, comand_line) == 0)
		{
			free(comand_line);
			break;
		}
		free(comand_line);
	}
	exit(0);
}*/

int	ft_strcmp(const char *s1, const char *s2);
char *generate_heredoc_filename(int index)
{
    char *index_str;
    char *filename;
	
    index_str = ft_itoa(index);
    if (!index_str)
        return (NULL);
    filename = ft_strjoin(TMP_DIR, index_str);
    free(index_str);
    
    return (filename);
}
void print_heredoc_warning(const char *delimiter)
{
    write(STDERR_FILENO, "minishell: warning: here-document delimited by ", 48);
    write(STDERR_FILENO, "end-of-file (wanted `", 21);
    write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
    write(STDERR_FILENO, "')\n", 3);
}

// I dont get this one 
 bool write_line_to_file(int fd, const char *line)
{
    size_t len;

    len = ft_strlen(line);
    
    if (write(fd, line, len) < 0)
        return (false);
    if (write(fd, "\n", 1) < 0)
        return (false);
    
    return (true);
}

bool make_heredoc(t_redir *redir, t_shell *shell, int heredoc_index)
{
    char        *line;
    char        *tmp_filename;
    int         tmp_fd;
    const char  *delimiter;

    (void)shell;  
    
    if (!redir || !redir->filename)
        return (false);
    delimiter = redir->filename;

    tmp_filename = generate_heredoc_filename(heredoc_index);
    if (!tmp_filename)
        return (false);
    tmp_fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (tmp_fd < 0)
    {
        perror("minishell: heredoc");
        free(tmp_filename);
        return (false);
    }
    while (1)
    {
        line = readline("> ");
        
        // Ctrl+D pressed (EOF)
        if (!line)
        {
            print_heredoc_warning(delimiter);
            break;
        }
        if (ft_strcmp(delimiter, line) == 0)
        {
            free(line);
            break;
        }
        
        // Write line to file
        if (!write_line_to_file(tmp_fd, line))
        {
            perror("minishell: heredoc write");
            free(line);
            close(tmp_fd);
            unlink(tmp_filename);
            free(tmp_filename);
            return (false);
        }
        
        free(line);
    }
    close(tmp_fd);
    redir->heredoc_tmpfile = tmp_filename;
    
    return (true);
}

bool apply_heredoc(t_command *cmd, t_shell *shell, int *heredoc_index)
{
    t_redir *redir;

    if (!cmd)
        return (true);
    redir = cmd->redir;
    while (redir)
    {
        if (redir->redir_type == REDIR_OP_HERE_DOC)
        {
            (*heredoc_index)++;  
            if (!make_heredoc(redir, shell, *heredoc_index))
                return (false);
        }
        redir = redir->next;
    }
    return (true);
}
 bool prepare_heredocs_recursive(t_ast_node *node, t_shell *shell, 
                                        int *heredoc_index)
{
    if (!node)
        return (true);

    if (node->command)
        return (apply_heredoc(node->command, shell, heredoc_index));

    if (!prepare_heredocs_recursive(node->left, shell, heredoc_index))
        return (false);
    if (!prepare_heredocs_recursive(node->right, shell, heredoc_index))
        return (false);
    
    return (true);
}
bool prepare_heredocs(t_ast_node *node, t_shell *shell)
{
    int heredoc_index;

    heredoc_index = 0;  // Start counting from 0
    return (prepare_heredocs_recursive(node, shell, &heredoc_index));
}