/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 22:12:12 by junsan            #+#    #+#             */
/*   Updated: 2024/07/31 09:35:52 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_pipe_segment(t_ast *pipe_node, t_info *info);

static void	handle_middle_and_last_pipe_segment(t_ast *pipe_node, t_info *info)
{
	info->pipe_loc = MIDDLE;
	process_phrase_node(pipe_node->left, info);
	pipe_node = pipe_node->parent;
	if (info->is_re_pipe)
	{
		process_pipe_segment(pipe_node, info);
		return ;
	}
	while (pipe_node->parent && pipe_node->parent->type == PIPE)
	{
		process_phrase_node(pipe_node->left, info);
		pipe_node = pipe_node->parent;
		if (info->is_re_pipe)
		{
			process_pipe_segment(pipe_node, info);
			return ;
		}
	}
	info->pipe_loc = LAST;
	info->is_re_pipe = false;
	process_phrase_node(pipe_node->left, info);
}

static void	process_pipe_segment(t_ast *pipe_node, t_info *info)
{
	if (info->is_re_pipe && pipe_node->parent && \
			pipe_node->parent->type == PIPE)
	{
		info->pipe_loc = FIRST;
		process_phrase_node(pipe_node->left, info);
	}
	if (pipe_node->parent && pipe_node->parent->type == PIPE)
		handle_middle_and_last_pipe_segment(pipe_node, info);
	else
	{
		info->pipe_loc = LAST;
		info->is_re_pipe = false;
		process_phrase_node(pipe_node->left, info);
	}
}

// The bottom left pipe node is the StartNode
void	process_pipe_node(t_ast *pipe_node, t_info *info)
{
	if (info->is_pipe == false)
	{
		info->stdin_pipe = -1;
		info->is_pipe = true;
		info->is_re_pipe = false;
		while (pipe_node && pipe_node->right && pipe_node->right->type == PIPE)
			pipe_node = pipe_node->right;
		info->pipe_loc = FIRST;
		process_phrase_node(pipe_node->right, info);
		process_pipe_segment(pipe_node, info);
		info->is_pipe = false;
		info->is_re_pipe = false;
		info->pipe_loc = -1;
	}
}

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
	if (redir_node && redir_node->type != SUBSHELL)
	{
		info->status = handle_io_redirection(redir_node->left, info);
		if (redir_node->right && info->status == SUCCESS)
			info->status = handle_io_redirection(redir_node->right, info);
		if (info->pipe_loc == MIDDLE)
		{
			info->pipe_loc = LAST;
			info->is_re_pipe = true;
		}
	}
	if (cmd_node && info->status == SUCCESS)
		info->exit_status = dispatch_cmd(cmd_node, info);
	restore_fds(info);
}
