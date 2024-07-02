/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_with_args.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 19:03:26 by junsan            #+#    #+#             */
/*   Updated: 2024/07/01 21:20:14 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_env_vars_in_str(const char *str, t_info *info)
{
	size_t	res_size;
	char	*res;

	res_size = ft_strlen(str) * 2 + 1;
	res = (char *)malloc(sizeof(char) * res_size);
	if (!res)
		return (perror("malloc error"), NULL);
	replace_env_vars(str, res, info);
	free((char *)str);
	return (res);
}

void	replace_env_vars_in_args(char **args, t_info *info)
{
	char	*new_arg;
	int		i;

	i = -1;
	while (args[++i])
	{
		new_arg = replace_env_vars_in_str(args[i], info);
		args[i] = new_arg;
	}
}
