/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_to_null.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 10:03:13 by junsan            #+#    #+#             */
/*   Updated: 2024/08/02 17:58:29 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_output_to_null(t_info *info)
{
	info->fd_null = open(DEV_NULL_PATH, O_WRONLY);
	if (info->fd_null == -1)
		return (fd_log_error("fd error!", NULL, NULL));
	if (dup2(info->fd_null, STDOUT_FILENO) == -1)
	{
		close(info->fd_null);
		return (fd_log_error("Dup fd error!", NULL, NULL));
	}
	close(info->fd_null);
	info->fd_null = -1;
	return (SUCCESS);
}

int	redirect_input_to_null(t_info *info)
{
	info->fd_null = open(DEV_NULL_PATH, O_RDONLY);
	if (info->fd_null == -1)
		return (fd_log_error("fd error!", NULL, NULL));
	if (dup2(info->fd_null, STDIN_FILENO) == -1)
	{
		close(info->fd_null);
		return (fd_log_error("Dup fd error!", NULL, NULL));
	}
	close(info->fd_null);
	info->fd_null = -1;
	return (SUCCESS);
}

int	redirect_stdin_to_empty(int stdin)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (fd_log_error("pipe error!", NULL, NULL));
	if (write(pipe_fd[1], "", 1) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (fd_log_error("write error!", NULL, NULL));
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], stdin) == -1)
	{
		close(pipe_fd[0]);
		return (fd_log_error("dup2 error!", NULL, NULL));
	}
	close(pipe_fd[0]);
	return (SUCCESS);
}
