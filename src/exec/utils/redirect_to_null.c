/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_to_null.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 10:03:13 by junsan            #+#    #+#             */
/*   Updated: 2024/08/02 10:03:44 by junsan           ###   ########.fr       */
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
	return (SUCCESS);
}
