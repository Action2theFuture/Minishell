/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 21:28:11 by junsan            #+#    #+#             */
/*   Updated: 2024/08/20 20:52:28 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd_with_pipe(\
				char *cmd, char **args, t_info *info, char **env)
{
	if ((ft_strlen(cmd) == ft_strlen("./minishell")) && \
		ft_strncmp(cmd, "./minishell", 11) == 0 && info->pipe_loc != LAST)
		cleanup_and_exit(SUCCESS, args, env, info);
	if (ft_strlen(cmd) == 1 && ft_strncmp(cmd, ".", 1) == 0)
		cleanup_and_exit(execve_log_error(cmd, errno), args, env, info);
	if (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "true", 4) == 0)
		cleanup_and_exit(SUCCESS, args, env, info);
	else if (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "false", 5) == 0)
		cleanup_and_exit(FAILURE, args, env, info);
	if (ft_strlen(args[0]) > 2 && args[0][0] == '.' && args[0][1] == '/'
		&& execve(cmd, args, env) == -1)
		cleanup_and_exit(125 + execve_log_error(cmd, ENOENT), args, env, info);
	else if (info->path)
	{
		if (execve(info->path, args, env) == -1)
			cleanup_and_exit(\
					126 + execve_log_error(cmd, EFAULT), args, env, info);
		(free(info->path), info->path = NULL);
	}
	if (execve(cmd, args, env) == -1)
		cleanup_and_exit(126 + execve_log_error(cmd, EFAULT), args, env, info);
}

void	first_pipe(char *cmd, char **env, char **args, t_info *info)
{
	int		built_in;
	int		(*arr_built_in[7])(\
			const char *, const char **, t_env *);

	if (info->pid == 0)
	{
		if (info->tmp_pipe[2] != -1)
			dup2(info->tmp_pipe[2], STDOUT_FILENO);
		close_tmp_pipe(info->tmp_pipe);
		init_builtin(arr_built_in);
		built_in = handler_builtin(cmd);
		if (built_in != NONE)
		{
			if (built_in == EXIT)
				cleanup_and_exit(0, args, env, info);
			cleanup_and_exit(arr_built_in[built_in](\
			(const char *)cmd, (const char **)args, info->env), \
					args, env, info);
		}
		else
			execute_cmd_with_pipe(cmd, args, info, env);
	}
}

void	middle_pipe(char *cmd, char **env, char **args, t_info *info)
{
	int		built_in;
	int		(*arr_built_in[7])(\
			const char *, const char **, t_env *);

	if (info->pid == 0)
	{
		if (info->tmp_pipe[0] != -1)
			dup2(info->tmp_pipe[0], STDIN_FILENO);
		if (info->tmp_pipe[2] != -1)
			dup2(info->tmp_pipe[2], STDOUT_FILENO);
		close_tmp_pipe(info->tmp_pipe);
		init_builtin(arr_built_in);
		built_in = handler_builtin(cmd);
		if (built_in != NONE)
		{
			if (built_in == EXIT)
				cleanup_and_exit(0, args, env, info);
			cleanup_and_exit(arr_built_in[built_in](\
			(const char *)cmd, (const char **)args, info->env), \
					args, env, info);
		}
		else
			execute_cmd_with_pipe(cmd, args, info, env);
	}
}

void	last_pipe(char *cmd, char **env, char **args, t_info *info)
{
	int		status;
	int		built_in;
	int		(*arr_built_in[7])(\
			const char *, const char **, t_env *);

	if (info->pid == 0)
	{
		if (info->tmp_pipe[0] != -1)
			dup2(info->tmp_pipe[0], STDIN_FILENO);
		close_tmp_pipe(info->tmp_pipe);
		init_builtin(arr_built_in);
		built_in = handler_builtin(cmd);
		if (built_in != NONE)
		{
			if (built_in == EXIT)
				cleanup_and_exit(0, args, env, info);
			status = (arr_built_in[built_in](\
			(const char *)cmd, (const char **)args, info->env));
			cleanup_and_exit(status, args, env, info);
		}
		else
			execute_cmd_with_pipe(cmd, args, info, env);
	}
	close_tmp_pipe(info->tmp_pipe);
}
