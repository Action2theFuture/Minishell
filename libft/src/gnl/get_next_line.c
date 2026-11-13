/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:27:01 by junsan            #+#    #+#             */
/*   Updated: 2024/05/12 18:36:18 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static char	*process_backup(char **backup)
{
	char	*new_line_ptr;
	char	*line;
	char	*tmp;

	new_line_ptr = gnl_strchr(*backup, ft_strlen(*backup));
	if (!new_line_ptr)
	{
		line = ft_strdup(*backup);
		free(*backup);
		*backup = NULL;
		return (line);
	}
	line = ft_substr(*backup, 0, new_line_ptr - *backup + 1);
	if ((*backup)[0] != '\0')
	{
		tmp = ft_strdup(new_line_ptr + 1);
		free(*backup);
		*backup = NULL;
		*backup = tmp;
	}
	return (line);
}

static ssize_t	gnl_read(int fd, char *buf, char **obj_free)
{
	ssize_t	rd;

	if (!buf)
		return (0);
	rd = read(fd, buf, BUFFER_SIZE);
	if (rd > 0)
		buf[rd] = '\0';
	else
	{
		free(buf);
		buf = NULL;
		if (rd == -1)
		{
			if (!*obj_free)
				return (rd);
			else if (*obj_free || *obj_free[0] == '\0')
			{
				free(*obj_free);
				*obj_free = NULL;
			}
		}
	}
	return (rd);
}

static char	*strslice(char *line, char **backup, char *buf, size_t len)
{
	char	*front_line;
	char	*ptr;

	ptr = gnl_strchr(line, len);
	if (!ptr)
		return (NULL);
	front_line = ft_substr(line, 0, ptr - line + 1);
	if (*(ptr + 1) != '\0')
		*backup = ft_strdup(ptr + 1);
	free(line);
	line = NULL;
	free(buf);
	buf = NULL;
	return (front_line);
}

static char	*process_line(ssize_t rd, int fd, char *buf, char **backup)
{
	char	*line;
	size_t	len;

	line = NULL;
	while (1)
	{
		if (*backup)
		{
			line = ft_strdup(*backup);
			free(*backup);
			*backup = NULL;
		}
		len = ft_strlen(line) + (size_t)rd;
		line = gnl_strjoin(line, buf, len, (size_t)rd);
		if (!line)
			return (NULL);
		if (gnl_strchr(line, len))
			return (strslice(line, backup, buf, len));
		rd = gnl_read(fd, buf, &line);
		if (rd == -1)
			return (NULL);
		if (!rd)
			break ;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*backup[FD_MAX];
	char		*line;
	char		*buf;
	ssize_t		rd;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	rd = gnl_read(fd, buf, &(backup[fd]));
	if (rd <= 0)
	{
		if (!backup[fd])
			return (NULL);
		else if (backup[fd][0] == '\0')
		{
			free(backup[fd]);
			backup[fd] = NULL;
			return (NULL);
		}
		line = process_backup(&(backup[fd]));
	}
	else
		line = process_line(rd, fd, buf, &(backup[fd]));
	return (line);
}
