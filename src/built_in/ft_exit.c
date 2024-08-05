/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 12:05:58 by junsan            #+#    #+#             */
/*   Updated: 2024/08/05 17:48:21 by junsan           ###   ########.fr       */
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

static void	handle_exit_code(const char **args, t_info *info)
{
	int	exit_code;

	exit_code = validate_exit_args(args);
	if (exit_code != 0)
		cleanup_and_exit(exit_code, info->args, NULL, info);
	cleanup_and_exit(ft_atoi(args[1]), info->args, NULL, info);
}

int	ft_exit(t_info *info)
{
	const char	**args;
	int			num_args;

	args = (const char **)info->args;
	num_args = 0;
	while (args[num_args])
		num_args++;
	if (num_args == 1 || num_args == 2)
	{
		if (!info->in_subshell)
			ft_putstr_fd("exit\n", 2);
		if (num_args == 2)
			handle_exit_code(args, info);
		cleanup_and_exit(EXIT_SUCCESS, info->args, NULL, info);
	}
	else if (num_args > 2)
		return (ft_putstr_fd("minishell: exit: too many arguments\n", \
			STDERR_FILENO), FAILURE);
	return (SUCCESS);
}
