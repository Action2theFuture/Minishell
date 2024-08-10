/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 10:39:36 by junsan            #+#    #+#             */
/*   Updated: 2024/08/08 10:51:45 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_var_in_env(char *name, t_env *head)
{
	t_env	*cur;

	if (!name || !head)
		return (NULL);
	cur = head;
	while (cur)
	{
		if (ft_strlen(name) == ft_strlen(cur->name) && \
		ft_strncmp(cur->name, name, ft_strlen(name)) == 0)
			return (cur->content);
		cur = cur->next;
	}
	return (NULL);
}
