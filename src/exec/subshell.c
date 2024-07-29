/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 19:38:46 by junsan            #+#    #+#             */
/*   Updated: 2024/07/29 21:16:46 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_logical_node_in_subshell(t_ast **node, t_info *info);

static void	handle_special_nodes(t_ast	**node, t_info *info)
{
	t_info	subshell_info;

	if ((*node)->type == LOGICAL)
		process_logical_node_in_subshell(node, info);
	else if ((*node)->type == PIPE)
		process_pipe_node(*node, info);
	else if ((*node)->type == SUBSHELL)
	{
		init_info(&subshell_info, info->env);
		process_logical_node_in_subshell(&(*node)->left, &subshell_info);
		info->exit_status = subshell_info.exit_status;
	}
}

static void	traverse_tree_in_subshell(t_ast **node, t_info *info)
{
	if (*node == NULL)
		return ;
	if ((*node)->type == LOGICAL || (*node)->type == PIPE || \
		(*node)->type == SUBSHELL)
		handle_special_nodes(node, info);
	else if ((*node)->type != PIPE && (*node)->type != LOGICAL)
	{
		if ((*node)->type == PHRASE && info->status == SUCCESS)
		{
			process_phrase_node(*node, info);
			if (!(*node)->left)
				return ;
		}
		if ((*node)->left)
			traverse_tree_in_subshell(&(*node)->left, info);
		if ((*node)->right)
			traverse_tree_in_subshell(&(*node)->right, info);
	}
}

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
static void	process_logical_node_in_subshell(t_ast **node, t_info *info)
{
	//t_ast	*last_node;
	int		status;
	
	traverse_tree_in_subshell(&(*node)->right, info);
	status = info->exit_status;
	if (ft_strncmp((*node)->data, "&&", 2) == 0 && status == SUCCESS)
		traverse_tree_in_subshell(&(*node)->left, info);
	else if (ft_strncmp((*node)->data, "||", 2) == 0)
	{
		if (status > 0)
			traverse_tree_in_subshell(&(*node)->left, info);
		/*
		else if (status == 0)
		{
			last_node = *node;
			while (last_node && last_node->type != SUBSHELL)
				last_node = last_node->left;
			printf("last-----------------------\n");
			print_tree(last_node, 10);
			if (last_node->data && last_node->data[0] == ')' && last_node->left && \
				!(ft_strncmp(last_node->left->data, "||", 2) == 0))
				traverse_tree_in_subshell(&last_node, info);
		}*/
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
		set_signal_handler(DFL);
		traverse_tree_in_subshell(&subshell_node, info);
		exit(info->exit_status);
	}
	else
		wait_for_child_task(info);
	return (info->exit_status);
}
