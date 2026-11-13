/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_absolute_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 19:20:36 by junsan            #+#    #+#             */
/*   Updated: 2024/06/23 22:13:04 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_current_directory(char *path, size_t size)
{
	if (getcwd(path, size) == NULL)
	{
		perror("Error getting current current directory");
		return ;
	}
}

static void	join_paths(const char *path_1, const char *path_2, char *result,
		size_t size)
{
	ft_strlcpy(result, path_1, size);
	ft_strlcat(result, "/", size);
	ft_strlcat(result, path_2, size);
}

static void	process_component(const char *cpn, char *r_path, size_t size)
{
	char	*last_slash;

	if (ft_strncmp(cpn, "..", 2) == 0 && cpn[2] == '\0')
	{
		last_slash = ft_strrchr(r_path, '/');
		if (last_slash && last_slash != r_path)
			*last_slash = '\0';
		else if (last_slash == r_path)
			*(last_slash + 1) = '\0';
	}
	else if (ft_strncmp(cpn, ".", 1) != 0 || cpn[1] != '\0')
	{
		if (ft_strlen(r_path) != 1 || r_path[0] != '/')
			ft_strlcat(r_path, "/", size);
		ft_strlcat(r_path, cpn, size);
	}
}

static void	get_resolve_path(const char *path, char *r_path, size_t size)
{
	char	component[MAX_PATH_LENGTH];
	size_t	start;
	size_t	len;
	size_t	pos;

	len = ft_strlen(path);
	pos = 0;
	while (pos < len)
	{
		start = pos;
		while (pos < len && path[pos] != '/')
			pos++;
		if (pos > start)
		{
			ft_strlcpy(component, path + start, pos - start + 1);
			process_component(component, r_path, size);
		}
		while (pos < len && path[pos] == '/')
			pos++;
	}
}

char	*get_absolute_path(const char *path)
{
	char	cur_dir[MAX_PATH_LENGTH];
	char	tmp_path[MAX_PATH_LENGTH];
	char	resolve_path[MAX_PATH_LENGTH];
	char	*absolute_path;

	resolve_path[0] = '\0';
	get_current_directory(cur_dir, sizeof(cur_dir));
	if (path[0] == '/')
		ft_strlcpy(tmp_path, path, sizeof(tmp_path));
	else
		join_paths(cur_dir, path, tmp_path, sizeof(tmp_path));
	get_resolve_path(tmp_path, resolve_path, sizeof(resolve_path));
	absolute_path = (char *)malloc(sizeof(char) * (ft_strlen(resolve_path)
				+ 1));
	if (absolute_path == NULL)
	{
		perror("Error alocating memory");
		return (NULL);
	}
	ft_strlcpy(absolute_path, resolve_path, ft_strlen(resolve_path) + 1);
	return (absolute_path);
}
