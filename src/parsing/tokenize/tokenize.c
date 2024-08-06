/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:39:22 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 12:13:16 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_character(\
		const char **input, const char **start, int *depth, t_token **tokens)
{
	int	err;

	err = -1;
	if (**input == '"' || **input == '\'')
	{
		err = handle_quotes(input, start, tokens);
		if (*depth > 0 && **input == ')')
			handle_close_subshell(input, depth, start, tokens);
	}
	else if (**input == '(')
		err = handle_open_subshell(input, depth, start, tokens);
	else if (*depth > 0 && **input == ')')
		err = handle_close_subshell(input, depth, start, tokens);
	else
	{
		handle_operators_and_spaces(input, start, tokens);
		err = add_depth_token(input, depth, tokens);
	}
	if (err != -1)
		return (err);
	return (err);
}

int	tokenize(const char *input, t_token **tokens)
{
	const char	*start;
	int			depth;
	int			err;

	start = input;
	depth = 0;
	while (*input)
	{
		err = process_character(&input, &start, &depth, tokens);
		if (err != -1)
			return (err);
		if (*input)
			input++;
	}
	if (input > start)
		add_token(tokens, start, input - start);
	return (-1);
}
