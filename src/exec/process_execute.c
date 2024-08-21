/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 22:12:12 by junsan            #+#    #+#             */
/*   Updated: 2024/08/21 11:26:44 by junsan           ###   ########.fr       */
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

static void	process_io_redirections_and_cmd(\
		t_ast *redir_node, t_ast *cmd_node, t_info *info)
{
	int	status;

	status = SUCCESS;
	if (redir_node && redir_node->type != SUBSHELL)
		process_io_redirections(redir_node, info);
	status = info->status;
	if ((info->pipe_loc == FIRST || info->pipe_loc == MIDDLE) && !cmd_node)
	{
		info->is_re_pipe = true;
		info->pipe_loc = LAST;
	}
	if (cmd_node && status == SUCCESS)
		status = dispatch_cmd(cmd_node, info);
	info->exit_status = status;
	if (redir_node)
	{
		info->is_redirection = false;
		if (cmd_node == NULL && info->pipe_loc == LAST)
			(launch_process_pipe(NULL, NULL, info));
	}
}

/* 
 left is redir, right is args
 why launch_process_pipe is added
 ->  Child process does not terminate without this conditional 
 if there is no command, only redirection

 redirect_input_to_empty : 
 The reason redirect_input_to_empty exists is that a redirect 
 that generates input for a pipeline will not generate output 
 unless it is accompanied by a command, 
 but why empty instead of null? 
 Output commands such as cat will not receive an eof when null comes up, 
 so they will be pending, so I put an empty value to prevent this
*/
void	process_phrase_node(t_ast *node, t_info *info)
{
	t_ast	*redir_node;
	t_ast	*cmd_node;

	if (node == NULL)
		return ;
	redir_node = node->left;
	cmd_node = node->right;
	backup_fds(info);
	process_io_redirections_and_cmd(redir_node, cmd_node, info);
	if (cmd_node)
		restore_fds(info);
	(free_args(info->redir_args), info->redir_args = NULL);
	if (info->is_input_redir && cmd_node == NULL && info->is_pipe)
		(redirect_input_to_empty(), info->is_input_redir = false);
}
