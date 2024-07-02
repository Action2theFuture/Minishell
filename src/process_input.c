/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 22:45:45 by junsan            #+#    #+#             */
/*   Updated: 2024/07/02 09:05:53 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_and_execute(\
		t_token *tokens, t_env *env, int *exit_status, int *err)
{
	t_token_list	*token_list;
	t_ast			*root;
	bool			is_parse;

	root = NULL;
	token_list = get_token_list(tokens);
	is_parse = parsing_tree(&token_list, &root);
	if (is_parse)
	{
		print_tree(root, 10);
		execute(root, env, exit_status);
	}
	else
		*err = PARSE_ERROR;
	free_tree(root);
	free(token_list);
	free_token(tokens);
}

static bool	are_parentheses_balanced(const char *input)
{
	int		balance;
	int		i;
	bool	in_single_quotes;
	bool	in_double_quotes;

	in_single_quotes = false;
	in_double_quotes = false;
	i = -1;
	balance = 0;
	while (input[++i])
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (!in_single_quotes && !in_double_quotes)
		{
			if (input[i] == '(')
				balance++;
			else if (input[i] == ')')
			{
				balance--;
				if (balance < 0)
					return (false);
			}
		}
	}
	return (balance == 0);
}

void	process_input(char *input, t_env *env, int *exit_status)
{
	t_token			*tokens;
	int				err;

	tokens = NULL;
	err = -1;
	if (input)
	{
		if (are_parentheses_balanced(input))
		{
			tokenize(input, &tokens);
			print_token(tokens);
			err = valid_token(tokens);
			if (err != -1)
			{
				free_token(tokens);
				tokens = NULL;
			}
			if (tokens)
				parse_and_execute(tokens, env, exit_status, &err);
			parse_log_error(err, exit_status);
		}
		else
		{
			*exit_status = 1;
			parse_log_error(TOKEN_ERROR, exit_status);
		}
		free(input);
	}
}
