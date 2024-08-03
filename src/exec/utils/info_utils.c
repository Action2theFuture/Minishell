/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 19:56:30 by junsan            #+#    #+#             */
/*   Updated: 2024/08/03 08:49:43 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
