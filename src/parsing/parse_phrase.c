/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_phrase.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:24:59 by junsan            #+#    #+#             */
/*   Updated: 2024/08/08 10:00:43 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	parse_phrase_part(
	t_token **token, t_ast **node,
	bool (*parse_func)(t_token **, t_ast **, t_ast **))
{
	t_ast	*phrase_node;

	phrase_node = new_node(NULL, PHRASE);
	if (!phrase_node)
		return (false);
	phrase_node->parent = *node;
	if (!parse_func(token, &phrase_node, node))
		return (free_tree(phrase_node), false);
	return (true);
}

bool	parse_phrase(t_token **token, t_ast **node)
{
	t_ast	*subshell_node;

	subshell_node = NULL;
	if (*token && (*token)->type == SUBSHELL)
		parse_subshell(token, &subshell_node);
	if (subshell_node)
		*node = subshell_node;
	if (*token && (*token)->type == REDIRECTION)
	{
		if (!parse_phrase_part(\
				token, node, parse_redirection_with_cmd))
			return (free(subshell_node), false);
	}
	else if (*token && (*token)->type == CMD)
	{
		if (!parse_phrase_part(token, node, parse_cmd_with_redir))
			return (free(subshell_node), false);
	}
	return (true);
}
