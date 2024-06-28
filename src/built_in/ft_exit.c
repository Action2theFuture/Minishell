/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 12:05:58 by rabouzia          #+#    #+#             */
/*   Updated: 2024/06/28 14:22:02 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(const char *cmd, const char **args, t_env *list)
{
	(void)cmd;
	(void)list;
	if (*args && !ft_isdigit(ft_atoi(args[0])))
	{
		ft_putstr_fd("kashell: exit: ", 1);
		ft_putstr_fd((char *)args[0], 1);
		ft_putstr_fd(": numeric argument required\n", 1);
		exit(1);
	}
	if (*args && sizeof(args) > 1)
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 1);
		exit(1);
		
	}
	else
	{
		ft_putstr_fd((char *)args[0], STDERR_FILENO);
		exit(ft_atoi(args[0]));
	}
}
