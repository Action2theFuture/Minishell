/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 21:28:11 by junsan            #+#    #+#             */
/*   Updated: 2024/07/19 21:37:12 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd_with_pipe(\
				char *cmd, char **args, t_info *info, char **env)
{
	if (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "true", 4) == 0)
		exit(SUCCESS);
	else if (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "false", 5) == 0)
		exit(FAILURE);
	if (ft_strlen(args[0]) > 2 && args[0][0] == '.' && args[0][1] == '/'
		&& execve(cmd, args, env) == -1)
		exit(125 + execve_log_error(cmd, errno));
	else if (info->path)
	{
		execve(info->path, args, env);
		free(info->path);
	}
	if (execve(cmd, args, env) == -1)
		exit(126 + execve_log_error(cmd, errno));
}

void	first_pipe(char *cmd, char **env, char **args, t_info *info)
{
	int		built_in;
	int		(*arr_built_in[8])(\
			const char *, const char **, t_env *);

	if (info->pid == 0)
	{
		dup2(info->tmp_pipe[2], STDOUT_FILENO);
		close(info->tmp_pipe[0]);
		close(info->tmp_pipe[1]);
		close(info->tmp_pipe[2]);
		init_builtin(arr_built_in);
		built_in = handler_builtin(cmd);
		if (built_in != NONE)
			exit(arr_built_in[built_in](\
				(const char *)cmd, (const char **)args, info->env));
		else
			execute_cmd_with_pipe(cmd, args, info, env);
	}
}

void	middle_pipe(char *cmd, char **env, char **args, t_info *info)
{
	int		built_in;
	int		(*arr_built_in[8])(\
			const char *, const char **, t_env *);

	if (info->pid == 0)
	{
		dup2(info->tmp_pipe[0], STDIN_FILENO);
		dup2(info->tmp_pipe[2], STDOUT_FILENO);
		close(info->tmp_pipe[0]);
		close(info->tmp_pipe[1]);
		close(info->tmp_pipe[2]);
		init_builtin(arr_built_in);
		built_in = handler_builtin(cmd);
		if (built_in != NONE)
			exit(arr_built_in[built_in](\
				(const char *)cmd, (const char **)args, info->env));
		else
			execute_cmd_with_pipe(cmd, args, info, env);
	}
}

void	last_pipe(char *cmd, char **env, char **args, t_info *info)
{
	int		built_in;
	int		(*arr_built_in[8])(\
			const char *, const char **, t_env *);

	if (info->pid == 0)
	{
		dup2(info->tmp_pipe[0], STDIN_FILENO);
		close(info->tmp_pipe[0]);
		close(info->tmp_pipe[1]);
		close(info->tmp_pipe[2]);
		init_builtin(arr_built_in);
		built_in = handler_builtin(cmd);
		if (built_in != NONE)
			exit(arr_built_in[built_in](\
				(const char *)cmd, (const char **)args, info->env));
		else
			execute_cmd_with_pipe(cmd, args, info, env);
	}
}
