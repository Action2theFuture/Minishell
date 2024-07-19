/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 22:12:12 by junsan            #+#    #+#             */
/*   Updated: 2024/07/19 22:17:23 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	categorize_tree(t_ast *node, t_info *info)
{
	t_info	subshell_info;

	if (info->in_subshell == true && node->type == PHRASE && \
		info->status == SUCCESS && (node->left && node->left->type == PHRASE))
		info->in_subshell = false;
	if (node->type == PHRASE && info->status == SUCCESS)
		process_phrase_node(node, info);
	if (node->type == SUBSHELL && ft_strncmp(node->data, "(", 1) == 0 \
			&& info->status == SUCCESS)
	{
		init_info(&subshell_info, info->env);
		subshell_info.in_subshell = true;
		traverse_tree(node->right, &subshell_info);
		info = &subshell_info;
		info->in_subshell = false;
	}
}

static void	process_pipe_segment(t_ast *pipe_node, t_info *info)
{
	if (pipe_node->parent && pipe_node->parent->type == PIPE)
	{
		info->pipe_loc = MIDDLE;
		process_phrase_node(pipe_node->left, info);
		pipe_node = pipe_node->parent;
		while (pipe_node->parent && pipe_node->parent->type == PIPE)
		{
			process_phrase_node(pipe_node->left, info);
			pipe_node = pipe_node->parent;
		}
		info->pipe_loc = LAST;
		process_phrase_node(pipe_node->left, info);
	}
	else
	{
		info->pipe_loc = LAST;
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
		while (pipe_node && pipe_node->right && pipe_node->right->type == PIPE)
			pipe_node = pipe_node->right;
		info->pipe_loc = FIRST;
		process_phrase_node(pipe_node->right, info);
		process_pipe_segment(pipe_node, info);
		info->is_pipe = false;
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
