/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:25:43 by junsan            #+#    #+#             */
/*   Updated: 2024/08/17 11:34:10 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_and_exit(int status, char **args, char **env, t_info *info)
{
	if (env)
		(clear_env_arr(env), env = NULL);
	(free_tree(info->root), info->root = NULL);
	if (info->nested_subshell_root)
		(free_tree(info->nested_subshell_root), \
		info->nested_subshell_root = NULL);
	if (args)
		free_args(args);
	if (info->path)
		(free(info->path), info->path = NULL);
	if (info->token)
		(free_token(info->token), info->token = NULL);
	clear_env(info->env);
	info->env = NULL;
	exit(status);
}
