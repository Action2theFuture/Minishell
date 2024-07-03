/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 21:38:26 by rabouzia          #+#    #+#             */
/*   Updated: 2024/07/03 09:31:12 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to be completed
// echo $?
// echo env expansion $USER
int	good_flag(const char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(const char *cmd, const char **args, t_env *list)
{
	int		i;
	bool	new_line;

	(void)cmd;
	(void)list;
	if (!args || !*args)
		return (write(1, "\n", 1), FAILURE);
	i = 1;
	new_line = 0;
	// if(ft_strncmp(args[0], "$?", 2) == 0)
	// 	return(1);
	// if(args[0][0] == "?")
	while (args[i] && good_flag(args[i]))
	{
		new_line = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd((char *)args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!new_line)
		ft_putstr_fd("\n", 1);
	return (SUCCESS);
}

// void print_echo(t_cmd_list *list)
// {
//     if (ft_echo(list->cmd))
//     {
//         ft_putstr_fd("\n", stdout);
//         ft_putstr_fd(list->cmd->next, stdout);
//     }
//     else
//     {
//         ft_putstr_fd("\n", stdout);
//         ft_putstr_fd(list->cmd->next, stdout);
//         ft_putstr_fd("\n", stdout);
//     }
// }
