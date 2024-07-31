/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:25:32 by junsan            #+#    #+#             */
/*   Updated: 2024/07/31 17:58:13 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_io_redirection(t_token **token, t_ast **node)
{
	t_ast	*io_redirection_node;
	char	*arg_tokens;

	if (*token && (*token)->type == REDIRECTION)
	{
		io_redirection_node = new_node(NULL, IO);
		io_redirection_node->parent = *node;
		if (!io_redirection_node)
			return (false);
		*node = new_node((*token)->data, get_type_redir((*token)->data));
		*token = (*token)->next;
		if (*token && (*token)->type == CMD)
		{
			arg_tokens = arg_parsing(token);
			io_redirection_node->right = new_node(arg_tokens, ARGS);
			free(arg_tokens);
		}
		io_redirection_node->left = *node;
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
