/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 19:56:30 by junsan            #+#    #+#             */
/*   Updated: 2024/07/31 13:34:27 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_info(t_info *info, t_env *env, t_ast *root)
{
	info->status = SUCCESS;
	info->exit_status = SUCCESS;
	info->is_pipe = false;
	info->is_re_pipe = false;
	info->stdin_fd = -1;
	info->stdout_fd = -1;
	info->origin_stdin_fd = -1;
	info->origin_stdout_fd = -1;
	info->backup_stdin = -1;
	info->backup_stdout = -1;
	info->tmp_fd = -1;
	info->fd_null = -1;
	info->in_subshell = false;
	info->stdin_backup = dup(STDIN_FILENO);
	if (info->stdin_backup == -1)
		perror("dup stdin_backup error");
	info->stdout_backup = dup(STDOUT_FILENO);
	if (info->stdout_backup == -1)
		perror("dup stdout_backup error");
	info->path = NULL;
	info->env = env;
	info->root = root;
	info->quote_info = NULL;
}

void	reset_consts_fd(t_info *info)
{
	if (dup2(info->stdin_backup, STDIN_FILENO) == -1)
		perror("dup2 stdin_backup error");
	close(info->stdin_backup);
	if (dup2(info->stdout_backup, STDOUT_FILENO) == -1)
		perror("dup2 stdout_backup error");
	close(info->stdout_backup);
}

void	clear_info(t_info *info)
{
	if (info->fd_null != -1)
		close(info->fd_null);
	if (info->stdin_fd != -1)
		close(info->stdin_fd);
	if (info->stdout_fd != -1)
		close(info->stdout_fd);
	if (info->tmp_fd != -1)
		close(info->tmp_fd);
	if (info->origin_stdin_fd != -1)
		close(info->origin_stdin_fd);
	if (info->origin_stdout_fd != -1)
		close(info->origin_stdout_fd);
}
