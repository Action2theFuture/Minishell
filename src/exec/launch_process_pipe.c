/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_process_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 08:49:20 by junsan            #+#    #+#             */
/*   Updated: 2024/08/19 11:04:14 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_for_child_task(t_info *info)
{
	int		status;
	bool	printed_signal_msg;

	status = 0;
	printed_signal_msg = false;
	set_signal_handler(IGN);
	while (wait(&status) != -1)
	{
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT && !printed_signal_msg)
			{
				ft_putstr_fd("^C\n", STDERR_FILENO);
				printed_signal_msg = true;
			}
			else if (WTERMSIG(status) == SIGPIPE)
				continue ;
			info->exit_status = 128 + WTERMSIG(status);
		}
		else if (WIFEXITED(status))
			info->exit_status = WEXITSTATUS(status);
	}
	set_signal_handler(SIGNAL_HANDLER);
	return (SUCCESS);
}

static void	execute_child_process(\
		char *cmd, char **args, t_info *info)
{
	char	**env;

	env = (char **)list_to_array(info->env);
	if (env == NULL)
	{
		perror("Empty env");
		exit(1);
	}
	if (info->pipe_loc == FIRST)
	{
		first_pipe(cmd, env, args, info);
	}
	else if (info->pipe_loc == MIDDLE)
		middle_pipe(cmd, env, args, info);
	else if (info->pipe_loc == LAST)
		last_pipe(cmd, env, args, info);
	exit(info->exit_status);
}

static void	close_pipe_ends(t_info *info)
{
	if (info->pipe_loc == FIRST)
	{
		if (info->tmp_pipe[2] != -1)
			close(info->tmp_pipe[2]);
		info->tmp_pipe[2] = -1;
	}
	if (info->pipe_loc == MIDDLE)
	{
		if (info->tmp_pipe[0] != -1)
			close(info->tmp_pipe[0]);
		info->tmp_pipe[0] = -1;
		if (info->tmp_pipe[2] != -1)
			close(info->tmp_pipe[2]);
		info->tmp_pipe[2] = -1;
	}
	if (info->pipe_loc == LAST)
	{
		if (info->tmp_pipe[0] != -1)
			close(info->tmp_pipe[0]);
		info->tmp_pipe[0] = -1;
	}
}

static void	handle_parent_process(t_info *info)
{
	if (info->pipe_loc == FIRST || info->pipe_loc == MIDDLE)
		close_pipe_ends(info);
	if (info->pipe_loc == LAST)
	{
		close_pipe_ends(info);
		wait_for_child_task(info);
	}
}

int	launch_process_pipe(char *cmd, char **args, t_info *info)
{
	if (info->pipe_loc == FIRST || info->pipe_loc == MIDDLE)
	{
		if (pipe(info->pipe) == -1)
			return (fd_log_error("pipe error", NULL, NULL));
	}
	info->tmp_pipe[0] = info->stdin_pipe;
	if (info->is_heredoc)
		info->tmp_pipe[0] = dup(STDIN_FILENO);
	info->tmp_pipe[1] = info->pipe[0];
	info->tmp_pipe[2] = info->pipe[1];
	info->pid = fork();
	if (info->pid == -1)
		return (fd_log_error("fork error", NULL, NULL));
	if (info->pid == 0)
		(set_signal_handler(DFL), execute_child_process(cmd, args, info));
	else
		handle_parent_process(info);
	if (info->pipe_loc == FIRST || info->pipe_loc == MIDDLE)
		info->stdin_pipe = info->pipe[0];
	if (info->pipe[1] != -1)
		(close(info->pipe[1]), info->pipe[1] = -1);
	return (info->exit_status);
}
