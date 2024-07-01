/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 02:39:36 by rabouzia          #+#    #+#             */
/*   Updated: 2024/06/29 20:54:34 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to be completed
// cd -
void	lst_swap(t_env *a)
{
	t_env *tmp;

	tmp = a->pwd;
	a->pwd = a->old_pwd;
	a->old_pwd = tmp;
}


int	ft_cd(const char *cmd, const char **args, t_env *list)
{
	(void)cmd;
	// printf("cmd in cd: %s\n", cmd);
	// cd src 23d2 wd -> too many args
	// create fct search in env to do cd (alone)
	// cd -
	if (!args[1])
		return (change_dir("/home", list), 1);
	if (!args[2])
	{
		
	}
	if (chdir(args[1]) == -1)
	{
		ft_putstr_fd("cd: ",2);
		ft_putstr_fd((char *)args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	else
		change_dir(args[1], list);
	return (0);
}

// void	modify_pwd_in_env(t_env *env, char *str)
// {
// 	char	*new_pwd;
// 	char	*last_slash;

// 	if (str && *str != '\0')
// 	{
// 		new_pwd = ft_strjoin(env->pwd, str);
// 		free(env->pwd);
// 		env->pwd = new_pwd;
// 	}
// 	else
// 	{
// 		last_slash = strrchr(env->pwd, '/');
// 		if (last_slash)
// 			*last_slash = '\0';
// 	}
// }
