/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:25:32 by junsan            #+#    #+#             */
/*   Updated: 2024/07/31 19:52:08 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_redirection_token(\
			t_token **token, t_ast **node, t_ast *io_redirection_node)
{
	char	*arg_tokens;

	if (*token && (*token)->type == CMD)
	{
		if ((*node)->type == OUT_REDIR || (*node)->type == OUT_APPEND)
		{
			arg_tokens = arg_parsing(token);
			io_redirection_node->right = new_node(arg_tokens, ARGS);
			free(arg_tokens);
		}
		else
		{
			io_redirection_node->right = new_node((*token)->data, ARGS);
			*token = (*token)->next;
		}
	}
}

bool	parse_io_redirection(t_token **token, t_ast **node)
{
	t_ast	*io_redirection_node;

	if (*token && (*token)->type == REDIRECTION)
	{
		io_redirection_node = new_node(NULL, IO);
		if (!io_redirection_node)
			return (false);
		io_redirection_node->parent = *node;
		*node = new_node((*token)->data, get_type_redir((*token)->data));
		*token = (*token)->next;
		handle_redirection_token(token, node, io_redirection_node);
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
