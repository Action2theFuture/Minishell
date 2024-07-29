/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:22:54 by junsan            #+#    #+#             */
/*   Updated: 2024/07/29 11:10:26 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*get_type_str_redir(int type)
{
	if (type == IN_REDIR)
		return ("IN_REDIR");
	else if (type == IN_HEREDOC)
		return ("IN_HEREDOC");
	else if (type == IN_HERESTR)
		return ("IN_HERESTR");
	else if (type == OUT_REDIR)
		return ("OUT_REDIR");
	else if (type == OUT_APPEND)
		return ("OUT_APPEND");
	else if (type == NOT_REDIR)
		return ("NOT_REDIR");
	return ("UNKNOWN");
}

static const char	*get_type_str(int type)
{
	if (type == ROOT)
		return ("ROOT");
	else if (type == LOGICAL)
		return ("LOGICAL");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECTION)
		return ("REDIRECTION");
	else if (type == IO)
		return ("IO");
	else if (type == CMD)
		return ("CMD");
	else if (type == SUBSHELL)
		return ("SUBSHELL");
	else if (type == PHRASE)
		return ("PHRASE");
	else if (type == FILE_NAME)
		return ("FILE_NAME");
	else if (type == ARGS)
		return ("ARGS");
	return (get_type_str_redir(type));
}

static void	print_all(t_ast *node, const char *direction)
{
	const char	*type_str;

	type_str = get_type_str((int)node->type);
	if (node->data)
		printf("%s: data : %s, type : %s\n", direction, node->data, type_str);
	else
		printf("%s: %s\n", direction, type_str);
}

static void	print_tree_util(\
			t_ast *node, int space, int depth, const char *direction)
{
	int		i;

	if (node == NULL)
		return ;
	space += depth;
	i = depth;
	if (node->right)
		print_tree_util(node->right, space, depth, "Right");
	printf("\n");
	while (i < space)
	{
		printf("-");
		i++;
	}
	print_all(node, direction);
	if (node->left)
		print_tree_util(node->left, space, depth, "Left");
}

void	print_tree(t_ast *root, int depth)
{
	if (!root)
		return ;
	printf("start----------------------------------\n");
	print_tree_util(root, 0, depth, "Root");
	printf("end----------------------------------\n");
}
