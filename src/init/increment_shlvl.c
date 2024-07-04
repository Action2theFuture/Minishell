/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   increment_shlvl.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:10:30 by junsan            #+#    #+#             */
/*   Updated: 2024/07/04 11:08:53 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	increment_shlvl(t_env *env)
{
	char	*new_shlvl;
	int		shlvl;

	while (env)
	{
		if (ft_strncmp(env->name, "SHLVL", 5) == 0)
			break ;
		env = env->next;
	}
	if (env == NULL)
		return (SUCCESS);
	shlvl = ft_atoi(env->content);
	free(env->content);
	new_shlvl = ft_itoa(++shlvl);
	if (new_shlvl == NULL)
		return (FAILURE);
	env->content = new_shlvl;
	return (SUCCESS);
}
