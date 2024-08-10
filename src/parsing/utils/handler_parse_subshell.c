/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_parse_subshell.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:49:41 by junsan            #+#    #+#             */
/*   Updated: 2024/08/10 18:00:10 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*parse_open_close_subshell(\
			const char *data_in_subshell, int subshell_status)
{
	t_token	*token_head;
	t_token	*tokens_in_subshell;
	t_ast	*subshell_node;
	t_ast	*logical_node;

	logical_node = NULL;
	subshell_node = NULL;
	tokens_in_subshell = NULL;
	token_head = NULL;
	tokenize(data_in_subshell, &tokens_in_subshell);
	token_head = tokens_in_subshell;
	parse_logical(&tokens_in_subshell, &logical_node);
	if (subshell_status == OPEN)
		subshell_node = new_node("(", SUBSHELL);
	else if (subshell_status == CLOSE)
		subshell_node = new_node(")", SUBSHELL);
	attach_to_tree(subshell_node, logical_node, LEFT);
	free_token(token_head);
	return (subshell_node);
}

t_ast	*handler_parse_subshell(\
		char *data_in_subshell, int subshell_status)
{
	t_ast	*subshell_node;
	t_ast	*nest_subshell_node;

	subshell_node = NULL;
	nest_subshell_node = NULL;
	if (subshell_status == OPEN || subshell_status == CLOSE)
		subshell_node = parse_open_close_subshell(\
							data_in_subshell, subshell_status);
	else if (subshell_status == NESTED)
	{
		nest_subshell_node = new_node(data_in_subshell, NESTED_SUBSHELL);
		subshell_node = nest_subshell_node;
	}
	free(data_in_subshell);
	if (!subshell_node)
		return (NULL);
	return (subshell_node);
}
