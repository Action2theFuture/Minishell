/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:07:48 by junsan            #+#    #+#             */
/*   Updated: 2024/08/13 11:54:43 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_subshell_balanced(t_token *head)
{
	t_token	*cur;
	int		depth;

	depth = 0;
	cur = head;
	while (cur)
	{
		if (cur->data[0] == '(')
			depth++;
		else if (cur->data[0] == ')')
		{
			depth--;
			if (depth < 0)
				return (false);
		}
		cur = cur->next;
	}
	return (depth == 0);
}

static bool	process_parentheses(t_token **cur, int *depth)
{
	if ((*cur)->data[0] == '(')
	{
		++(*depth);
		*cur = (*cur)->next;
		return (true);
	}
	else if ((*cur)->data[0] == ')')
	{
		--(*depth);
		*cur = (*cur)->next;
		return (true);
	}
	return (false);
}

static int	calculate_len(t_token *cur)
{
	int		depth;
	int		len;

	depth = 0;
	len = 0;
	while (cur)
	{
		if (process_parentheses(&cur, &depth))
			continue ;
		if (depth >= 1 && cur->data[0] != '(' && cur->data[0] != ')')
			len += ft_strlen(cur->data) + 1;
		cur = cur->next;
	}
	return (len);
}

static void	process_token_list(\
	t_token **token, char *result, int *depth, int *result_idx)
{
	int	len;

	while (*token)
	{
		if (process_parentheses(token, depth))
			continue ;
		if (*depth >= 1 && (*token)->data[0] != '(' && (*token)->data[0] != ')')
		{
			len = ft_strlen((*token)->data);
			ft_strlcpy(result + *result_idx, (*token)->data, len + 1);
			*result_idx += len;
			result[(*result_idx)++] = ' ';
		}
		if (*depth == 0)
			break ;
		*token = (*token)->next;
	}
}

char	*remove_nested_subshell(t_token **token)
{
	t_token	*cur;
	char	*result;
	int		depth;
	int		result_idx;
	int		len;

	cur = *token;
	len = calculate_len(cur);
	result = (char *)malloc(len + 1);
	if (!is_subshell_balanced(cur) || !result)
		return (NULL);
	depth = 0;
	result_idx = 0;
	process_token_list(token, result, &depth, &result_idx);
	len = ft_strlen(result);
	if (len > 0 && result[len - 1] == ' ')
		result[len - 1] = '\0';
	return (result);
}
