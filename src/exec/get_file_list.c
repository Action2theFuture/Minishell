/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 19:17:41 by junsan            #+#    #+#             */
/*   Updated: 2024/08/09 10:10:12 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_if_dir(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		perror("stat");
		fd_log_error("Command not found", NULL, (char *)path);
		return (-1);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		fd_log_error((char *)path, NULL, "Is a Directory");
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	get_file_list_size(const char *path)
{
	struct dirent	*entry;
	DIR				*dir;
	int				file_count;
	int				dir_check;

	dir_check = check_if_dir(path);
	if (dir_check != SUCCESS)
		return (dir_check);
	file_count = 0;
	dir = opendir(path);
	if (dir == NULL)
	{
		perror("opendir");
		return (-1);
	}
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, ".", 1) != 0 && \
			ft_strncmp(entry->d_name, "..", 2) != 0)
			file_count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (file_count);
}

void	free_file_list(t_file_list *file_list)
{
	int	i;

	if (file_list == NULL)
		return ;
	i = -1;
	while (++i < (int)file_list->count)
		free(file_list->names[i]);
	free(file_list->names);
	free(file_list);
}

const char	*get_path(const char *full_path)
{
	const char	*file_name;
	char		*path;
	size_t		path_len;

	file_name = ft_strchr(full_path, '/');
	if (file_name == NULL)
		return (ft_strdup("."));
	path_len = file_name - full_path + 1;
	path = (char *)malloc(sizeof(char) * (path_len + 1));
	if (path == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(path, full_path, path_len + 1);
	path[path_len] = '\0';
	return ((const char *)path);
}

t_file_list	*get_file_list(const char *path)
{
	t_file_list		*file_list;
	DIR				*dir;
	int				file_count;

	file_list = (t_file_list *)malloc(sizeof(t_file_list));
	if (file_list == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	file_count = get_file_list_size(path);
	if (file_count == FAILURE)
	{
		free(file_list);
		return ((t_file_list *)(intptr_t)IS_DIRECTORY);
	}
	else if (file_count < 0)
	{
		free(file_list);
		return ((t_file_list *)(intptr_t)CMD_NOT_FOUND);
	}
	dir = get_dir(path, file_count, &file_list);
	return (get_entry_list(file_list, dir));
}
