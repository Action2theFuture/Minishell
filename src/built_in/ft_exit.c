/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 12:05:58 by junsan            #+#    #+#             */
/*   Updated: 2024/08/03 11:32:58 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_exit_args(const char **args)
{
	int	i;

	i = 0;
	while (args[1][i])
	{
		if (!ft_isdigit(\
			args[1][i]) && args[1][i] != '-' && args[1][i] != '+')
		{
			ft_putstr_fd("minishell$> exit: ", STDERR_FILENO);
			ft_putstr_fd((char *)args[0], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			return (255);
		}
		i++;
	}
	return (0);
}

int	ft_exit(t_info *info)
{
	const char	**args;
	int			num_args;
	int			exit_code;

	args = (const char **)info->args;
	num_args = 0;
	while (args[num_args])
		num_args++;
	if (num_args == 1)
	{
		if (!info->in_subshell)
			ft_putstr_fd("exit\n", 2);
		cleanup_and_exit(EXIT_SUCCESS, info->args, NULL, info);
	}
	if (num_args > 2)
		return (ft_putstr_fd("minishell: exit: too many arguments\n", \
			STDERR_FILENO), FAILURE);
	if (args[1])
	{
		exit_code = validate_exit_args(args);
		if (exit_code != 0)
			(cleanup_and_exit(exit_code, info->args, NULL, info));
		(cleanup_and_exit(ft_atoi(args[0]), info->args, NULL, info));
	}
	return (SUCCESS);
}
