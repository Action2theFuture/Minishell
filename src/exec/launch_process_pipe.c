/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_process_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 08:49:20 by junsan            #+#    #+#             */
/*   Updated: 2024/07/19 15:22:40 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	first_pipe(char *cmd, char **env, char **args, t_info *info)
{
	if (info->pid == 0)
	{
		dup2(info->tmp_pipe[2], STDOUT_FILENO);
		close(info->tmp_pipe[0]);
		close(info->tmp_pipe[1]);
		close(info->tmp_pipe[2]);
		execute_cmd(cmd, args, info, env);
	}
	else
		close(info->tmp_pipe[2]);
}

static void	middle_pipe(char *cmd, char **env, char **args, t_info *info)
{
	if (info->pid == 0)
	{
		dup2(info->tmp_pipe[0], STDIN_FILENO);
		dup2(info->tmp_pipe[2], STDOUT_FILENO);
		close(info->tmp_pipe[0]);
		close(info->tmp_pipe[1]);
		close(info->tmp_pipe[2]);
		execute_cmd(cmd, args, info, env);
	}
	else
	{
		close(info->tmp_pipe[0]);
		close(info->tmp_pipe[2]);
	}
}

static void	last_pipe(char *cmd, char **env, char **args, t_info *info)
{
	if (info->pid == 0)
	{
		dup2(info->tmp_pipe[0], STDIN_FILENO);
		close(info->tmp_pipe[0]);
		close(info->tmp_pipe[1]);
		close(info->tmp_pipe[2]);
		execute_cmd(cmd, args, info, env);
	}
	else
	{
		close(info->tmp_pipe[0]);
		wait_for_child_task(info);
	}
}

int	wait_for_child_task(t_info *info)
{
	int	status;
	int	i;

	i = 0;
	disable_interrupt_signals();
	while (wait(&status) != -1)
	{
		waitpid(info->child_pids[i], &status, 0);
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
	set_signal_handler();
	return (SUCCESS);
}

int	launch_process_pipe(char *cmd, char **args, t_info *info)
{
	char	**env;

	info->tmp_pipe[0] = info->stdin_pipe;
	info->tmp_pipe[1] = info->pipe[0];
	info->tmp_pipe[2] = info->pipe[1];
	info->pid = fork();
	if (info->pid == -1)
		return (fd_log_error("fork error", NULL, NULL));
	env = (char **)list_to_array(info->env);
	if (env == NULL)
		return (perror("Empty env"), 1);
	if (info->pipe_loc == FIRST)
		first_pipe(cmd, env, args, info);
	else if (info->pipe_loc == MIDDLE)
		middle_pipe(cmd, env, args, info);
	else if (info->pipe_loc == LAST)
		last_pipe(cmd, env, args, info);
	info->stdin_pipe = info->tmp_pipe[1];
	if (env)
		clear_env_arr(env);
	return (info->exit_status);
}
