/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch_cmd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 20:14:55 by junsan            #+#    #+#             */
/*   Updated: 2024/07/31 20:15:04 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**prepend_cmd_and_add_spaces(char **cmd, char **args, int cmd_cnt)
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
	return (new_args);
}
