/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:59:35 by junsan            #+#    #+#             */
/*   Updated: 2024/07/01 11:35:12 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(const char *cmd, const char **args, t_env *list)
{
	char	*print;

	(void)args;
	(void)cmd;
	(void)list;
	print = getcwd(NULL, 0);
	if (!print)
	{
		perror("kashell: pwd: ");
		return (1);
	}
	if (write(1, print, strlen(print)) == -1)
		return (1);
	if (write(1, "\n", 1) == -1)
		return (0);
	return (0);
}
