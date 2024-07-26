/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 12:05:45 by junsan            #+#    #+#             */
/*   Updated: 2024/07/26 12:54:58 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*new_node(const char *data, t_type type)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->data = NULL;
	if (data)
		new_node->data = ft_strdup(data);
	new_node->type = type;
	new_node->parent = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_ast	*attach_to_tree(t_ast *root, t_ast *node, int side)
{
	if (!root)
		root = node;
	else
	{
		if (side == LEFT)
			root->left = attach_to_tree(root->left, node, side);
		else
			root->right = attach_to_tree(root->right, node, side);
	}
	return (root);
}

void	free_tree(t_ast *root)
{
	if (root == NULL)
		return ;
	if (root->left)
	{
		free_tree(root->left);
		root->left = NULL;
	}
	if (root->right)
	{
		free_tree(root->right);
		root->right = NULL;
	}
	if (root->data)
	{
		free(root->data);
		root->data = NULL;
	}
	root->parent = NULL;
	free(root);
}

int	calculate_depth(const char *str)
{
	int	depth;
	int	max_depth;

	depth = 0;
	max_depth = 0;
	while (*str)
	{
		if (*str == '(')
		{
			depth++;
			if (depth > max_depth)
				max_depth = depth;
		}
		else if (*str == ')')
			depth--;
		str++;
	}
	return (max_depth);
}

void	remove_outer_parentheses(char **str, t_ast **root)
{
	int		len;
	int		depth;

	len = ft_strlen(*str);
	depth = calculate_depth(*str);
	if (depth > 1 && len >= 2 && (*str)[0] == '(' && (*str)[len - 1] == ')')
	{
		ft_memmove(*str, *str + 1, len - 2);
		(*str)[len - 2] = '\0';
		*root = new_node("(", SUBSHELL);
	}
}
