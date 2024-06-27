/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:34:10 by junsan            #+#    #+#             */
/*   Updated: 2024/06/27 17:17:07 by junsan           ###   ########.fr       */
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
	if (node->type == PHRASE && info->status == SUCCESS)
		process_phrase_node(node, info);
	else if (node->type == SUBSHELL && ft_strncmp(node->data, "(", 1) == 0 \
			&& info->status == SUCCESS)
	{
		init_info(&subshell_info);
		subshell_info.in_subshell = true;
		traverse_tree(node->right, &subshell_info);
	}
}

static void	traverse_tree(t_ast *node, t_info *info)
{
	t_ast	*cnt_node_for_pipe;
	int		cnt;
	int		status;

	if (node == NULL)
		return ;
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
	if (node->type == LOGICAL)
	{
		traverse_tree(node->right, info);
		status = info->exit_status;
		// printf("status : %d, exit status : %d\n", info->status, info->exit_status);
		if ((ft_strncmp(node->data, "&&", 2) == 0 && status == SUCCESS) || \
			(ft_strncmp(node->data, "||", 2) == 0 && status > 0))
			traverse_tree(node->left, info);
	}
	if (node->type == PIPE)
	{
		traverse_tree(node->right, info);
		if (node->type == PIPE && info->status == SUCCESS)
		{
			//print_tree(node, 10);
			traverse_tree(node->left, info);
		}
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

	init_info(&info);
	if (exit_status != 0)
		info.exit_status = *exit_status;
	info.env = env;
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
