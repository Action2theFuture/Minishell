/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:07:54 by junsan            #+#    #+#             */
/*   Updated: 2024/08/20 21:00:00 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	start_heredoc(char *limiter, t_info *info)
{
	char	*parsed_limiter;
	int		status;

	info->stdin_fd = open_file_with_mode(HEREDOC_TMP, WRITE);
	if (ft_strlen(limiter) > 1 && \
		((limiter[0] == '\'' && limiter[ft_strlen(limiter) - 1] == '\'') || \
		(limiter[0] == '"' && limiter[ft_strlen(limiter) - 1] == '"')))
	{
		parsed_limiter = remove_shell_quotes(limiter);
		status = here_doc(info->stdin_fd, parsed_limiter, info);
		free(parsed_limiter);
	}
	else
	{
		info->is_expansion_heredoc = true;
		status = here_doc(info->stdin_fd, limiter, info);
	}
	info->is_expansion_heredoc = false;
	if (status == FAILURE)
		info->is_heredoc = false;
	if (dup2(info->tmp_fd, STDOUT_FILENO) == -1)
		return (fd_log_error("Dup tmp_fd error!", NULL, NULL));
	(close(info->tmp_fd), info->tmp_fd = -1);
	return (status);
}

int	here_doc_redir(char *limiter, t_info *info)
{
	if (redirect_input_to_null() == FAILURE)
		return (FAILURE);
	info->tmp_fd = dup(STDOUT_FILENO);
	if (info->tmp_fd == -1)
		return (fd_log_error("fd error!", NULL, NULL));
	if (info->stdin_pipe != -1)
		redirect_stdin_to_empty(&info->stdin_pipe);
	if (dup2(info->origin_stdin_fd, STDIN_FILENO) == -1)
		return (fd_log_error("Dup origin_stdin_fd error!", NULL, NULL));
	if (dup2(info->origin_stdout_fd, STDOUT_FILENO) == -1)
		return (fd_log_error("Dup origin_stdout_fd error!", NULL, NULL));
	return (start_heredoc(limiter, info));
}

int	input_redir(char *arg, t_ast *node, t_info *info)
{
	t_ast	*io_node;
	int		pipe_fd[2];
	int		status;

	io_node = node->left;
	status = SUCCESS;
	if (io_node->type == IN_REDIR)
		info->stdin_fd = open_file_with_mode(arg, READ);
	else if (io_node->type == IN_HEREDOC)
		status = here_doc_redir(arg, info);
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
		return (fd_log_error(NULL, arg, strerror(errno)));
	if (dup2(info->stdin_fd, STDIN_FILENO) == -1)
		return (fd_log_error("Dup stdin_fd error!", NULL, NULL));
	return (close(info->stdin_fd), info->stdin_fd = -1, status);
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
