/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 19:38:46 by junsan            #+#    #+#             */
/*   Updated: 2024/08/17 11:37:37 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 When a subshell within a subshell is encountered, 
 the logical operator || prevents the next logical operator from working, 
 forcing the code to send to next logical operator
		else if (status == 0)
		{
			last_node = *node;
			while (last_node && last_node->type != SUBSHELL)
				last_node = last_node->left;
			if (last_node->data[0] == ')') 
				traverse_tree_in_subshell(&last_node, info);
		}
*/
static void	process_logical_node_in_subshell(t_ast *node, t_info *info)
{
	int		status;

	traverse_tree_in_subshell(node->right, info);
	status = info->exit_status;
	if (ft_strncmp(node->data, "&&", 2) == 0 && status == SUCCESS)
		traverse_tree_in_subshell(node->left, info);
	else if (ft_strncmp(node->data, "||", 2) == 0)
	{
		if (info->exit_status > 0)
			traverse_tree_in_subshell(node->left, info);
	}
}

void	traverse_tree_in_subshell(t_ast *node, t_info *info)
{
	if (node == NULL)
		return ;
	if (node->type == LOGICAL)
		process_logical_node_in_subshell(node, info);
	else if (node->type == PIPE)
		process_pipe_node(node, info);
	else if (node->type != PIPE && node->type != LOGICAL)
	{
		if (node->type == PHRASE && info->status == SUCCESS)
		{
			process_phrase_node(node, info);
			if (!node->left)
				return ;
		}
		else if (node->type == NESTED_SUBSHELL && info->status == SUCCESS)
		{
			start_nested_subshell_node(node, info);
			if (!node->left)
				return ;
		}
		if (node->left)
			traverse_tree_in_subshell(node->left, info);
		if (node->right)
			traverse_tree_in_subshell(node->right, info);
	}
}

int	process_subshell_node(t_ast *node, t_info *info)
{
	pid_t	pid;
	t_ast	*subshell_node;

	subshell_node = node->left;
	pid = fork();
	if (pid == -1)
		return (fd_log_error("fork_error", NULL, NULL));
	if (pid == 0)
	{
		if (backup_stdio(info) == FAILURE)
			fd_log_error(NULL, NULL, strerror(errno));
		set_signal_handler(DFL);
		traverse_tree_in_subshell(subshell_node, info);
		if (restore_stdio(info) == FAILURE)
			fd_log_error(NULL, NULL, strerror(errno));
		cleanup_and_exit(info->exit_status, NULL, NULL, info);
	}
	else
		wait_for_child_task(info);
	cleanup_tmp_file();
	return (info->exit_status);
}
