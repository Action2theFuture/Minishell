/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:26:43 by junsan            #+#    #+#             */
/*   Updated: 2024/07/28 22:10:25 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*collect_and_tokenize_subshell_data(t_token **token)
{
	t_token	*tokens_in_subshell;
	char	*data_in_subshell;

	tokens_in_subshell = NULL;
	*token = (*token)->next;
	data_in_subshell = collect_data_until_subshell(token);
	tokenize(data_in_subshell, &tokens_in_subshell);
	free(data_in_subshell);
	return (tokens_in_subshell);
}

static t_ast	*parse_single_subshell(\
		t_token *tokens_in_subshell, int subshell_status)
{
	t_token	*token_head;
	t_ast	*subshell_node;
	t_ast	*logical_node;

	logical_node = NULL;
	subshell_node = NULL;
	token_head = tokens_in_subshell;
	parse_logical(&tokens_in_subshell, &logical_node);
	free_token(token_head);
	if (subshell_status == OPEN)
		subshell_node = new_node("(", SUBSHELL);
	else
		subshell_node = new_node(")", SUBSHELL);
	if (!subshell_node)
		return (free_tree(logical_node), NULL);
	subshell_node->left = logical_node;
	return (subshell_node);
}

static t_ast	*create_subshell_node(t_token **token, char **subshell)
{
	t_token	*tokens_in_subshell;
	t_ast	*subshell_node;

	*subshell = (*token)->data;
	tokens_in_subshell = collect_and_tokenize_subshell_data(token);
	if (!tokens_in_subshell)
		return (NULL);
	if ((*subshell)[0] == '(')
		subshell_node = parse_single_subshell(tokens_in_subshell, OPEN);
	else if ((*subshell)[0] == ')')
		subshell_node = parse_single_subshell(tokens_in_subshell, CLOSE);
	else
		subshell_node = NULL;
	return (subshell_node);
}

// The reason for the code for close is for subshells within subshells 
static bool	process_subshell_token(t_token **token, t_ast **cur_node)
{
	t_ast	*subshell_node;
	t_ast	*last_node;
	char	*subshell;

	subshell_node = NULL;
	last_node = NULL;
	if (*token && (*token)->type == SUBSHELL)
		subshell_node = create_subshell_node(token, &subshell);
	if (!subshell_node)
		return (false);
	if (!(*cur_node))
		*cur_node = subshell_node;
	else
	{
		last_node = *cur_node;
		while (last_node->left)
			last_node = last_node->left;
		last_node->left = subshell_node;
	}
	return (true);
}

bool	parse_subshell(t_token **token, t_ast **node)
{
	t_ast	*cur_node;
	int		depth;

	cur_node = NULL;
	depth = 0;
	while (*token && (*token)->type == SUBSHELL)
	{
		if (*token && (*token)->data[0] == '(')
			depth++;
		if (*token && (*token)->data[0] == ')')
			depth--;
		if (depth == 0)
		{
			while (*token && (*token)->data[0] == ')')
				*token = (*token)->next;
			break ;
		}
		if (!process_subshell_token(token, &cur_node))
			return (free_tree(cur_node), false);
	}
	*node = cur_node;
	return (true);
}
