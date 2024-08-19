/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:26:43 by junsan            #+#    #+#             */
/*   Updated: 2024/08/19 16:16:38 by junsan           ###   ########.fr       */
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

/*
// depth which is more than 1 is nested shell
	if (*depth > 0 && subshell_data[0] == ')') -> subshell close
	else if (*depth > 0 && subshell_data[0] != ')') -> nested shell
	else -> subshell open
*/
static int	process_subshell_token(\
					t_token **token, t_ast **subshell_node, int *depth)
{
	char	*subshell_data;
	char	*data_in_subshell;
	int		subshell_type;

	data_in_subshell = NULL;
	subshell_data = (*token)->data;
	subshell_type = (*token)->type;
	if (*depth == 0 && subshell_data[0] == '(' && \
			(*token)->next && (*token)->next->type == SUBSHELL)
		return ((*depth)++, *token = (*token)->next, \
		*subshell_node = new_node("(", SUBSHELL), OPEN);
	else if (*depth > 0 && subshell_data[0] == ')')
		return (*token = (*token)->next, (*depth)--, \
		*subshell_node = new_node(")", SUBSHELL), CLOSE);
	else if (*depth > 0 && subshell_data[0] != ')')
		return (*subshell_node = process_nested_subshell(token),  NESTED);
	data_in_subshell = collect_and_tokenize_subshell_data(\
						NULL, token, SINGLE);
	if (!data_in_subshell)
		return (-1);
	if (subshell_type == SUBSHELL && subshell_data[0] == '(')
		return ((*depth)++, \
	*subshell_node = handler_parse_subshell(data_in_subshell, OPEN), OPEN);
	return (-1);
}

static bool	handle_subshell_node(\
	t_ast **node, t_ast *subshell_node, int subshell_status)
{
	if (!*node)
		*node = subshell_node;
	else if (subshell_status == CLOSE)
		attach_to_tree(*node, subshell_node, LEFT);
	else
	{
		if ((*node)->left && !(*node)->left->left)
			(*node)->left->left = subshell_node;
		else
		{
			subshell_node->right = (*node)->left;
			(*node)->left = subshell_node;
		}
	}
	return (true);
}

/*
//The first node can have the next new node 
//only if both the left and right sides have it, 
//so after checking the first node, we add the subshell node to the next node
	if (!(*node)->left->left)
		(*node)->left->left = subshell_node;
	else
	{
		subshell_node->right = (*node)->left;
		(*node)->left = subshell_node;
	}
*/
bool	parse_subshell(t_token **token, t_ast **node)
{
	t_ast	*subshell_node;
	int		depth;
	int		subshell_status;

	depth = 0;
	subshell_node = NULL;
	subshell_status = -1;
	while (*token)
	{
		subshell_status = process_subshell_token(token, &subshell_node, &depth);
		if (subshell_status == -1)
			return (free_tree(*node), false);
		if (!handle_subshell_node(node, subshell_node, subshell_status))
			return (free_tree(*node), false);
		if (depth == 0)
			break ;
	}
	return (true);
}
