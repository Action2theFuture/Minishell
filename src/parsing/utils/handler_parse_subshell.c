/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_parse_subshell.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:49:41 by junsan            #+#    #+#             */
/*   Updated: 2024/08/13 11:51:07 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*parse_open_subshell(const char *data_in_subshell)
{
	t_token	*token_head;
	t_token	*tokens_in_subshell;
	t_ast	*subshell_node;
	t_ast	*logical_node;

	logical_node = NULL;
	subshell_node = NULL;
	tokens_in_subshell = NULL;
	token_head = NULL;
	subshell_node = new_node("(", SUBSHELL);
	if (!subshell_node)
		return (NULL);
	tokenize(data_in_subshell, &tokens_in_subshell);
	token_head = tokens_in_subshell;
	parse_logical(&tokens_in_subshell, &logical_node);
	subshell_node->left = logical_node;
	free_token(token_head);
	return (subshell_node);
}

static t_ast	*create_nested_subshell_node(char *data_in_subshell)
{
	t_token	*tokens_in_subshell;
	t_token	*token_head;
	t_ast	*subshell_node;
	int		type;

	subshell_node = NULL;
	tokens_in_subshell = NULL;
	if (data_in_subshell[0] == '(' && \
	data_in_subshell[ft_strlen(data_in_subshell) - 1] == ')')
	{
		type = NESTED_SUBSHELL;
		subshell_node = new_node(data_in_subshell, type);
	}
	else
	{
		type = SUBSHELL;
		tokenize(data_in_subshell, &tokens_in_subshell);
		token_head = tokens_in_subshell;
		parse_phrase(&tokens_in_subshell, &subshell_node);
		free_token(token_head);
	}
	return (subshell_node);
}

// There is a slightly problematic flow where 
// when a nested subshell is started, 
// all commands after it go into a NESTED conditional. 
// This isn't a problem for the behavior, 
// so I just added code to treat any parentheses 
// at either end of the string in data_in_subshell as nested_subshell type
t_ast	*handler_parse_subshell(\
		char *data_in_subshell, int subshell_status)
{
	t_ast	*subshell_node;

	subshell_node = NULL;
	if (subshell_status == OPEN)
		subshell_node = parse_open_subshell(data_in_subshell);
	else if (subshell_status == NESTED)
		subshell_node = create_nested_subshell_node(data_in_subshell);
	if (data_in_subshell)
		free(data_in_subshell);
	return (subshell_node);
}
