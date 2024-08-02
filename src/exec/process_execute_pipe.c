/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 10:22:38 by junsan            #+#    #+#             */
/*   Updated: 2024/08/02 20:36:42 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_pipe_segment(t_ast *pipe_node, t_info *info);

static void	handle_middle_and_last_pipe_segment(t_ast *pipe_node, t_info *info)
{
	info->pipe_loc = MIDDLE;
	process_phrase_node(pipe_node->left, info);
	print_tree(pipe_node, 10);
	if (info->is_re_pipe)
	{
		process_pipe_segment(pipe_node->parent, info);
		return ;
	}
	while (pipe_node->parent && pipe_node->parent->type == PIPE)
	{
		process_phrase_node(pipe_node->left, info);
		if (info->is_re_pipe)
		{
			process_pipe_segment(pipe_node->parent, info);
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
	else if (!pipe_node->parent)
	{
		info->pipe_loc = LAST;
		info->is_re_pipe = false;
		process_phrase_node(pipe_node->left, info);
		return ;
	}
}

// The bottom left pipe node is the StartNode
void	process_pipe_node(t_ast *pipe_node, t_info *info)
{
	if (info->is_pipe == false)
	{
		info->stdin_pipe = -1;
		info->is_pipe = true;
		while (pipe_node && pipe_node->right && pipe_node->right->type == PIPE)
			pipe_node = pipe_node->right;
		info->pipe_loc = FIRST;
		process_phrase_node(pipe_node->right, info);
		info->pipe_loc = LAST;
		if (pipe_node->parent)
			info->pipe_loc = MIDDLE;
		process_phrase_node(pipe_node->left, info);
		if (pipe_node->parent)
		{
		print_tree(pipe_node->parent, 10);
			process_pipe_segment(pipe_node->parent, info);
		}
		info->is_pipe = false;
		info->is_re_pipe = false;
		info->pipe_loc = -1;
		print_tree(pipe_node, 10);
	}
}
