/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 22:45:45 by junsan            #+#    #+#             */
/*   Updated: 2024/07/29 09:20:11 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//print_tree(root, 10);
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
		execute(root, env, exit_status);
	else
		*err = PARSE_ERROR;
	free(token_list);
	free_token(tokens);
	free_tree(root);
}

// print_token(*tokens);
static void	tokenize_and_validate(char *input, t_token **tokens, int *err)
{
	int	err_in_tokenizing;

	err_in_tokenizing = tokenize(input, tokens);
	if (err_in_tokenizing != -1)
	{
		*err = err_in_tokenizing;
		if (*tokens)
			free_token(*tokens);
		*tokens = NULL;
		return ;
	}
	*err = valid_token(*tokens);
	if (*err != -1)
	{
		free_token(*tokens);
		*tokens = NULL;
	}
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
			tokenize_and_validate(input, &tokens, &err);
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
