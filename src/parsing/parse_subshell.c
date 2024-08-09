/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:26:43 by junsan            #+#    #+#             */
/*   Updated: 2024/08/10 13:02:21 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*collect_and_tokenize_subshell_data(\
							t_token **token, int subshell_status)
{
	char	*remainder;
	char	*data_in_subshell;

	remainder = NULL;
	if (*token && (*token)->type != SUBSHELL)
	{
		remainder = (*token)->data;
		*token = (*token)->next;
	}
	if (subshell_status == SINGLE)
		*token = (*token)->next;
	data_in_subshell = collect_data_until_subshell(token, subshell_status);
	if (remainder)
		return (concat_three_strings(remainder, " ", data_in_subshell));
	return (data_in_subshell);
}

static t_ast	*handler_parse_subshell(\
		char *data_in_subshell, int subshell_status)
{
	t_token	*token_head;
	t_token	*tokens_in_subshell;
	t_ast	*subshell_node;
	t_ast	*logical_node;

	logical_node = NULL;
	subshell_node = NULL;
	tokens_in_subshell = NULL;
	token_head = NULL;
	if (subshell_status == OPEN || subshell_status == CLOSE)
	{
		printf("data in subshell : %s\n", data_in_subshell);
		tokenize(data_in_subshell, &tokens_in_subshell);
		printf("token in subshell --------------------------\n");
		print_token(tokens_in_subshell);
		token_head = tokens_in_subshell;
		if (subshell_status == OPEN)
			subshell_node = new_node("(", SUBSHELL);
		else if (subshell_status == CLOSE)
			subshell_node = new_node(")", SUBSHELL);
		attach_to_tree(subshell_node, logical_node, LEFT);
		free_token(token_head);
	}
	else if (subshell_status == NESTED)
	{
		printf("nested ----------------------------------\n");
		printf("data in subshell : %s\n", data_in_subshell);
		parse_logical(&tokens_in_subshell, &logical_node);
		subshell_node = logical_node;
	}
	free(data_in_subshell);
	if (!subshell_node)
		return (free_tree(logical_node), NULL);
	return (subshell_node);
}

static t_ast	*create_subshell_node(t_token **token, int *depth)
{
	t_ast	*subshell_node;
	char	*subshell_data;
	char	*data_in_subshell;
	int		subshell_type;

	subshell_node = NULL;
	data_in_subshell = NULL;
	subshell_data = (*token)->data;
	subshell_type = (*token)->type;
	if (*depth > 0)
	{
		data_in_subshell = collect_and_tokenize_subshell_data(token, NESTED);
		if (!data_in_subshell)
			return (NULL);
		subshell_node = handler_parse_subshell(data_in_subshell, NESTED);
	}
	else
	{
		data_in_subshell = collect_and_tokenize_subshell_data(token, SINGLE);
		if (!data_in_subshell)
			return (NULL);
		if (subshell_type == SUBSHELL && subshell_data[0] == '(')
		{
			(*depth)++;
			printf("depth in plus : %d\n", *depth);
			subshell_node = handler_parse_subshell(data_in_subshell, OPEN);
		}
		else if (subshell_type == SUBSHELL && subshell_data[0] == ')')
		{
			(*depth)--;
			printf("depth in minus : %d\n", *depth);
			subshell_node = handler_parse_subshell(data_in_subshell, CLOSE);
		}
	}
	return (subshell_node);
}

static bool	process_subshell_token(\
					t_token **token, t_ast **subshell_node, int *depth)
{
	t_ast	*new_node;

	new_node = create_subshell_node(token, depth);
	if (!new_node)
		return (false);
	*subshell_node = new_node;
	return (true);
}

bool	parse_subshell(t_token **token, t_ast **node)
{
	t_ast	*subshell_node;
	int		depth;

	depth = 0;
	while (*token && (*token)->type == SUBSHELL)
	{
		if (!process_subshell_token(token, &subshell_node, &depth))
			return (free_tree(*node), false);
		if (!*node)
			*node = subshell_node;
		else
			attach_to_tree(*node, subshell_node, LEFT);
		if (depth == 0)
			break ;
		printf("depth in main : %d\n", depth);
	}
	return (true);
}
