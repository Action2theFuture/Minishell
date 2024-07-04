/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_phrase_parts.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 22:46:47 by junsan            #+#    #+#             */
/*   Updated: 2024/07/04 16:19:13 by junsan           ###   ########.fr       */
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
			if (ft_strlen(cmd_node->left->data) == 4 && \
					ft_strncmp(cmd_node->left->data, "find", 4) == 0)
				arg_tokens = ft_strdup((*token)->data);
			else
				arg_tokens = arg_parsing(token);
			cmd_node->right = new_node(arg_tokens, ARGS);
			free(arg_tokens);
		}
		*node = cmd_node;
	}
	return (true);
}

static bool	handle_redir_in_parse_redir_part(\
			t_token **token, t_ast **phrase_node, t_ast **left)
{
	t_ast	*attach_node;
	t_ast	*right;

	right = NULL;
	attach_node = NULL;
	if (*token && (*token)->type == CMD)
	{
		parse_cmd(token, &right);
		(*phrase_node)->right = right;
		parse_io_redirection(token, &((*left)->right));
		*left = (*left)->right;
		while (*token && (*token)->type == REDIRECTION)
		{
			parse_io_redirection(token, &attach_node);
			*left = attach_to_tree(*left, attach_node, LEFT);
		}
	}
	else if (*token && (*token)->type == SUBSHELL)
	{
		if (!parse_subshell(token, &right))
			return (false);
		(*phrase_node)->right = right;
		parse_io_redirection(token, &((*left)->right));
	}
	return (true);
}

// case : [<in1 cmd1 in2>]
bool	parse_redirection_part(\
			t_token **token, t_ast **phrase_node, t_ast **node)
{
	t_ast	*left;

	left = NULL;
	if (*token && (*token)->type == REDIRECTION)
	{
		parse_redirection(token, &left);
		(*phrase_node)->left = left;
		if (!handle_redir_in_parse_redir_part(token, phrase_node, &left))
			return (false);
		*node = *phrase_node;
	}
	return (true);
}

// cose : [cmd1 < in2] or [cmd1 > in2]
bool	parse_cmd_part(t_token **token, t_ast **phrase_node, t_ast **node)
{
	t_ast	*left;

	left = NULL;
	if (!parse_cmd(token, node))
		return (false);
	if (*token && (*token)->type == REDIRECTION)
	{
		parse_redirection(token, &left);
		(*phrase_node)->left = left;
	}
	(*phrase_node)->right = *node;
	*node = *phrase_node;
	return (true);
}

bool	parse_phrase_part(\
	t_token **token, t_ast **node, \
	bool (*parse_func)(t_token **, t_ast **, t_ast **))
{
	t_ast	*phrase_node;

	phrase_node = new_node(NULL, PHRASE);
	if (!phrase_node)
		return (false);
	return (parse_func(token, &phrase_node, node));
}
