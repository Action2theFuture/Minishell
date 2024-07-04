/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:58:55 by junsan            #+#    #+#             */
/*   Updated: 2024/07/05 10:25:02 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**prepend_cmd_and_add_spaces(char **cmd, char **args, int cmd_cnt)
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
	{
		remove_empty_quotes(args[i]);
		new_args[cmd_cnt + i] = ft_strdup(args[i]);
	}
	return (new_args[cmd_cnt + arg_cnt] = NULL, new_args);
}

static void	determine_and_set_path(const char *cmd, t_info *info)
{
	t_path_type	path_type;

	path_type = get_path_type(cmd, info);
	if (path_type == PATH_RELATIVE)
		info->path = get_absolute_path(cmd);
	else if (path_type == PATH_COMMAND)
		info->path = find_cmd_in_path(cmd, info->env);
	else if (path_type == PATH_INVALID)
		info->path = get_bin_path(cmd);
}

static char	**prepare_cmd(\
			char **args, t_ast *cmd_node, t_ast *args_node, t_info *info)
{
	char		**chunk;
	char		**parsed_cmd;
	int			cnt;

	args = NULL;
	chunk = NULL;
	determine_and_set_path(cmd_node->data, info);
	cnt = 0;
	parsed_cmd = parse_cmd_line_with_quotes(cmd_node->data, &cnt);
	if (args_node)
	{
		args = ft_split(args_node->data, ARR_SEP);
		chunk = prepend_cmd_and_add_spaces(parsed_cmd, args, cnt);
		clear_arr(args);
	}
	else
		chunk = allocate_null_and_cmd_chunk(parsed_cmd, cnt);
	clear_arr(parsed_cmd);
	return (chunk);
}

static int	execute_cmd(char **chunk, t_info *info)
{
	int		status;
	int		i;
	int		built_in;
	int		(*arr_built_in[8])(const char *, const char **, t_env *);

	replace_env_vars_in_args(chunk, info);
	init_builtin(arr_built_in);
	built_in = handler_builtin(chunk[0]);
	if (built_in == ENV || built_in == M_ECHO || built_in == NONE)
		status = launch_process(chunk[0], chunk, info);
	else
		status = arr_built_in[built_in](\
			(const char *)chunk[0], (const char **)chunk, info->env);
	i = -1;
	while (chunk[++i])
		free(chunk[i]);
	free(chunk);
	if (info->path)
		free(info->path);
	return (status);
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
	status = execute_cmd(chunk, info);
	info->pipe_exists = false;
	if (args)
		free_args(args);
	return (status);
}
