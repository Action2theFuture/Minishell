/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdio_redirector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:08:08 by junsan            #+#    #+#             */
/*   Updated: 2024/06/27 12:19:28 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
