/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 22:12:12 by junsan            #+#    #+#             */
/*   Updated: 2024/08/04 14:44:51 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// printf("status : %d, exit status : %d\n", info->status, info->exit_status);
void	process_logical_node(t_ast *node, t_info *info)
{
	int	status;

	traverse_tree(node->right, info);
	status = info->exit_status;
	if ((ft_strncmp(node->data, "&&", 2) == 0 && status == SUCCESS) || \
		(ft_strncmp(node->data, "||", 2) == 0 && status > 0))
		traverse_tree(node->left, info);
}

static void	process_io_redirections(t_ast *redir_node, t_info *info)
{
	info->status = handle_io_redirection(redir_node->left, info);
	if (redir_node->right)
		free_args(info->redir_args);
	if (redir_node->right && info->status == SUCCESS)
		info->status = handle_io_redirection(redir_node->right, info);
	if ((info->pipe_loc == FIRST && info->has_multiple_pipes) || \
		info->pipe_loc == MIDDLE)
	{
		info->is_re_pipe = true;
		info->pipe_loc = LAST;
	}
}

// left is redir, right is args
void	process_phrase_node(t_ast *node, t_info *info)
{
	t_ast	*redir_node;
	t_ast	*cmd_node;

	if (node == NULL)
		return ;
	backup_fds(info);
	redir_node = node->left;
	cmd_node = node->right;
	if (redir_node && redir_node->type != SUBSHELL && info->status == SUCCESS)
		process_io_redirections(redir_node, info);
	if (cmd_node && info->status == SUCCESS)
		info->exit_status = dispatch_cmd(cmd_node, info);
	(free_args(info->redir_args), info->redir_args = NULL, restore_fds(info));
}
