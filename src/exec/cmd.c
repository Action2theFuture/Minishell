/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:58:55 by junsan            #+#    #+#             */
/*   Updated: 2024/06/29 18:02:36 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**prepend_cmd_and_add_spaces(const char *cmd, char **args)
{
	char	**new_args;
	int		arg_cnt;
	int		i;

	arg_cnt = 0;
	while (args[arg_cnt])
		arg_cnt++;
	new_args = (char **)malloc(sizeof(char *) * (arg_cnt + 2));
	if (!new_args)
	{
		perror("malloc error");
		return (NULL);
	}
	new_args[0] = ft_strdup(cmd);
	i = -1;
	while (++i < arg_cnt)
		new_args[i + 1] = args[i];
	new_args[arg_cnt + 1] = NULL;
	return (new_args);
}

static char	**prepare_cmd(\
			char **args, t_ast *cmd_node, t_ast *args_node, t_info *info)
{
	char	**chunk;

	args = NULL;
	chunk = NULL;
	if (get_path_type(cmd_node->data, info) == PATH_RELATIVE)
		info->path = get_absolute_path(cmd_node->data);
	if (get_path_type(cmd_node->data, info) == PATH_COMMAND)
		info->path = find_cmd_in_path(cmd_node->data, info->env);
	if (args_node)
	{
		args = ft_split(args_node->data, ARR_SEP);
		chunk = prepend_cmd_and_add_spaces(cmd_node->data, args);
	}
	else
		chunk = allocate_null_and_cmd_chunk(cmd_node->data);
	return (chunk);
}

static int	execute_cmd(char **chunk, t_info *info)
{
	int		status;
	int		built_in;
	int		(*arr_built_in[8])(const char *, const char **, t_env *);

	init_builtin(arr_built_in);
	built_in = handler_builtin(chunk[0]);
	if (built_in != NONE)
		status = arr_built_in[built_in](\
				(const char *)chunk[0], (const char **)chunk, info->env);
	else
		status = launch_process(chunk[0], chunk, info);
	free(chunk[0]);
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
