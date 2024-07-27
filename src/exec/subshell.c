/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 19:38:46 by junsan            #+#    #+#             */
/*   Updated: 2024/07/27 21:39:36 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_logical_node_in_subshell(t_ast **node, t_info *info);

static void	traverse_tree_in_subshell(t_ast **node, t_info *info)
{
	if (*node == NULL)
		return ;
	if ((*node)->type == LOGICAL)
		process_logical_node_in_subshell(node, info);
	if ((*node)->type == PIPE)
		process_pipe_node(*node, info);
	if ((*node)->type != PIPE && (*node)->type != LOGICAL)
	{
		if ((*node)->type == PHRASE && info->status == SUCCESS && \
			((*node)->left && (*node)->left->type == PHRASE))
			info->in_subshell = false;
		if ((*node)->type == PHRASE && info->status == SUCCESS)
			process_phrase_node(*node, info);
		if ((*node)->left)
			traverse_tree_in_subshell(&(*node)->left, info);
		if ((*node)->right && info->in_subshell == false)
			traverse_tree_in_subshell(&(*node)->right, info);
	}
}

static void	process_logical_node_in_subshell(t_ast **node, t_info *info)
{
	int	status;

	traverse_tree_in_subshell(&(*node)->right, info);
	status = info->exit_status;
	if ((ft_strncmp((*node)->data, "&&", 2) == 0 && status == SUCCESS) || \
		(ft_strncmp((*node)->data, "||", 2) == 0 && status > 0))
		traverse_tree_in_subshell(&(*node)->left, info);
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
		set_signal_handler(DFL);
		traverse_tree_in_subshell(&subshell_node, info);
		exit(info->exit_status);
	}
	else
		wait_for_child_task(info);
	return (info->exit_status);
}
