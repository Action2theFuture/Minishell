/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:29:01 by junsan            #+#    #+#             */
/*   Updated: 2024/07/12 17:32:05 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_dir_entry(\
	const char *base_path, struct dirent *entry, int *cnt, char **matches);

static void	expand_wildcard_recursive(\
			const char *base_path, int *cnt, char **matches)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(base_path);
	if (!dir)
	{
		perror("dir error");
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		process_dir_entry(base_path, entry, cnt, matches);
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		perror("close dir");
}

static void	process_dir_entry(\
	const char *base_path, struct dirent *entry, int *cnt, char **matches)
{
	struct stat	info;
	char		path[MAX_ARGS];

	if (entry->d_name[0] == '.')
		return ;
	ft_strlcpy(path, base_path, MAX_ARGS);
	ft_strlcat(path, "/", MAX_ARGS);
	ft_strlcat(path, entry->d_name, MAX_ARGS);
	if (stat(path, &info) != 0)
	{
		perror("stat error");
		return ;
	}
	if (S_ISDIR(info.st_mode))
		expand_wildcard_recursive(path, cnt, matches);
	else
	{
		if (*cnt < MAX_ARGS)
			matches[(*cnt)++] = strdup(path);
	}
}

static int	count_new_args(char **args, int match_cnt)
{
	int	cnt;
	int	i;

	cnt = 0;
	i = -1;
	while (args[++i])
	{
		if (ft_strlen(args[i]) == 1 && ft_strncmp(args[i], "*", 1) == 0)
			cnt += match_cnt;
		else
			cnt++;
	}
	return (cnt);
}

static void	populate_new_args(\
		char **new_args, char **args, char **matches, int match_cnt)
{
	int	i;
	int	j;
	int	new_idx;

	i = -1;
	new_idx = 0;
	while (args[++i])
	{
		if (ft_strlen(args[i]) == 1 && ft_strncmp(args[i], "*", 1) == 0)
		{
			j = -1;
			while (++j < match_cnt)
				new_args[new_idx++] = matches[j];
		}
		else
			new_args[new_idx++] = ft_strdup(args[i]);
	}
	new_args[new_idx] = NULL;
}

void	expand_wildcard(char ***args)
{
	char	**new_args;
	char	**matches;
	int		match_cnt;
	int		new_arg_cnt;

	match_cnt = 0;
	matches = (char **)malloc(sizeof(char *) * MAX_ARGS);
	if (!matches)
	{
		perror("malloc error");
		return ;
	}
	expand_wildcard_recursive(".", &match_cnt, matches);
	new_arg_cnt = count_new_args(*args, match_cnt);
	new_args = (char **)malloc(sizeof(char *) * (new_arg_cnt + 1));
	if (!new_args)
	{
		perror("malloc error");
		free_args(matches);
		return ;
	}
	(populate_new_args(new_args, *args, matches, match_cnt), \
		free(matches), free_args(*args));
	*args = new_args;
}
