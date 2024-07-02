/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_with_args.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 19:03:26 by junsan            #+#    #+#             */
/*   Updated: 2024/07/02 11:13:31 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_env_vars_in_str(const char *str, t_info *info)
{
	char	*res;

	res = (char *)malloc(sizeof(char) * MAX_ARGS);
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
