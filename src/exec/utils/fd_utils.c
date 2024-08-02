/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:08:08 by junsan            #+#    #+#             */
/*   Updated: 2024/08/02 19:47:01 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	backup_fds(t_info *info)
{
	info->backup_stdin = dup(STDIN_FILENO);
	info->backup_stdout = dup(STDOUT_FILENO);
	if (info->backup_stdin == -1 || info->backup_stdout == -1)
		return (fd_log_error("Backup fd error", NULL, NULL));
	return (SUCCESS);
}

int	restore_fds(t_info *info)
{
	if (info->is_re_pipe && info->backup_stdin != -1 && \
			info->backup_stdout != -1)
	{
		if (dup2(info->backup_stdin, STDIN_FILENO) == -1 || \
			dup2(info->backup_stdout, STDOUT_FILENO) == -1)
			return (fd_log_error("Dup Restore fd error", NULL, NULL));
		if (info->backup_stdin != -1)
			close(info->backup_stdin);
		if (info->backup_stdout != -1)
			close(info->backup_stdout);
		info->backup_stdin = -1;
		info->backup_stdin = -1;
	}
	else if (!info->is_re_pipe && info->backup_stdout != -1)
	{
		if (dup2(info->backup_stdout, STDOUT_FILENO) == -1)
			return (fd_log_error("Dup Restore stdout_fd error!", NULL, NULL));
		if (info->stdout_fd != -1)
			close(info->stdout_fd);
	}
	return (SUCCESS);
}

int	backup_stdio(t_info *info)
{
	info->origin_stdin_fd = dup(STDIN_FILENO);
	if (info->origin_stdin_fd == -1)
		return (fd_log_error("Dup origin_stdin_fd error", NULL, NULL));
	info->origin_stdout_fd = dup(STDOUT_FILENO);
	if (info->origin_stdin_fd == -1)
		return (fd_log_error("Dup origin_stdout_fd error", NULL, NULL));
	return (SUCCESS);
}

int	restore_stdio(t_info *info)
{
	if (dup2(info->origin_stdin_fd, STDIN_FILENO) == -1)
		return (fd_log_error("Dup origin_stdin_fd error", NULL, NULL));
	if (dup2(info->origin_stdout_fd, STDOUT_FILENO) == -1)
		return (fd_log_error("Dup origin_stdout_fd error", NULL, NULL));
	return (SUCCESS);
}

void	close_tmp_pipe(int *tmp_pipe)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		if (tmp_pipe[i] != -1)
		{
			close(tmp_pipe[i]);
			tmp_pipe[i] = -1;
		}
	}
}
