/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:26:43 by junsan            #+#    #+#             */
/*   Updated: 2024/08/10 17:59:46 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*collect_and_tokenize_subshell_data(\
						char **remainder, t_token **token, int subshell_status)
{
	char	*data_in_subshell;

	if (*token && (*token)->type != SUBSHELL)
	{
		*remainder = (*token)->data;
		*token = (*token)->next;
	}
	if (subshell_status == SINGLE)
		*token = (*token)->next;
	data_in_subshell = collect_data_until_subshell(token, subshell_status);
	return (data_in_subshell);
}

static t_ast	*process_single_subshell(\
	char *data_in_subshell, int *depth, int subshell_type, char *subshell_data)
{
	t_ast	*subshell_node;

	subshell_node = NULL;
	if (subshell_type == SUBSHELL && subshell_data[0] == '(')
	{
		(*depth)++;
		subshell_node = handler_parse_subshell(data_in_subshell, OPEN);
	}
	else if (subshell_type == SUBSHELL && subshell_data[0] == ')')
	{
		(*depth)--;
		subshell_node = handler_parse_subshell(data_in_subshell, CLOSE);
	}
	return (subshell_node);
}

static t_ast	*process_nested_subshell(t_token **token)
{
	t_ast	*subshell_node;
	t_ast	*remainder_node;
	char	*data_in_subshell;
	char	*remainder;

	subshell_node = NULL;
	remainder = NULL;
	remainder_node = NULL;
	data_in_subshell = collect_and_tokenize_subshell_data(\
								&remainder, token, NESTED);
	if (!data_in_subshell)
		return (NULL);
	subshell_node = handler_parse_subshell(data_in_subshell, NESTED);
	if (remainder)
	{
		remainder_node = new_node(remainder, get_type(remainder));
		remainder_node->left = subshell_node;
		return (remainder_node);
	}
	return (subshell_node);
}

// depth which is more than 1 is nested shell
static bool	process_subshell_token(\
					t_token **token, t_ast **subshell_node, int *depth)
{
	char	*subshell_data;
	char	*data_in_subshell;
	int		subshell_type;

	data_in_subshell = NULL;
	subshell_data = (*token)->data;
	subshell_type = (*token)->type;
	if (*depth > 0)
		return (*subshell_node = process_nested_subshell(token), true);
	data_in_subshell = collect_and_tokenize_subshell_data(\
								NULL, token, SINGLE);
	if (!data_in_subshell)
		return (false);
	return (*subshell_node = process_single_subshell(\
			data_in_subshell, depth, subshell_type, subshell_data), true);
}

bool	parse_subshell(t_token **token, t_ast **node)
{
	t_ast	*subshell_node;
	int		depth;

	depth = 0;
	while (*token)
	{
		if (!process_subshell_token(token, &subshell_node, &depth))
			return (free_tree(*node), false);
		if (!*node)
			*node = subshell_node;
		else
			attach_to_tree(*node, subshell_node, LEFT);
		if (depth == 0)
			break ;
	}
	return (true);
}
