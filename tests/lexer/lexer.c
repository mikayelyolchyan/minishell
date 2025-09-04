/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:03:42 by miyolchy          #+#    #+#             */
/*   Updated: 2025/08/18 21:46:32 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer/lexer.h"
#include <stdio.h>

/*void		print_tokens(t_list *tokens);

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
}*/

/*int	main(void)
{
	// 🔹 Простые команды
	run_test("echo hello");
	run_test("ls -l /tmp");

	// 🔹 Операторы
	run_test("cmd1 | cmd2");
	run_test("cmd1 && cmd2 || cmd3");
	run_test("(subshell)");
	run_test("(nested (subshell))");
	run_test("cmd1 ; cmd2 & cmd3");

	// 🔹 Редиректы
	run_test(">file");
	run_test(">>append");
	run_test("<input");
	run_test("<<heredoc");
	run_test("cmd > out < in");

	// 🔹 Кавычки
	run_test("'single quoted string'");
	run_test("\"double quoted string\"");
	run_test("\"mix'ed inside double\"");
	run_test("'mix\"ed inside single'");
	run_test("word'with'single\"and\"double");
	run_test("'\"nested quotes inside single\"'");
	run_test("\"'nested quotes inside double'\"");

	// 🔹 Переменные
	run_test("echo $USER");
	run_test("echo '$NO_EXPAND'");
	run_test("echo \"$EXPAND\"");
	run_test("echo text$VARmore");
	run_test("$$var '$$no' \"$yes\"");

	// 🔹 Смешанные кейсы
	run_test("echo \"hello $USER\" > file | grep something");
	run_test("cat <<EOF | grep pattern");
	run_test("complex \"mix'ed $var\" | 'literal' && (sub \"inner\")");

	// 🔹 Пустые строки и пробелы
	run_test("");
	run_test("   ");

	// 🔹 Незакрытые кавычки
	run_test("\"unfinished");
	run_test("'unfinished");

	// 🔹 Комбинированный набор операторов
	run_test(">|&();");

	return (0);
}*/
