/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:19:00 by junsan            #+#    #+#             */
/*   Updated: 2024/08/04 13:35:30 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_name(const char *name)
{
	int	i;

	i = 0;
	if (!name || (!ft_isalpha((unsigned char)name[i]) && name[i] != '_'))
		return (false);
	while (name[i])
	{
		if (!ft_isalnum((unsigned char)name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

bool	var_if_exist(const char *name, t_env *lst)
{
	while (lst)
	{
		if (ft_strncmp(name, lst->name, ft_strlen(name)) == 0)
			return (true);
		lst = lst->next;
	}
	return (false);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

bool	strip_trailing_plus(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len > 0 && str[len - 1] == '+')
		return (str[len - 1] = '\0', true);
	return (false);
}

