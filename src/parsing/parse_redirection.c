/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:25:32 by junsan            #+#    #+#             */
/*   Updated: 2024/07/31 16:31:32 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_io_redirection(t_token **token, t_ast **node)
{
	t_ast	*io_redirection_node;
	t_ast	*right;

	right = NULL;
	if (*token && (*token)->type == REDIRECTION)
	{
		io_redirection_node = new_node(NULL, IO);
		io_redirection_node->parent = *node;
		if (!io_redirection_node)
			return (false);
		*node = new_node((*token)->data, get_type_redir((*token)->data));
		*token = (*token)->next;
		while (*token && (*token)->type == CMD)
		{
			if (!right)
				right = new_node((*token)->data, ARGS);
			*token = (*token)->next;
		}
		io_redirection_node->left = *node;
		io_redirection_node->right = right;
		*node = io_redirection_node;
	}
	return (true);
}

bool	parse_redirection(t_token **token, t_ast **node)
{
	t_ast	*redirection_node;
	t_ast	*left;

	redirection_node = new_node(NULL, REDIRECTION);
	if (!redirection_node)
		return (false);
	left = NULL;
	while (*token && (*token)->type == REDIRECTION)
	{
		parse_io_redirection(token, &left);
		left = attach_to_tree(redirection_node, left, LEFT);
	}
	*node = redirection_node;
	return (true);
}
