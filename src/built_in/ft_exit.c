/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 12:05:58 by rabouzia          #+#    #+#             */
/*   Updated: 2024/07/01 00:57:28 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void  exit_msg(void)
{
	ft_putstr_fd("exit\n", 2);
	exit(0);
}


int	ft_exit(const char *cmd, const char **args, t_env *list)
{
	int i = 0;
	int num_args = 0;

	(void)cmd;
	(void)list;
	while (args[num_args])
		num_args++;
	if (num_args == 0)
		exit_msg();
	if (num_args > 2)
	{
		ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
		return (1); // Return 1 to indicate error but not exit the shell
	}
	if (args[1])
	{
		while (args[0][i])
		{
			if (!ft_isdigit(args[0][i]) && args[0][i] != '-' && args[0][i] != '+')
			{
				ft_putstr_fd("kashell: exit: ", STDERR_FILENO);
				ft_putstr_fd((char *)args[0], STDERR_FILENO);
				ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
				exit(255); // Exit with status 255
			}
			i++;
		}
		exit(ft_atoi(args[0]));
	}
	exit(0);
}
