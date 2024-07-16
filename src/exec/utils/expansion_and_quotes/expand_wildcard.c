/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:29:01 by junsan            #+#    #+#             */
/*   Updated: 2024/07/16 15:18:06 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	process_dir_entry(\
		const char *pattern, struct dirent *entry, t_expand_info *e_info)
{
	struct stat	info;
	char		path[MAX_ARGS];
	char		**new_matches;

	new_matches = NULL;
	if (entry->d_name[0] == BASE_PATH)
		return (true);
	ft_strlcpy(path, entry->d_name, MAX_ARGS);
	if (stat(path, &info) != 0)
		return (perror("stat error"), false);
	if (match_pattern(pattern, path))
	{
		if (e_info->cnt >= e_info->capacity)
		{
			new_matches = reallocate_matches(\
					e_info->matches, &e_info->capacity, e_info->cnt + 1);
			if (!new_matches)
				return (perror("reallocte error"), false);
			e_info->matches = new_matches;
		}
		e_info->matches[e_info->cnt++] = ft_strdup(path);
	}
	return (true);
}

static void	expand_wildcard_in_cur_dir(\
					const char *pattern, t_expand_info *e_info)
{
	DIR				*dir;
	struct dirent	*entry;
	const char		*base_path;

	base_path = ".";
	dir = opendir(base_path);
	if (!dir)
	{
		perror("dir error");
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (!process_dir_entry(pattern, entry, e_info))
			break ;
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		perror("close dir");
}

static int	count_new_args(char **args, t_expand_info *e_info)
{
	int	cnt;
	int	i;

	cnt = 0;
	i = -1;
	while (args[++i])
	{
		if (i == 0)
		{
			cnt++;
			continue ;
		}
		if (ft_strchr(args[i], '*') && \
			!(ft_strchr(args[i], '\"') || ft_strchr(args[i], '\'')))
		{
			expand_wildcard_in_cur_dir(args[i], e_info);
			cnt += e_info->cnt;
			e_info->next = new_expand_info();
			e_info = e_info->next;
		}
		else
			cnt++;
	}
	return (cnt);
}

static void	populate_new_args(\
		char **new_args, char **args, t_expand_info *e_info)
{
	int			i;
	int			j;
	int			new_idx;

	i = -1;
	new_idx = 0;
	while (args[++i])
	{
		if (i == 0)
		{
			new_args[new_idx++] = ft_strdup(args[i]);
			continue ;
		}
		if (ft_strchr(args[i], '*') && \
			!(ft_strchr(args[i], '\"') || ft_strchr(args[i], '\'')))
		{
			j = -1;
			while (++j < e_info->cnt)
				new_args[new_idx++] = ft_strdup(e_info->matches[j]);
			e_info = e_info->next;
		}
		else
			new_args[new_idx++] = ft_strdup(args[i]);
	}
	new_args[new_idx] = NULL;
}

bool	expand_wildcard(char ***args)
{
	t_visited_paths	visited;
	t_expand_info	*e_info;
	char			**new_args;
	int				new_arg_cnt;

	init_visited_paths(&visited);
	e_info = NULL;
	e_info = new_expand_info();
	new_arg_cnt = count_new_args(*args, e_info);
	new_args = (char **)malloc(sizeof(char *) * (new_arg_cnt + 1));
	if (!new_args)
		return ((perror("malloc error"), clear_expand_info(e_info)), false);
	(populate_new_args(new_args, *args, e_info), \
	clear_visited_paths(&visited), clear_expand_info(e_info));
	return (free_args(*args), *args = new_args, true);
}
