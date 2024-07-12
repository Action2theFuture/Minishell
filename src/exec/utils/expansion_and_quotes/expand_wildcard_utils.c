/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 20:42:07 by junsan            #+#    #+#             */
/*   Updated: 2024/07/12 20:46:44 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**reallocate_matches(\
	char **matches, int *capacity, int required_capacity)
{
	int		new_capacity;
	int		i;
	char	**new_matches;

	new_capacity = *capacity;
	while (new_capacity < required_capacity)
		new_capacity *= 2;
	new_matches = (char **)malloc(sizeof(char *) * new_capacity);
	if (!new_matches)
		return (perror("malloc error"), free(matches), NULL);
	i = -1;
	while (++i < *capacity)
		new_matches[i] = matches[i];
	free(matches);
	*capacity = new_capacity;
	return (new_matches);
}

bool	is_path_visited(const char *path, t_visited_paths *visited)
{
	int	i;

	i = -1;
	if (visited->count == 0)
		return (false);
	while (++i < visited->count)
	{
		if (ft_strncmp(visited->paths[i], path, ft_strlen(path)) == 0)
			return (true);
	}
	return (false);
}

void	mark_path_visited(const char *path, t_visited_paths *visited)
{
	int	cnt;

	cnt = visited->count + 1;
	if (visited->count >= visited->capacity)
		visited->paths = reallocate_matches(\
		visited->paths, &visited->capacity, cnt);
	visited->paths[visited->count] = ft_strdup(path);
	visited->count++;
}

int	count_new_args(char **args, int match_cnt)
{
	int	cnt;
	int	i;

	cnt = 0;
	i = 0;
	while (args[++i])
	{
		if (ft_strlen(args[i]) == 1 && ft_strncmp(args[i], "*", 1) == 0)
			cnt += match_cnt;
		else
			cnt++;
	}
	return (cnt + 1);
}
