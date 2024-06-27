/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:44:23 by junsan            #+#    #+#             */
/*   Updated: 2024/06/27 13:54:38 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
bool	process_subshell_node(t_ast *node)
{
	t_info	info;

	init_info(&info);
	if (node->data)
		printf("subhshell data : %s\n", node->data);
	traverse_tree(node->left, &info);
	if (info.status == SUCCESS)
		return (SUCCESS);
	else
		return (FAILURE);
}*/

// left is redir, right is args
void	process_phrase_node(t_ast *node, t_info *info)
{
	t_ast	*redir_node;
	t_ast	*cmd_node;

	(void)info;
	redir_node = node->left;
	cmd_node = node->right;
	if (redir_node)
	{
		info->status = handle_io_redirection(redir_node->left, info);
		if (redir_node->right)
			info ->status = handle_io_redirection(redir_node->right, info);
	}
	if (cmd_node)
		info->exit_status = dispatch_cmd(cmd_node, info);
}
