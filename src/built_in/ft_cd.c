/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 02:39:36 by rabouzia          #+#    #+#             */
/*   Updated: 2024/06/19 19:14:37 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to be completed

int	ft_listsize(t_cmd_list *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	lst_swap(t_cmd_list *a)
{
	t_cmd_list *tmp;

	tmp = a->env->pwd;
	a->env->pwd = a->env->old_pwd;
	a->env->old_pwd = tmp;
}


int	ft_cd(char **cmd, t_cmd_list *list)
{
	if (!cmd[1])
		return (change_dir("HOME", list), 1);
	if (cmd[2])
	{
		ft_putstr_fd("cd: too many arguments", 2);
		return 1;
	}
	if (ft_strncmp(cmd[1], "-", 1))
	{
		change_dir(list->env->old_pwd->content, list);
		lst_swap(list);
	}
	if (chdir(cmd[1]) == -1)
	{
		ft_putstr_fd("cd: no such file or directory: ", 2);
		ft_putstr_fd(cmd[1], 2);
		return 1;
	}
	else
		change_dir(cmd[1], list);
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
