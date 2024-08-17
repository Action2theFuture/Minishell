/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 10:22:38 by junsan            #+#    #+#             */
/*   Updated: 2024/08/17 19:25:32 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_pipe_segment(t_ast *pipe_node, t_info *info);

static void	process_subshell_or_phrase_node(t_ast *node, t_info *info)
{
	if (node->type == SUBSHELL)
		prepare_subshell_node(node, info);
	else
	{
		if (!info->is_last && info->pipe_loc == LAST && info->in_subshell)
			info->pipe_loc = MIDDLE;
		process_phrase_node(node, info);
	}
}

static void	handle_middle_and_last_pipe_segment(t_ast *pipe_node, t_info *info)
{
	while (pipe_node->parent && pipe_node->parent->type == PIPE)
	{
		info->pipe_loc = MIDDLE;
		process_subshell_or_phrase_node(pipe_node->left, info);
		pipe_node = pipe_node->parent;
		if (!pipe_node)
			break ;
		if (info->is_re_pipe && \
			pipe_node->parent && pipe_node->parent->type == PIPE)
		{
			info->pipe_loc = FIRST;
			process_subshell_or_phrase_node(pipe_node->left, info);
			pipe_node = pipe_node->parent;
		}
	}
	info->pipe_loc = LAST;
	info->is_re_pipe = false;
	process_subshell_or_phrase_node(pipe_node->left, info);
	if (info->in_subshell)
		cleanup_and_exit(info->exit_status, NULL, NULL, info);
}

static void	process_pipe_segment(t_ast *pipe_node, t_info *info)
{
	if (pipe_node->parent && pipe_node->parent->type == PIPE)
		handle_middle_and_last_pipe_segment(pipe_node->parent, info);
	else
	{
		info->pipe_loc = LAST;
		info->is_re_pipe = false;
		process_subshell_or_phrase_node(pipe_node->left, info);
		if (info->in_subshell)
			cleanup_and_exit(info->exit_status, NULL, NULL, info);
	}
}

static void	handle_pipe_segment(t_ast *pipe_node, t_info *info)
{
	info->pipe_loc = FIRST;
	process_subshell_or_phrase_node(pipe_node->right, info);
	info->pipe_loc = LAST;
	if (pipe_node->parent && pipe_node->parent->type == PIPE)
		info->pipe_loc = MIDDLE;
	process_subshell_or_phrase_node(pipe_node->left, info);
	if (pipe_node->parent && pipe_node->parent->type == PIPE)
		process_pipe_segment(pipe_node, info);
	else
	{
		if (info->in_subshell)
			cleanup_and_exit(info->exit_status, NULL, NULL, info);
	}
}

// The bottom left pipe node is the StartNode
void	process_pipe_node(t_ast *pipe_node, t_info *info)
{
	bool	is_pipe;

	is_pipe = info->is_pipe;
	if (info->in_subshell)
		info->is_pipe = true;
	else if (is_pipe == false)
	{
		info->stdin_pipe = -1;
		info->is_pipe = true;
		info->is_re_pipe = false;
	}
	while (pipe_node && pipe_node->right && \
		pipe_node->right->type == PIPE)
	{
		pipe_node = pipe_node->right;
		info->has_multiple_pipes = true;
	}
	handle_pipe_segment(pipe_node, info);
	if (is_pipe == false)
	{
		info->has_multiple_pipes = false;
		info->is_pipe = false;
		info->is_re_pipe = false;
		info->pipe_loc = -1;
	}
}
