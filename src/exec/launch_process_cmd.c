/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_process_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:08:10 by junsan            #+#    #+#             */
/*   Updated: 2024/08/20 08:39:11 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd(char *cmd, char **args, t_info *info, char **env)
{
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
			cleanup_and_exit(126 + execve_log_error(cmd, EFAULT), args, env,
				info);
		(free(info->path), info->path = NULL);
	}
	if (execve(cmd, args, env) == -1)
		cleanup_and_exit(126 + execve_log_error(cmd, EFAULT), args, env, info);
}

static int	monitor_child_task(pid_t pid, t_info *info)
{
	int	status;
	int	sig;

	status = 0;
	set_signal_handler(IGN);
	if (waitpid(pid, &status, 0) == -1)
		return (set_signal_handler(SIGNAL_HANDLER), FAILURE);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ft_putstr_fd("^C\n", STDERR_FILENO);
		else if (sig == SIGQUIT)
			ft_putstr_fd("^\\Quit (core dumped)\n", STDERR_FILENO);
		if (sig != SIGPIPE)
			info->exit_status = 128 + sig;
	}
	else if (WIFEXITED(status))
		info->exit_status = WEXITSTATUS(status);
	set_signal_handler(SIGNAL_HANDLER);
	return (SUCCESS);
}

int	launch_process_cmd(char *cmd, char **args, t_info *info)
{
	pid_t pid;
	char **env;

	pid = fork();
	if (pid == -1)
		return (fd_log_error("fork error", NULL, NULL));
	else if (pid == 0)
	{
		set_signal_handler(DFL);
		env = list_to_array(info->env);
		execute_cmd(cmd, args, info, env);
	}
	else
		return (monitor_child_task(pid, info));
	return (SUCCESS);
}