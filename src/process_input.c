/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 22:45:45 by junsan            #+#    #+#             */
/*   Updated: 2024/06/25 18:55:34 by junsan           ###   ########.fr       */
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

void	process_input(char *input, t_env *env, int *exit_status)
{
	t_token			*tokens;
	t_ast			*root;
	int				err;

	tokens = NULL;
	root = NULL;
	err = -1;
	if (input)
	{
		remove_outer_parentheses(&input, &root);
		tokenize(input, &tokens);
		//print_token(tokens);
		err = valid_token(tokens);
		if (err != -1)
		{
			free_token(tokens);
			tokens = NULL;
		}
		if (tokens)
			parse_and_execute(tokens, env, exit_status, &err);
		parse_log_error(err, exit_status);
		free(input);
	}
}
