/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_under_score.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 20:41:55 by junsan            #+#    #+#             */
/*   Updated: 2024/07/26 23:07:47 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fetch_last_arg(char **chunk, t_info *info)
{
	t_env	*env;
	int		i;

	if (chunk == NULL || chunk[0] == NULL)
		return ;
	i = 0;
	while (chunk[i])
		i++;
	env = info->env;
	if (!info->env->last_arg->content)
	{
		while (env)
		{
			if ((ft_strncmp(env->name, "_", 1)) == 0
				&& ft_strlen(env->name) == 1)
			{
				info->env->last_arg->content = ft_strdup(env->content);
				return ;
			}
			env = env->next;
		}
	}
	if (info->env->last_arg->content)
		free(info->env->last_arg->content);
	info->env->last_arg->content = ft_strdup(chunk[i - 1]);
}
