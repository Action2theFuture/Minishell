/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 10:22:38 by junsan            #+#    #+#             */
/*   Updated: 2024/08/02 11:32:43 by junsan           ###   ########.fr       */
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
