/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:29:01 by junsan            #+#    #+#             */
/*   Updated: 2024/07/15 20:38:03 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_wildcard_recursive(\
	const char *base_path, t_expand_info *e_info, int depth);

static bool	process_dir_entry(\
	const char *base_path, struct dirent *entry, \
	t_expand_info *e_info, int depth)
{
	struct stat	info;
	char		path[MAX_ARGS];

	if (entry->d_name[0] == BASE_PATH)
		return (false);
	ft_strlcpy(path, base_path, MAX_ARGS);
	ft_strlcat(path, "/", MAX_ARGS);
	ft_strlcat(path, entry->d_name, MAX_ARGS);
	if (stat(path, &info) != 0)
		return (perror("stat error"), false);
	if (S_ISDIR(info.st_mode))
	{
		if (!is_path_visited(path, e_info->visited))
			(mark_path_visited(path, e_info->visited), \
			expand_wildcard_recursive(path, e_info, depth));
	}
	else
	{
		if (e_info->cnt >= *e_info->capacity)
			e_info->matches = reallocate_matches(\
					e_info->matches, e_info->capacity, e_info->cnt + 1);
		e_info->matches[e_info->cnt++] = ft_strdup(path);
	}
	return (true);
}

static void	expand_wildcard_recursive(\
	const char *base_path, t_expand_info *e_info, int depth)
{
	DIR				*dir;
	struct dirent	*entry;

	if (depth > MAX_RECURSION_DEPTH)
		return ;
	dir = opendir(base_path);
	if (!dir)
	{
		perror("dir error");
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		process_dir_entry(base_path, entry, e_info, depth + 1);
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		perror("close dir");
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
				new_args[new_idx++] = ft_strdup(matches[j]);
		}
		else
			new_args[new_idx++] = ft_strdup(args[i]);
	}
	new_args[new_idx] = NULL;
}

bool	expand_wildcard(char ***args)
{
	t_visited_paths	visited;
	t_expand_info	e_info;
	char			**new_args;
	int				new_arg_cnt;
	int				capacity;

	capacity = INITIAL_CAPACITY;
	init_visited_paths(&visited);
	e_info.matches = ft_calloc(sizeof(char *), capacity);
	if (!e_info.matches)
		return (perror("malloc error"), false);
	e_info.cnt = 0;
	e_info.capacity = &capacity;
	e_info.visited = &visited;
	expand_wildcard_recursive(".", &e_info, 0);
	new_arg_cnt = count_new_args(*args, e_info.cnt);
	new_args = (char **)malloc(sizeof(char *) * (new_arg_cnt + 1));
	if (!new_args)
		return ((perror("malloc error"), free_args(e_info.matches)), false);
	(populate_new_args(new_args, *args, e_info.matches, e_info.cnt), \
	free_visited_paths(&visited), free_args(e_info.matches));
	return (free_args(*args), *args = new_args, true);
}
