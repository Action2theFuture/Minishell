/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 19:37:41 by junsan            #+#    #+#             */
/*   Updated: 2024/07/02 09:59:19 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_path_type	check_absolute_or_relative(\
				const char *path, struct stat path_stat)
{
	if (S_ISDIR(path_stat.st_mode) || S_ISREG(path_stat.st_mode))
	{
		if (path[0] == '/')
			return (PATH_ABSOLUTE);
	}
	return (PATH_RELATIVE);
}

static char	*get_path_in_env(t_env *env)
{
	while (env)
	{
		if (ft_strlen(env->name) == 4 && ft_strncmp(env->name, "PATH", 4) == 0)
			return (env->content);
		env = env->next;
	}
	return (NULL);
}

static t_path_type	check_command(const char *path, t_info *info)
{
	char	*cmd_path;
	char	*end;
	char	cmd[MAX_PATH_LENGTH];

	cmd_path = get_path_in_env(info->env);
	if (cmd_path)
	{
		while (*cmd_path)
		{
			end = ft_strchr(cmd_path, ':');
			if (!end)
				end = cmd_path + ft_strlen(cmd_path);
			ft_strlcpy(cmd, cmd_path, end - cmd_path + 1);
			ft_strlcat(cmd, "/", sizeof(cmd));
			ft_strlcat(cmd, path, sizeof(cmd));
			if (access(cmd, X_OK) == 0)
				return (PATH_COMMAND);
			if (*end == '\0')
				break ;
			cmd_path = end + 1;
		}
	}
	return (PATH_INVALID);
}

t_path_type	get_path_type(const char *path, t_info *info)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
		return (check_absolute_or_relative(path, path_stat));
	return (check_command(path, info));
}
