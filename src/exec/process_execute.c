/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 22:12:12 by junsan            #+#    #+#             */
/*   Updated: 2024/08/19 11:17:31 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*
The behavior of converting to last on redirection causes an error 
in the pipeline with input to the wrong pipe input/output
	if ((info->pipe_loc == FIRST && info->has_multiple_pipes) || \
		info->pipe_loc == MIDDLE)
	{
		info->is_re_pipe = true;
		info->pipe_loc = LAST;
	}
I need to do more testing on this
*/
static void	process_io_redirections(t_ast *redir_node, t_info *info)
{
	info->status = handle_io_redirection(redir_node->left, info);
	if (redir_node->right)
		free_args(info->redir_args);
	if (redir_node->right && info->exit_status == SUCCESS)
		info->exit_status = handle_io_redirection(redir_node->right, info);
	info->is_redirection = true;
}

// left is redir, right is args
void	process_phrase_node(t_ast *node, t_info *info)
{
	t_ast	*redir_node;
	t_ast	*cmd_node;
	int		status;

	if (node == NULL)
		return ;
	redir_node = node->left;
	cmd_node = node->right;
	status = SUCCESS;
	backup_fds(info);
	if (redir_node && redir_node->type != SUBSHELL)
		process_io_redirections(redir_node, info);
	status = info->status;
	if (cmd_node && status == SUCCESS)
		status = dispatch_cmd(cmd_node, info);
	info->exit_status = status;
	if (redir_node)
		(redirect_output_to_null(), redirect_input_to_null(), \
		info->is_redirection = false);
	(free_args(info->redir_args), info->redir_args = NULL, restore_fds(info));
	if (info->is_heredoc)
		(redirect_input_to_empty(), info->is_heredoc = false);
}
