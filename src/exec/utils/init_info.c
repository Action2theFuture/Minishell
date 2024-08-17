/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:04:00 by junsan            #+#    #+#             */
/*   Updated: 2024/08/17 17:02:23 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_fds(t_info *info)
{
	info->stdin_pipe = -1;
	info->stdin_fd = -1;
	info->stdout_fd = -1;
	info->origin_stdin_fd = -1;
	info->origin_stdout_fd = -1;
	info->backup_stdin = -1;
	info->backup_stdout = -1;
	info->tmp_fd = -1;
	info->stdin_backup = dup(STDIN_FILENO);
	if (info->stdin_backup == -1)
		perror("dup stdin_backup error");
	info->stdout_backup = dup(STDOUT_FILENO);
	if (info->stdout_backup == -1)
		perror("dup stdout_backup error");
}

static void	init_flags(t_info *info)
{
	info->status = SUCCESS;
	info->exit_status = SUCCESS;
	info->is_pipe = false;
	info->is_re_pipe = false;
	info->is_heredoc = false;
	info->is_expansion_heredoc = false;
	info->is_last = false;
	info->pipe_loc = -1;
	info->in_subshell = false;
	info->has_multiple_pipes = false;
}

static void	init_pipes(t_info *info)
{
	info->pipe[0] = -1;
	info->pipe[1] = -1;
}

static void	init_pointers(t_info *info, t_env *env, t_ast *root)
{
	info->path = NULL;
	info->redir_args = NULL;
	info->env = env;
	info->root = root;
	info->nested_subshell_root = NULL;
	info->quote_info = NULL;
	info->token = NULL;
}

void	init_info(t_info *info, t_env *env, t_ast *root)
{
	init_flags(info);
	init_fds(info);
	init_pipes(info);
	init_pointers(info, env, root);
}
