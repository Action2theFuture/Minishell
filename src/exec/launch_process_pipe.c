/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_process_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 08:49:20 by junsan            #+#    #+#             */
/*   Updated: 2024/07/19 22:04:21 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_for_child_task(t_info *info)
{
	int	status;
	int	i;

	i = 0;
	set_signal_handler(IGN);
	while (wait(&status) != -1)
	{
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT && i == 0)
				ft_putstr_fd("^C\n", STDERR_FILENO);
			info->exit_status = 128 + WTERMSIG(status);
			i++;
		}
		else if (WIFEXITED(status))
			info->exit_status = WEXITSTATUS(status);
	}
	set_signal_handler(SIGNAL_HANDLER);
	return (SUCCESS);
}

static void	execute_child_process(char *cmd, char **args, t_info *info)
{
	char	**env;

	env = (char **)list_to_array(info->env);
	if (env == NULL)
	{
		perror("Empty env");
		exit(1);
	}
	if (info->pipe_loc == FIRST)
		first_pipe(cmd, env, args, info);
	else if (info->pipe_loc == MIDDLE)
		middle_pipe(cmd, env, args, info);
	else if (info->pipe_loc == LAST)
		last_pipe(cmd, env, args, info);
	if (env)
		clear_env_arr(env);
	exit(info->exit_status);
}

static void	handle_parent_process(t_info *info)
{
	if (info->pipe_loc == FIRST)
	{
		close(info->tmp_pipe[2]);
	}
	if (info->pipe_loc == MIDDLE)
	{
		close(info->tmp_pipe[0]);
		close(info->tmp_pipe[2]);
	}
	if (info->pipe_loc == LAST)
	{
		close(info->tmp_pipe[0]);
		wait_for_child_task(info);
	}
}

int	launch_process_pipe(char *cmd, char **args, t_info *info)
{
	if (info->pipe_loc != LAST)
	{
		if (pipe(info->pipe) == -1)
			return (fd_log_error("pipe error", NULL, NULL));
	}
	info->tmp_pipe[0] = info->stdin_pipe;
	info->tmp_pipe[1] = info->pipe[0];
	info->tmp_pipe[2] = info->pipe[1];
	info->pid = fork();
	if (info->pid == -1)
		return (fd_log_error("fork error", NULL, NULL));
	if (info->pid == 0)
	{
		set_signal_handler(DFL);
		execute_child_process(cmd, args, info);
	}
	else
		handle_parent_process(info);
	if (info->pipe_loc != LAST)
		info->stdin_pipe = info->pipe[0];
	return (info->exit_status);
}
