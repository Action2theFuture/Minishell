/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:34:10 by junsan            #+#    #+#             */
/*   Updated: 2024/07/19 22:15:20 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	traverse_tree(t_ast *node, t_info *info)
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
