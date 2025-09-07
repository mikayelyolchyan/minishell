
#include "../../include/parser/parser.h"

void print_syntax_error(char *token_value)
{
    ft_putstr_fd("bash: syntax error near unexpected token `", 2);
    ft_putstr_fd(token_value, 2);
    ft_putstr_fd("'\n", 2);
}

int subshell_open_count(t_list *tokens)
{
	int open_count = 0;
	t_list *current = tokens;
	t_token *token;

	while(current != NULL)
	{
		token = (t_token *)current->content;
		if(token->ctrl_op_type == CTRL_OP_SUBSHELL_OPEN)
			open_count++;
		current = current->next;
	}
	return (open_count);
}

int subshell_closed_count(t_list *tokens)
{
    int close_count = 0;
    t_list *current = tokens;
    t_token *token;
    
    while (current != NULL)
    {
        token = (t_token *)current->content;
        if (token->ctrl_op_type == CTRL_OP_SUBSHELL_CLOSE)
            close_count++;
        current = current->next;
    }
    return (close_count);
}

bool check_subshell_balance(t_list *tokens)
{
    int open_count = subshell_open_count(tokens) ;
    int close_count = subshell_closed_count(tokens);
    
    if (open_count != close_count)
    {
        if (open_count > close_count)
            print_syntax_error("newline");
        else
            print_syntax_error(")");
        return false;
    }
    return true;
}

