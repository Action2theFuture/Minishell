/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 12:01:59 by junsan            #+#    #+#             */
/*   Updated: 2024/08/01 10:10:41 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_redir_args(char **args, t_info *info)
{
	char	**new_redir_args;
	char	**merged_args;

	if (args[1])
	{
		new_redir_args = copy_str_arr(&args[1]);
		if (info->redir_args)
		{
			merged_args = merge_args_and_add_spaces(\
			info->redir_args, new_redir_args, count_strings(info->redir_args));
			free_args(new_redir_args);
			info->redir_args = merged_args;
		}
		else
			info->redir_args = new_redir_args;
	}
}

// first ele in args is file name or heredoc limiter
int	handle_io_redirection(t_ast *node, t_info *info)
{
	int		status;
	char	**args;

	args = NULL;
	status = SUCCESS;
	close_fds(info);
	while (node && status == SUCCESS)
	{
		args = ft_split(node->right->data, ARR_SEP);
		process_quotes_in_args(args);
		status = handle_ft_redirection(args[0], node, info);
		process_redir_args(args, info);
		free_args(args);
		if (status > SUCCESS)
			info->is_pipe = false;
		if (!node->left->left)
			break ;
		node = node->left->left;
	}
	return (status);
}
