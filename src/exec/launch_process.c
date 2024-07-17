/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:08:10 by junsan            #+#    #+#             */
/*   Updated: 2024/07/17 11:17:20 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	prepare_and_execute(\
				char *cmd, char **args, t_info *info, char **env)
{
	int		built_in;
	int		(*arr_built_in[8])(const char *, const char **, t_env *);

	if (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "true", 4) == 0)
		exit(SUCCESS);
	else if (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "false", 5) == 0)
		exit(FAILURE);
	init_builtin(arr_built_in);
	built_in = handler_builtin(cmd);
	if (built_in != NONE)
		exit(arr_built_in[built_in](\
			(const char *)cmd, (const char **)args, info->env));
	else if (ft_strlen(args[0]) > 2 && args[0][0] == '.' && args[0][1] == '/'
		&& execve(cmd, args, env) == -1)
		exit(125 + execve_log_error(cmd, errno));
	else if (info->path)
		(execve(info->path, args, env), free(info->path));
	if (execve(cmd, args, env) == -1)
		exit(126 + execve_log_error(cmd, errno));
}

static int	exec_child_task(char *cmd, char **env, char **args, t_info *info)
{
	cmd = args[0];
	if (info->prev_pipe[0] != -1)
	{
		close(info->prev_pipe[1]);
		if (dup2(info->prev_pipe[0], STDIN_FILENO) == -1)
			return (fd_log_error("Dup pipe error 1", NULL, NULL));
	}
	if (info->pipe_exists)
	{
		close(info->pipe[0]);
		if (dup2(info->pipe[1], STDOUT_FILENO) == -1)
			return (fd_log_error("Dup pipe error 2", NULL, NULL));
	}
	prepare_and_execute(cmd, args, info, env);
	return (SUCCESS);
}

// if (info->prev_pipe[0] != -1)
	// (close(info->prev_pipe[0]), close(info->prev_pipe[1]));
static int	monitor_child_task(char *cmd, pid_t pid, t_info *info)
{
	int	status;

	if ((ft_strlen(cmd) == ft_strlen("./minishell")) && \
		ft_strncmp(cmd, "./minishell", 11) == 0)
		disable_interrupt_signals();
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("^C\n", STDERR_FILENO);
		info->exit_status = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		info->exit_status = WEXITSTATUS(status);
	set_signal_handler();
	return (SUCCESS);
}

int	launch_process(char *cmd, char **args, t_info *info)
{
	pid_t	pid;
	char	**env;

	pid = fork();
	if (pid == -1)
		return (fd_log_error("fork error", NULL, NULL));
	env = (char **)list_to_array(info->env);
	if (env == NULL)
		return (perror("Empty env"), 1);
	if (pid == 0)
		exec_child_task(cmd, env, args, info);
	monitor_child_task(cmd, pid, info);
	if (env)
		clear_arr(env);
	if (info->pipe_exists)
	{
		close(info->pipe[1]);
		info->prev_pipe[0] = info->pipe[0];
		info->prev_pipe[1] = info->pipe[1];
		info->pipe_cnt--;
	}
	return (info->exit_status);
}
