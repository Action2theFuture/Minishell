/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:45:58 by junsan            #+#    #+#             */
/*   Updated: 2024/06/29 19:53:22 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_builtin(int (*func[])(const char *, const char **, t_env *))
{
	func[CD] = ft_cd;
	func[M_ECHO] = ft_echo;
	func[ENV] = ft_env;
	func[EXIT] = ft_exit;
	func[EXPORT] = ft_export;
	func[PWD] = ft_pwd;
	func[UNSET] = ft_unset;
}

int	handler_builtin(const char *cmd)
{
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (CD);
	else if (ft_strncmp(cmd, "echo", 5) == 0)
		return (M_ECHO);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (ENV);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (EXIT);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (EXPORT);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (PWD);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (UNSET);
	else
		return (NONE);
}
