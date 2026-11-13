/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_in_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 22:13:14 by junsan            #+#    #+#             */
/*   Updated: 2024/06/23 22:57:57 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_path_in_env(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->name, "PATH", 4) == 0 && ft_strlen(env->name) == 4)
			return (env->content);
		env = env->next;
	}
	return (NULL);
}

static char	*construct_path(const char *dir, const char *cmd)
{
	char	*path;
	size_t	dir_len;
	size_t	cmd_len;

	path = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
	if (!path)
	{
		perror("malloc error");
		return (NULL);
	}
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	if (dir_len + cmd_len + 2 > MAX_PATH_LENGTH)
	{
		free(path);
		return (NULL);
	}
	ft_memcpy(path, dir, dir_len);
	path[dir_len] = '/';
	ft_memcpy(path + dir_len + 1, cmd, cmd_len + 1);
	return (path);
}

static char	*search_in_path(char *path_env_cpy, const char *cmd)
{
	char	*start;
	char	*path;
	char	*end;

	start = path_env_cpy;
	while (*start)
	{
		end = start;
		while (*end && *end != ':')
			end++;
		if (*end)
			*end = '\0';
		path = construct_path(start, cmd);
		if (path && access(path, F_OK) == 0 && access(path, X_OK) == 0)
			return (path);
		free(path);
		start = end + 1;
	}
	return (NULL);
}

char	*find_cmd_in_path(const char *cmd, t_env *env)
{
	char	*cmd_path;
	char	*path_env_cpy;
	char	*path;

	cmd_path = get_path_in_env(env);
	if (!cmd_path)
		return (NULL);
	path_env_cpy = ft_strdup(cmd_path);
	if (!path_env_cpy)
	{
		perror("strdup error");
		return (NULL);
	}
	path = search_in_path(path_env_cpy, cmd);
	free(path_env_cpy);
	return (path);
}
