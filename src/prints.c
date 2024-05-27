/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 11:49:26 by junsan            #+#    #+#             */
/*   Updated: 2024/05/28 19:41:56 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token(t_token *head)
{
	if (!head)
	{
		printf("empty head\n");
		return ;
	}
	while (head)
	{
		printf("data : %s\n", head->data);
		head = head->next;
	}
}

static void	print_all(t_cmd *node)
{
	const char	*type_str;
	int			type;

	type = (int)node->type;
	type_str = "";
	if (type == 1)
		type_str = "LOGICAL";
	else if (type == 3)
		type_str = "PIPE";
	else if (type == 5)
		type_str = "REDIRECTION";
	else if (type == 6)
		type_str = "IO";
	else if (type == 20)
		type_str = "CMD";
	else if (type == 100)
		type_str = "SUBSHELL";
	else if (type == 0)
		type_str = "PHARAS";
	if (node->data)
		printf("data : %s, type : %s\n", node->data, type_str);
	else
		printf("%s\n", type_str);
}

static void	print_tree_util(t_cmd *node, int space, int depth)
{
	int		i;

	if (node == NULL)
		return ;
	space += depth;
	i = depth;
	if (node->right)
		print_tree_util(node->right, space, depth);
	printf("\n");
	while (i < space)
	{
		printf("-");
		i++;
	}
	print_all(node);
	if (node->left)
		print_tree_util(node->left, space, depth);
}

void	print_tree(t_cmd *root, int depth)
{
	if (!root)
		return ;
	print_tree_util(root, 0, depth);
}
