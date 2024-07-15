/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 19:56:30 by junsan            #+#    #+#             */
/*   Updated: 2024/07/12 16:43:32 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_info(t_info *info)
{
	info->status = SUCCESS;
	info->exit_status = SUCCESS;
	info->pipe_exists = false;
	info->stdin_fd = -1;
	info->stdout_fd = -1;
	info->tmp_fd = -1;
	info->prev_pipe[0] = -1;
	info->prev_pipe[1] = -1;
	info->pipe_cnt = -1;
	info->in_subshell = false;
	info->path = NULL;
	info->lst_pid = NULL;
}

void	clear_info(t_info *info)
{
	if (info->stdin_fd != -1)
		close(info->stdin_fd);
	if (info->stdout_fd != -1)
		close(info->stdout_fd);
	if (info->tmp_fd != -1)
		close(info->tmp_fd);
	if (info->pipe[0] != -1)
		close(info->pipe[0]);
	if (info->pipe[1] != -1)
		close(info->pipe[1]);
	if (info->prev_pipe[0] != -1)
		close(info->prev_pipe[0]);
	if (info->prev_pipe[1] != -1)
		close(info->prev_pipe[1]);
	close(info->origin_stdin_fd);
	close(info->origin_stdout_fd);
}
