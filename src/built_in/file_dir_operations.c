/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_dir_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:29:42 by junsan            #+#    #+#             */
/*   Updated: 2024/07/04 13:07:34 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int init_oldpwd_node(t_env *lst)
{
	t_env *tmp;
	tmp = lst;
	if (!tmp)
		return 0;
	while(tmp)
	{
		if (ft_strncmp(tmp->name, "OLDPWD",6) == 0)
		{
			tmp->old_pwd = tmp;
			return 1;
		}
		tmp=tmp->next;
	}
	return 0;
}

int	change_dir(const char *path, t_env *lst)
{
	(void) lst;
	init_oldpwd_node(lst);
	if (!path || *path == '\0')
		return (0);
	if (ft_strncmp(path, "-", 1) == 0)
		chdir(lst->old_pwd->content);
	else if (chdir(path) == -1)
		return (0);
	else
		return (1);
	return (0);
}


// bool	get_cur_dir(void)
// {
// 	char	cwd[MAX_PATH_LENGTH];

// 	if (getcwd(cwd, sizeof(cwd)) != NULL)
// 	{
// 		printf("%s\n", cwd);
// 		return (SUCCESS);
// 	}
// 	else
// 		perror("getcwd");
// 	return (FAILURE);
// }

// bool	file_exist(const char *filename)
// {
// 	return (access(filename, F_OK) == 0);
// }

// void	list_dir(const char *dirname)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;

// 	dir = opendir(dirname);
// 	if (dir == NULL)
// 	{
// 		perror("opendir");
// 		return ;
// 	}
// 	entry = readdir(dir);
// 	while (entry != NULL)
// 		printf("%s\n", entry->d_name);
// 	closedir(dir);
// }
