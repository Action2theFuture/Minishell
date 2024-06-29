/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 10:41:06 by junsan            #+#    #+#             */
/*   Updated: 2024/06/27 08:57:57 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_list	*get_token_list(t_token *token)
{
	t_token_list	*token_list;

	if (!token)
		return (NULL);
	token_list = (t_token_list *)malloc(sizeof(t_token_list));
	token_list->head = token;
	token_list->tail = tokens_last(token);
	return (token_list);
}

static t_token	*new_token(const char *start, size_t len)
{
	t_token	*new_node;
	char	*data;

	data = (char *)malloc(sizeof(char) * (len + 1));
	if (!data)
		return (NULL);
	data = ft_memcpy(data, start, len);
	data[len] = '\0';
	new_node = (t_token *)malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	remove_control_characters(data);
	new_node->data = data;
	new_node->next = NULL;
	new_node->type = get_type(new_node->data);
	return (new_node);
}

void	add_token(t_token **head, const char *start, size_t len)
{
	t_token	*cur;
	t_token	*new_node;
	int		i;

	if (start == NULL || is_all_whitespace(start))
		return ;
	new_node = new_token(start, len);
	if (*head == NULL)
	{
		new_node->num = 1;
		*head = new_node;
	}
	else
	{
		i = 1;
		cur = *head;
		while (cur->next)
		{
			i++;
			cur = cur->next;
			cur->num = i;
		}
		cur->next = new_node;
		new_node->num = i + 1;
	}
}
/*
static size_t	token_size(t_token *token)
{
	size_t	len;

	len = 0;
	while (token)
	{
		token = token->next;
		len++;
	}
	return (len);
}
*/
