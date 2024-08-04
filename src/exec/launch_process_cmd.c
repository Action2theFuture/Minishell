/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_process_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 17:08:10 by junsan            #+#    #+#             */
/*   Updated: 2024/08/04 14:51:59 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd(\
				char *cmd, char **args, t_info *info, char **env)
{
	if (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "true", 4) == 0)
		cleanup_and_exit(SUCCESS, args, env, info);
	else if (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "false", 5) == 0)
		cleanup_and_exit(FAILURE, args, env, info);
	if (ft_strlen(args[0]) > 2 && args[0][0] == '.' && args[0][1] == '/'
		&& execve(cmd, args, env) == -1)
		cleanup_and_exit(125 + execve_log_error(cmd, errno), args, env, info);
	else if (info->path)
	{
		if (execve(info->path, args, env) == -1)
			cleanup_and_exit(\
					126 + execve_log_error(cmd, EFAULT), args, env, info);
		(free(info->path), info->path = NULL);
	}
	if (execve(cmd, args, env) == -1)
		cleanup_and_exit(126 + execve_log_error(cmd, errno), args, env, info);
}

static int	monitor_child_task(char *cmd, pid_t pid, t_info *info)
{
	int	status;

	if ((ft_strlen(cmd) == ft_strlen("./minishell")) && \
		ft_strncmp(cmd, "./minishell", 11) == 0)
		set_signal_handler(IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("^C\n", STDERR_FILENO);
		info->exit_status = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		info->exit_status = WEXITSTATUS(status);
	set_signal_handler(SIGNAL_HANDLER);
	return (SUCCESS);
}

int	launch_process_cmd(char *cmd, char **args, t_info *info)
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
		(execute_cmd(cmd, args, info, env), exit(EXIT_FAILURE));
	else
		monitor_child_task(cmd, pid, info);
	if (env)
		clear_env_arr(env);
	return (info->exit_status);
}
