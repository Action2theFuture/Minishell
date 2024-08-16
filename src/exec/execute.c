/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:34:10 by junsan            #+#    #+#             */
/*   Updated: 2024/08/17 11:54:02 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	categorize_tree(t_ast *node, t_info *info)
{
	if (info->in_subshell == true && node->type == PHRASE && \
		info->status == SUCCESS && (node->left && node->left->type == PHRASE))
		info->in_subshell = false;
	if (node->type == PHRASE && info->status == SUCCESS)
		process_phrase_node(node, info);
}

void	copy_pipe_info(t_info *dest, t_info *src)
{
	dest->is_pipe = src->is_pipe;
	dest->is_re_pipe = src->is_re_pipe;
	dest->stdin_pipe = src->stdin_pipe;
	dest->has_multiple_pipes = src->has_multiple_pipes;
	dest->pipe_loc = src->pipe_loc;
}

void	prepare_subshell_node(t_ast *node, t_info *info)
{
	t_info	subshell_info;
	t_ast	*cur;

	init_info(&subshell_info, info->env, info->root);
	if (info->is_pipe)
		copy_pipe_info(&subshell_info, info);
	subshell_info.in_subshell = true;
	info->exit_status = process_subshell_node(node, &subshell_info);
	info->in_subshell = false;
	clear_info(&subshell_info);
	cur = node;
	while (cur)
	{
		if (cur->data && cur->data[0] == ')')
		{
			traverse_tree(cur->left, info);
			break ;
		}
		cur = cur->left;
	}
	if (subshell_info.is_pipe)
		copy_pipe_info(info, &subshell_info);
}

void	traverse_tree(t_ast *node, t_info *info)
{
	if (node == NULL)
		return ;
	if (node->type == LOGICAL)
		process_logical_node(node, info);
	else if (node->type == PIPE)
		process_pipe_node(node, info);
	else if (node->type == SUBSHELL && ft_strncmp(node->data, "(", 1) == 0 \
			&& info->status == SUCCESS)
		prepare_subshell_node(node, info);
	else if (node->type != PIPE && node->type != LOGICAL && \
		node->type != SUBSHELL)
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

	init_info(&info, env, root);
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
