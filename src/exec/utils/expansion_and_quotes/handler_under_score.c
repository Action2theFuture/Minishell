/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_under_score.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 20:41:55 by junsan            #+#    #+#             */
/*   Updated: 2024/07/26 22:31:02 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fetch_last_arg(char **chunk, t_info *info)
{
	t_env	*env;
	char	*last_arg;
	int		i;

	if (chunk == NULL || chunk[0] == NULL)
		return ;
	i = 0;
	while (chunk[i])
		i++;
	env = info->env;
	last_arg = info->env->last_arg->content;
	if (!last_arg)
	{
		while (env)
		{
			if ((ft_strncmp(env->name, "_", 1)) == 0
				&& ft_strlen(env->name) == 1)
			{
				last_arg = ft_strdup(env->content);
				return ;
			}
			env = env->next;
		}
	}
	free(info->env->last_arg->content);
	info->env->last_arg->content = ft_strdup(chunk[i - 1]);
}
