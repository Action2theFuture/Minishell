/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_phrase.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:24:59 by junsan            #+#    #+#             */
/*   Updated: 2024/07/02 08:41:41 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// what to do : cmd(left) + flag(right)
// or echo "something"
static bool	parse_cmd(t_token **token, t_ast **node)
{
	t_ast	*cmd_node;
	char	*arg_tokens;

	//printf("cmd >> \n");
	printf("token cmd : %s\n", (*token)->data);
	if (*token && (*token)->type == CMD)
	{
		arg_tokens = NULL;
		cmd_node = new_node(NULL, CMD);
		if (!cmd_node)
			return (NULL);
		// remove_double_quotes((*token)->data);
		cmd_node->left = new_node((*token)->data, (*token)->type);
		*token = (*token)->next;
		if (*token && (*token)->type == CMD)
		{
			arg_tokens = arg_parsing(token);
			cmd_node->right = new_node(arg_tokens, ARGS);
			free(arg_tokens);
		}
		*node = cmd_node;
	}
	return (true);
}

// case : [<in1 cmd1 in2>]
static bool	parse_redirection_part(\
			t_token **token, t_ast **phrase_node, t_ast **node)
{
	t_ast	*left;
	t_ast	*right;

	left = NULL;
	right = NULL;
	if (*token && (*token)->type == REDIRECTION)
	{
		parse_redirection(token, &left);
		(*phrase_node)->left = left;
		if (*token && (*token)->type == CMD)
		{
			parse_cmd(token, &right);
			(*phrase_node)->right = right;
			parse_io_redirection(token, &(left->right));
		}
		else if (*token && (*token)->type == SUBSHELL)
		{
			if (!parse_subshell(token, &right))
				return (false);
			(*phrase_node)->right = right;
			parse_io_redirection(token, &(left->right));
		}
		*node = *phrase_node;
	}
	return (true);
}

// cose : [cmd1 < in2] or [cmd1 > in2]
static bool	parse_cmd_part(t_token **token, t_ast **phrase_node, t_ast **node)
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

bool	parse_phrase(t_token **token, t_ast **node)
{
	t_ast	*phrase_node;
	t_ast	*subshell_node;

	// printf("pharse >> \n");
	subshell_node = NULL;
	if (*token && (*token)->type == SUBSHELL)
		parse_subshell(token, &subshell_node);
	if (*token && (*token)->type == REDIRECTION)
	{	
		phrase_node = new_node(NULL, PHRASE);
		if (!phrase_node)
			return (false);
		parse_redirection_part(token, &phrase_node, node);
	}
	else if (*token && (*token)->type == CMD)
	{
		phrase_node = new_node(NULL, PHRASE);
		if (!phrase_node)
			return (false);
		parse_cmd_part(token, &phrase_node, node);
	}
	if (subshell_node)
	{
		attach_to_tree(subshell_node, *node, LEFT);
		*node = subshell_node;
	}
	return (true);
}
