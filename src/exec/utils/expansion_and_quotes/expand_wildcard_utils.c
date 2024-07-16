/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 20:42:07 by junsan            #+#    #+#             */
/*   Updated: 2024/07/16 15:05:17 by junsan           ###   ########.fr       */
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
		return (perror("malloc error"), NULL);
	i = -1;
	while (++i < *capacity)
	{
		if (matches[i])
			new_matches[i] = matches[i];
	}
	i = *capacity;
	while (++i < new_capacity)
		new_matches[i] = NULL;
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

bool	match_pattern(const char *pattern, const char *str)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			pattern++;
			if (*pattern == '\0')
				return (true);
			while (*str)
			{
				if (match_pattern(pattern, str))
					return (true);
				str++;
			}
			return (false);
		}
		else if (*pattern == *str)
		{
			pattern++;
			str++;
		}
		else
			return (false);
	}
	return (*str == '\0');
}
