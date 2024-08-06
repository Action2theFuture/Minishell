/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:20:15 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 11:45:16 by junsan           ###   ########.fr       */
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
	if (fd == -1)
		return (fd_log_error(NULL, file, strerror(errno)), -1);
	return (fd);
}

void	close_fds(t_info *info)
{
	if (info->stdin_fd != -1)
	{
		close(info->stdin_fd);
		info->stdin_fd = -1;
	}
	if (info->stdout_fd != -1)
	{
		close(info->stdout_fd);
		info->stdout_fd = -1;
	}
}

void	process_quotes_in_arg(char *str)
{
	size_t	len;

	while (1)
	{
		len = ft_strlen(str);
		if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
			remove_surrounding_double_quotes(str);
		else if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
			remove_surrounding_single_quotes(str);
		else if (len >= 4 && str[0] == '\'' && str[len - 1] == '\'' && \
			str[1] == '"' && str[len - 2] == '"')
			remove_double_quotes_inside_single_quotes(str);
		else
			break ;
	}
}
