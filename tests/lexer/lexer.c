/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:03:42 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/17 19:11:29 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"
#include <stdio.h>

void		print_tokens(t_list *tokens);

static void	run_test(const char *input)
{
	t_list	*tokens;

	printf("Input: %s\n", input);
	tokens = tokenize(input);
	if (tokens)
	{
		print_tokens(tokens);
		ft_lstclear(&tokens, del_token);
	}
	else
	{
		printf("Tokenization failed\n\n");
	}
}

int	main(void)
{
	run_test("echo hello");
	run_test("cmd1 | cmd2");
	run_test("cmd1 && cmd2 || cmd3");
	run_test(" ( sub ) & ; ");
	run_test(">file >>append <in <<heredoc");
	run_test("'single quote'");
	run_test("\"double $quote\"");
	run_test("mixed'\"nested\"'");
	run_test("escaped\\ space \\\"escaped\\\"");
	run_test("hello$world '$noexpand' \"$expand\"");
	run_test("echo \"hello $USER\" > file | grep something");
	run_test("");
	run_test("   ");
	run_test("\"unfinished");
	run_test("'unfinished");
	run_test("\\\"escaped quote\\\"");
	run_test("word'with'single\"and\"double");
	run_test("$$var '$$no' \"$yes\"");
	run_test("<<delim'with quote'");
	run_test(">|&();");
	run_test("complex \"mix'ed $var\" | 'literal' && (sub \"inner\")");
	return (0);
}
