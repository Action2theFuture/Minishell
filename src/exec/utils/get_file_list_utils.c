/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_list_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 11:12:29 by junsan            #+#    #+#             */
/*   Updated: 2024/06/22 19:20:25 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_file_list	*get_entry_list(t_file_list *file_list, DIR *dir)
{
	struct dirent	*entry;
	int				index;

	index = -1;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, ".", 1) != 0 && ft_strncmp(entry->d_name,
				"..", 2) != 0)
		{
			file_list->names[++index] = ft_strdup(entry->d_name);
			if (file_list->names[index] == NULL)
			{
				perror("strdup");
				file_list->count = index - 1;
				free_file_list(file_list);
				closedir(dir);
				return (NULL);
			}
		}
		entry = readdir(dir);
	}
	file_list->count = index + 1;
	closedir(dir);
	return (file_list);
}

DIR	*get_dir(const char *path, int file_count, t_file_list **file_list)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir == NULL)
	{
		perror("opendir");
		return (NULL);
	}
	(*file_list)->names = (char **)malloc(sizeof(char *) * (size_t)file_count);
	if ((*file_list)->names == NULL)
	{
		perror("malloc");
		free(*file_list);
		closedir(dir);
		return (NULL);
	}
	(*file_list)->count = (size_t)file_count;
	return (dir);
}
