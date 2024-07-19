/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:34:10 by junsan            #+#    #+#             */
/*   Updated: 2024/07/19 15:19:55 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	traverse_tree(t_ast *node, t_info *info);

static void	categorize_tree(t_ast *node, t_info *info)
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

// The bottom left pipe node is the StartNode
static void	process_pipe_node(t_ast *pipe_node, t_info *info)
{
	if (info->pipe_exists == false)
	{
		info->stdin_pipe = -1;
		info->pipe_exists = true;
		if (pipe(info->pipe) == -1)
			fd_log_error("pipe error", NULL, NULL);
		while (pipe_node && pipe_node->right && pipe_node->right->type == PIPE)
			pipe_node = pipe_node->right;
		info->pipe_loc = FIRST;
		process_phrase_node(pipe_node->right, info);
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
		info->pipe_exists = false;
		info->pipe_loc = -1;
	}
}

// printf("status : %d, exit status : %d\n", info->status, info->exit_status);
static void	process_logical_node(t_ast *node, t_info *info)
{
	int	status;

	traverse_tree(node->right, info);
	status = info->exit_status;
	if ((ft_strncmp(node->data, "&&", 2) == 0 && status == SUCCESS) || \
		(ft_strncmp(node->data, "||", 2) == 0 && status > 0))
		traverse_tree(node->left, info);
}

static void	traverse_tree(t_ast *node, t_info *info)
{
	if (node == NULL)
		return ;
	if (node->type == LOGICAL)
		process_logical_node(node, info);
	if (node->type == PIPE)
		process_pipe_node(node, info);
	if (node->type != PIPE && node->type != LOGICAL)
	{
		categorize_tree(node, info);
		if (node->left)
			traverse_tree(node->left, info);
		if (node->right && info->in_subshell == false)
			traverse_tree(node->right, info);
	}
}

void	execute(t_ast *root, t_env *env, int *exit_status)
{
	t_info	info;

	init_info(&info, env);
	if (exit_status != 0)
		info.exit_status = *exit_status;
	if (backup_stdio(&info) == FAILURE)
		fd_log_error(NULL, NULL, strerror(errno));
	traverse_tree(root, &info);
	if (restore_stdio(&info) == FAILURE)
		fd_log_error(NULL, NULL, strerror(errno));
	if (info.status != SUCCESS)
		*exit_status = info.status;
	else
		*exit_status = info.exit_status;
	cleanup_tmp_file();
	clear_info(&info);
}
