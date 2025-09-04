#include "../../include/lexer/lexer.h"
#include "../../include/parser/parser.h"

#include <stdio.h>


void free_tokens(t_list *tokens)
{
    t_list *current = tokens;
    t_list *next;
    t_token *token;
    
    while (current)
    {
        next = current->next;
        token = (t_token *)current->content;
        
        if (token->value)
            free(token->value);
        free(token);
        free(current);
        
        current = next;
    }
}


void		print_tokens(t_list *tokens);

static void	run_test(const char *input)
{
	t_list	*tokens;

	printf("Input: %s\n", input);
	//tokens = tokenize(input);
    tokens = lexical_analyze(input);
	bool result = syntax_analyze(tokens);
    if(result == false)
        printf("Syntax analyze failed, invalid input\n");
    else if (result == true)
    {
        printf("Syntax analyze SUCSSES\n");
    }
    else
        printf("idk what happened\n");
    free_tokens(tokens);

}
int main()
{
    // Your original tests
    run_test("&& ||");        // Should fail - starts with &&
    run_test(">> &&");        // Should fail - >> needs filename, && needs command
    run_test(">>");           // Should fail - >> needs filename
    run_test("ls > << cat");  // Should fail - > followed by <<
    run_test("> <<");         // Should fail - > needs filename, then <<
    run_test("ls ");          // Should pass
    run_test("");             // Should pass - empty
    run_test("()");           // Should fail - empty subshell
    run_test("()))");         // Should fail - unbalanced parens
    
    // Additional comprehensive tests
    run_test("ls | grep test");           // Should pass
    run_test("ls && echo done");          // Should pass  
    run_test("ls ||");                    // Should fail - || at end
    run_test("| ls");                     // Should fail - starts with |
    run_test("ls > file.txt");            // Should pass
    run_test("ls >");                     // Should fail - > needs filename
    run_test("(ls | grep test)");         // Should pass
    run_test("ls && (echo hi)");          // Should pass
    run_test("(ls &&)");                  // Should fail - && at end in subshell
    run_test("ls()");                     // Should fail - word followed by (
    run_test("ls > file1 > file2");       // Should pass (last redirect wins)
    run_test("< input ls");               // Should pass
    run_test("ls << EOF");                // Should pass (heredoc)
    run_test("ls < < file");              // Should fail - < followed by <
    
    return 0;
}
