/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:58:55 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 18:55:43 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	determine_and_set_path(const char *cmd, t_info *info)
{
	t_path_type	path_type;

	if (cmd == NULL)
		return ;
	path_type = get_path_type(cmd, info);
	if (path_type == PATH_RELATIVE)
		info->path = get_absolute_path(cmd);
	else if (path_type == PATH_COMMAND)
		info->path = find_cmd_in_path(cmd, info->env);
	else if (path_type == PATH_INVALID)
		info->path = get_bin_path(cmd);
}

char	**merge_args_and_add_spaces(char **cmd, char **args, int cmd_cnt)
{
	char	**new_args;
	int		arg_cnt;
	int		i;

	arg_cnt = 0;
	while (args[arg_cnt])
		arg_cnt++;
	new_args = (char **)malloc(sizeof(char *) * (cmd_cnt + arg_cnt + 1));
	if (!new_args)
		return (perror("malloc error"), NULL);
	i = -1;
	while (++i < cmd_cnt)
		new_args[i] = ft_strdup(cmd[i]);
	i = -1;
	while (++i < arg_cnt)
		new_args[cmd_cnt + i] = ft_strdup(args[i]);
	new_args[cmd_cnt + arg_cnt] = NULL;
	if (cmd)
		free_args(cmd);
	return (new_args);
}

static char	**prepare_cmd(\
			char **args, t_ast *cmd_node, t_ast *args_node, t_info *info)
{
	char		**chunk;
	char		**parsed_cmd;
	int			cmd_cnt;

	args = NULL;
	chunk = NULL;
	cmd_cnt = 0;
	parsed_cmd = parse_cmd_line_with_quotes(cmd_node->data, &cmd_cnt);
	if (args_node)
	{
		args = ft_split(args_node->data, ARR_SEP);
		chunk = merge_args_and_add_spaces(parsed_cmd, args, cmd_cnt);
		free_args(args);
	}
	else
		chunk = allocate_null_and_cmd_chunk(parsed_cmd, cmd_cnt);
	if (info->redir_args)
		chunk = merge_args_and_add_spaces(\
				chunk, info->redir_args, count_strings(chunk));
	expand_wildcard(&chunk);
	expand_and_strip_quotes_in_args(chunk, info);
	determine_and_set_path(chunk[0], info);
	fetch_last_arg(chunk, info);
	return (chunk);
}

static int	start_execute(char **chunk, t_info *info)
{
	int		status;
	int		built_in;
	int		(*arr_built_in[7])(const char *, const char **, t_env *);

	info->args = chunk;
	if (info->is_pipe)
		status = launch_process_pipe(chunk[0], chunk, info);
	else
	{
		init_builtin(arr_built_in);
		built_in = handler_builtin(chunk[0]);
		if (built_in != NONE)
		{
			if (built_in == EXIT)
				status = ft_exit(info);
			else
				status = arr_built_in[built_in](\
				(const char *)chunk[0], (const char **)chunk, info->env);
		}
		else
			status = launch_process_cmd(chunk[0], chunk, info);
	}
	if (info->path)
		(free(info->path), info->path = NULL);
	return (free_args(chunk), status);
}

int	dispatch_cmd(t_ast	*node, t_info *info)
{
	t_ast	*cmd_node;
	t_ast	*args_node;
	char	**chunk;
	char	**args;
	int		status;

	cmd_node = node->left;
	args_node = node->right;
	info->path = NULL;
	args = NULL;
	chunk = prepare_cmd(args, cmd_node, args_node, info);
	status = start_execute(chunk, info);
	if (args)
		free_args(args);
	return (status);
}
