/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:45:20 by junsan            #+#    #+#             */
/*   Updated: 2024/08/09 11:47:21 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	jump_to_next_chr(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (str[i]);
}

static bool	is_syntax_error(t_token *head)
{
	if (head == NULL)
		return (false);
	while (head->next)
		head = head->next;
	if (head->type == CMD || head->type == REDIRECTION)
		return (true);
	return (false);
}

int	add_depth_token(const char **input, int *depth, t_token **tokens)
{
	if (**input == '(')
	{
		if (is_syntax_error(*tokens))
			return (SYNTAX_ERROR);
		if (jump_to_next_chr(*input + 1) == '(')
			return (SYNTAX_ERROR);
		if (*(*input - 1) == '$')
			return (SYNTAX_ERROR);
		add_token(tokens, "(", 1);
		(*input)++;
		(*depth)++;
	}
	else if (*depth > 0 && **input == ')')
	{
		if (jump_to_next_chr(*input + 1) == ')')
			return (SYNTAX_ERROR);
		add_token(tokens, ")", 1);
		(*input)++;
		(*depth)--;
	}
	return (-1);
}

int	handle_open_subshell(\
	const char **input, int *depth, const char **start, t_token **list)
{
	if (is_syntax_error(*list))
		return (SYNTAX_ERROR);
	if (jump_to_next_chr(*input + 1) == '(')
		return (SYNTAX_ERROR);
	add_token(list, "(", 1);
	(*depth)++;
	(*input)++;
	*start = *input;
	return (-1);
}

int	handle_close_subshell(\
	const char **input, int *depth, const char **start, t_token **list)
{
	if (*depth > 0)
	{
		if (jump_to_next_chr(*input + 1) == ')')
			return (SYNTAX_ERROR);
		while (ft_isspace(**input))
			(*input)++;
		add_token(list, ")", 1);
		(*depth)--;
		(*input)++;
		*start = *input;
	}
	return (-1);
}
