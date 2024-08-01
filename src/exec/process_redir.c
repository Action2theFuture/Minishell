/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:07:54 by junsan            #+#    #+#             */
/*   Updated: 2024/08/01 10:10:09 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	here_doc_redir(char *arg, t_info *info)
{
	info->tmp_fd = dup(STDOUT_FILENO);
	if (info->tmp_fd == -1)
		return (fd_log_error("fd error!", NULL, NULL));
	if (dup2(info->origin_stdin_fd, STDIN_FILENO) == -1)
		return (fd_log_error("Dup origin_stdin_fd error!", NULL, NULL));
	if (dup2(info->origin_stdout_fd, STDOUT_FILENO) == -1)
		return (fd_log_error("Dup origin_stdout_fd error!", NULL, NULL));
	info->stdin_fd = open_file_with_mode(HEREDOC_TMP, WRITE);
	here_doc(info->stdin_fd, arg, info);
	if (dup2(info->tmp_fd, STDOUT_FILENO) == -1)
		return (fd_log_error("Dup tmp fd error!", NULL, NULL));
	close(info->tmp_fd);
	return (SUCCESS);
}

int	input_redir(char *arg, t_ast *node, t_info *info)
{
	t_ast	*io_node;
	int		pipe_fd[2];

	io_node = node->left;
	if (io_node->type == IN_REDIR)
		info->stdin_fd = open_file_with_mode(arg, READ);
	else if (io_node->type == IN_HEREDOC)
		here_doc_redir(arg, info);
	else if (io_node->type == IN_HERESTR)
	{
		if (pipe(pipe_fd) == -1)
			return (fd_log_error(NULL, arg, strerror(errno)));
		if (write(pipe_fd[1], \
			arg, ft_strlen(arg)) == -1)
			return (fd_log_error(NULL, arg, strerror(errno)));
		close(pipe_fd[1]);
		info->stdin_fd = pipe_fd[0];
	}
	if (info->stdin_fd == -1)
		return (fd_log_error("Dup stdin_fd error!", NULL, NULL));
	if (dup2(info->stdin_fd, STDIN_FILENO) == -1)
		return (fd_log_error("Dup stdin_fd error!", NULL, NULL));
	return (SUCCESS);
}

int	output_redir(char *arg, t_ast *node, t_info *info)
{
	t_ast	*io_node;

	io_node = node->left;
	if (io_node->type == OUT_REDIR)
	{
		info->stdout_fd = open_file_with_mode(arg, WRITE);
		if (info->stdout_fd == -1)
			return (fd_log_error(NULL, arg, strerror(errno)));
		if (dup2(info->stdout_fd, STDOUT_FILENO) == -1)
			return (fd_log_error("Dup stdout_fd error!", NULL, NULL));
	}
	else if (io_node->type == OUT_APPEND)
	{
		info->stdout_fd = open_file_with_mode(arg, APPEND);
		if (info->stdout_fd == -1)
			return (fd_log_error(NULL, arg, strerror(errno)));
		if (dup2(info->stdout_fd, STDOUT_FILENO) == -1)
			return (fd_log_error("Dup stdout_fd error!", NULL, NULL));
	}
	return (SUCCESS);
}

int	handle_ft_redirection(char *arg, t_ast *node, t_info *info)
{
	t_ast	*io_node;

	io_node = node->left;
	if (io_node->type == IN_REDIR || io_node->type == IN_HEREDOC || \
		io_node->type == IN_HERESTR)
	{
		if (io_node->type == IN_REDIR && (access(arg, F_OK) == -1))
			return (fd_log_error(NULL, arg, strerror(errno)), 127);
		if (redirect_to_null(info) == FAILURE)
			return (FAILURE);
		if (input_redir(arg, node, info) == FAILURE)
			return (FAILURE);
	}
	else if (io_node->type == OUT_REDIR || io_node->type == OUT_APPEND)
	{
		if (output_redir(arg, node, info) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}
