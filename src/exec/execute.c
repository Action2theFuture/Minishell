/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:34:10 by junsan            #+#    #+#             */
/*   Updated: 2024/07/18 15:51:12 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	traverse_tree(t_ast *node, t_info *info);

static void	categorize_tree(t_ast *node, t_info *info)
{
	t_info	subshell_info;

	if (info->pipe_cnt > 0)
	{
		if (pipe(info->pipe) == -1)
			fd_log_error("pipe error", NULL, NULL);
		info->pipe_exists = true;
	}
	else
		info->pipe_exists = false;
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

static void	cnt_pipe(t_ast *node, t_info *info)
{
	t_ast	*cnt_node_for_pipe;
	int		cnt;

	if (node->type == PIPE && info->pipe_cnt == -1)
	{
		cnt_node_for_pipe = node;
		cnt = 1;
		while (cnt_node_for_pipe->right->type == PIPE)
		{
			cnt++;
			cnt_node_for_pipe = cnt_node_for_pipe->right;
		}
		info->pipe_cnt = cnt;
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
	{
		cnt_pipe(node, info);
		traverse_tree(node->right, info);
		if (node->type == PIPE && info->status == SUCCESS)
			traverse_tree(node->left, info);
	}
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
