/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:20:15 by junsan            #+#    #+#             */
/*   Updated: 2024/07/12 16:40:27 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_tmp_file(void)
{
	int	tmp_file;

	tmp_file = open(HEREDOC_TMP, O_RDONLY);
	if (tmp_file >= 0)
	{
		close(tmp_file);
		if (unlink(HEREDOC_TMP) == -1)
			fd_log_error(NULL, NULL, strerror(errno));
	}
}

int	open_file_with_mode(char *file, int mode)
{
	int	fd;

	if (mode == READ)
		fd = open(file, O_RDONLY, 0644);
	else if (mode == WRITE)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	return (fd);
}
