/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 10:49:14 by junsan            #+#    #+#             */
/*   Updated: 2024/06/26 12:21:10 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	parsor(t_token **token, t_ast **root, int start)
{
	bool	is_parse;
	t_ast	*left;

	if (!token || !*token)
		return (NULL);
	left = NULL;
	is_parse = false;
	if (start == LOGICAL)
		is_parse = parse_logical(token, &left);
	else if (start == PIPE)
		is_parse = parse_pipe(token, &left);
	else
		is_parse = parse_phrase(token, &left);
	if (!is_parse)
		return (false);
	if (*root)
		(*root)->left = left;
	else
		*root = left;
	return (true);
}

bool	parsing_tree(t_token_list **tokens, t_ast **root)
{
	t_token		*tmp;
	t_token		*token;
	int			priority;

	if (!tokens || !*tokens)
		return (NULL);
	token = (*tokens)->head;
	tmp = (*tokens)->head;
	priority = INT_MAX;
	while (tmp->next)
	{
		if ((int)tmp->type < priority)
			priority = (int)tmp->type;
		tmp = tmp->next;
	}
	return (parsor(&token, root, priority));
}
