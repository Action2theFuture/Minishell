/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils_for_expansion.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:36:39 by junsan            #+#    #+#             */
/*   Updated: 2024/07/16 13:41:38 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_env_var(\
	const char *str, char *res, t_env_var *env_var, t_info *info)
{
	env_var->is_value_expansion = false;
	env_var->str = str;
	env_var->res = res;
	env_var->info = info;
	env_var->i = 0;
	env_var->j = 0;
	env_var->in_double_quotes = 0;
}

void	init_handler_info(\
			t_handler_info *h_info, char *new_str, t_info *info)
{
	h_info->new_str = new_str;
	h_info->new_str_len = 0;
	h_info->var_len = 0;
	h_info->i = 0;
	h_info->in_single_quotes = false;
	h_info->in_single_quotes = false;
	h_info->info = info;
}

void	init_visited_paths(t_visited_paths *visited)
{
	visited->capacity = INITIAL_CAPACITY;
	visited->paths = (char **)malloc(sizeof(char *) * visited->capacity);
	visited->count = 0;
}

void	clear_visited_paths(t_visited_paths *visited)
{
	int	i;

	i = -1;
	while (++i < visited->count)
		free(visited->paths[i]);
	free(visited->paths);
}

t_expand_info	*new_expand_info(void)
{
	t_expand_info	*e_info;
	int				capacity;

	capacity = INITIAL_CAPACITY;
	e_info = (t_expand_info *)malloc(sizeof(t_expand_info));
	if (!e_info)
		return (perror("malloc error"), NULL);
	e_info->cnt = 0;
	e_info->capacity = &capacity;
	e_info->matches = ft_calloc(sizeof(char *), capacity);
	if (!e_info->matches)
		return (free(e_info), perror("malloc error"), NULL);
	e_info->next = NULL;
	return (e_info);
}
