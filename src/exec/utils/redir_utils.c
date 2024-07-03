/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:20:15 by junsan            #+#    #+#             */
/*   Updated: 2024/07/03 19:07:05 by junsan           ###   ########.fr       */
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

int	here_doc(int infile, char *limiter)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			line = NULL;
			break ;
		}
		write(infile, line, ft_strlen(line));
		write(infile, "\n", 1);
		free(line);
		line = NULL;
	}
	close(infile);
	infile = open(HEREDOC_TMP, O_RDONLY, 0644);
	if (infile == -1)
		return (FAILURE);
	return (SUCCESS);
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
