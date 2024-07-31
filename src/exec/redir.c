/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 12:01:59 by junsan            #+#    #+#             */
/*   Updated: 2024/07/31 20:22:51 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	here_doc_redir(t_info *info)
{
	info->tmp_fd = dup(STDOUT_FILENO);
	if (info->tmp_fd == -1)
		return (fd_log_error("fd error!", NULL, NULL));
	if (dup2(info->origin_stdin_fd, STDIN_FILENO) == -1)
		return (fd_log_error("Dup origin_stdin_fd error!", NULL, NULL));
	if (dup2(info->origin_stdout_fd, STDOUT_FILENO) == -1)
		return (fd_log_error("Dup origin_stdout_fd error!", NULL, NULL));
	info->stdin_fd = open_file_with_mode(HEREDOC_TMP, WRITE);
	here_doc(info->stdin_fd, info->redir_args[0], info);
	if (dup2(info->tmp_fd, STDOUT_FILENO) == -1)
		return (fd_log_error("Dup tmp fd error!", NULL, NULL));
	close(info->tmp_fd);
	return (SUCCESS);
}

static int	input_redir(t_ast *node, t_info *info)
{
	t_ast	*io_node;
	char	**args;
	int		pipe_fd[2];

	io_node = node->left;
	args = info->redir_args;
	if (io_node->type == IN_REDIR)
		info->stdin_fd = open_file_with_mode(args[0], READ);
	else if (io_node->type == IN_HEREDOC)
		here_doc_redir(info);
	else if (io_node->type == IN_HERESTR)
	{
		if (pipe(pipe_fd) == -1)
			return (fd_log_error(NULL, args[0], strerror(errno)));
		if (write(pipe_fd[1], \
			args[0], ft_strlen(args[0])) == -1)
			return (fd_log_error(NULL, args[0], strerror(errno)));
		close(pipe_fd[1]);
		info->stdin_fd = pipe_fd[0];
	}
	if (info->stdin_fd == -1)
		return (fd_log_error("Dup stdin_fd error!", NULL, NULL));
	if (dup2(info->stdin_fd, STDIN_FILENO) == -1)
		return (fd_log_error("Dup stdin_fd error!", NULL, NULL));
	return (SUCCESS);
}

static int	output_redir(t_ast *node, t_info *info)
{
	t_ast	*io_node;
	char	**args;

	io_node = node->left;
	args = info->redir_args;
	if (io_node->type == OUT_REDIR)
	{
		info->stdout_fd = open_file_with_mode(args[0], WRITE);
		if (info->stdout_fd == -1)
			return (fd_log_error(NULL, args[0], strerror(errno)));
		if (dup2(info->stdout_fd, STDOUT_FILENO) == -1)
			return (fd_log_error("Dup stdout_fd error!", NULL, NULL));
	}
	else if (io_node->type == OUT_APPEND)
	{
		info->stdout_fd = open_file_with_mode(args[0], APPEND);
		if (info->stdout_fd == -1)
			return (fd_log_error(NULL, args[0], strerror(errno)));
		if (dup2(info->stdout_fd, STDOUT_FILENO) == -1)
			return (fd_log_error("Dup stdout_fd error!", NULL, NULL));
	}
	return (SUCCESS);
}

static int	handle_ft_redirection(t_ast *node, t_info *info)
{
	t_ast	*io_node;
	char	**args;

	io_node = node->left;
	info->redir_args = ft_split(node->right->data, ARR_SEP);
	process_quotes_in_args(info->redir_args);
	args = info->redir_args;
	if (io_node->type == IN_REDIR || io_node->type == IN_HEREDOC || \
		io_node->type == IN_HERESTR)
	{
		if (io_node->type == IN_REDIR && (access(args[0], F_OK) == -1))
			return (fd_log_error(NULL, args[0], strerror(errno)), 127);
		if (redirect_to_null(info) == FAILURE)
			return (FAILURE);
		if (input_redir(node, info) == FAILURE)
			return (FAILURE);
	}
	else if (io_node->type == OUT_REDIR || io_node->type == OUT_APPEND)
	{
		if (output_redir(node, info) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	handle_io_redirection(t_ast *node, t_info *info)
{
	int		status;

	status = SUCCESS;
	if (info->stdin_fd != -1)
		close(info->stdin_fd);
	if (info->stdout_fd != -1)
		close(info->stdout_fd);
	while (node && status == SUCCESS)
	{
		status = handle_ft_redirection(node, info);
		if (status > SUCCESS)
			info->is_pipe = false;
		if (!node->left->left)
			break ;
		node = node->left->left;
	}
	return (status);
}
