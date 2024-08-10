/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 22:46:47 by junsan            #+#    #+#             */
/*   Updated: 2024/08/08 09:57:33 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_cmd(t_token **token, t_ast **node)
{
	t_ast	*cmd_node;
	char	*arg_tokens;

	if (*token && (*token)->type == CMD)
	{
		arg_tokens = NULL;
		cmd_node = new_node(NULL, CMD);
		if (!cmd_node)
			return (NULL);
		cmd_node->left = new_node((*token)->data, (*token)->type);
		*token = (*token)->next;
		if (*token && (*token)->type == CMD)
		{
			arg_tokens = arg_parsing(token);
			cmd_node->right = new_node(arg_tokens, ARGS);
			free(arg_tokens);
		}
		else if (*token && (*token)->type == SUBSHELL)
			return (false);
		*node = cmd_node;
	}
	return (true);
}

static bool	parse_additional_args(t_token **token, t_ast *cmd_node)
{
	char	*arg_tokens;
	char	*new_data;

	arg_tokens = arg_parsing(token);
	if (!cmd_node->right)
		cmd_node->right = new_node(arg_tokens, ARGS);
	else
	{
		new_data = concat_three_strings(cmd_node->right->data, "`", arg_tokens);
		free(cmd_node->right->data);
		cmd_node->right->data = new_data;
	}
	free(arg_tokens);
	return (true);
}

// case : [cmd1 < in2] or [cmd1 > in2]
bool	parse_cmd_with_redir(t_token **token, t_ast **phrase_node, t_ast **node)
{
	t_ast	*left;
	t_ast	*cmd_node;

	left = NULL;
	if (!parse_cmd(token, node))
		return (false);
	if (*token && (*token)->type == REDIRECTION)
	{
		if (!parse_redirection(token, &left))
			return (false);
		left->parent = *phrase_node;
		(*phrase_node)->left = left;
	}
	(*phrase_node)->right = *node;
	*node = *phrase_node;
	cmd_node = (*phrase_node)->right;
	if (*token && (*token)->type == CMD)
	{
		if (!parse_additional_args(token, cmd_node))
			return (false);
	}
	return (true);
}
