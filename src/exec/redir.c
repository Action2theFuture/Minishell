/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 12:01:59 by junsan            #+#    #+#             */
/*   Updated: 2024/08/04 10:27:23 by junsan           ###   ########.fr       */
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

static bool	is_valid_ambiguous_redirect(const char *str1, const char *str2)
{
	if (str1 && ft_strchr(str1, '$') != NULL)
	{
		if (str2 == NULL || *str2 == '\0')
			return (false);
		while (*str2)
		{
			if (ft_isspace((unsigned char)*str2))
				return (false);
			str2++;
		}
	}
	return (true);
}

// first ele in args is file name or heredoc limiter
int	handle_io_redirection(t_ast *node, t_info *info)
{
	int		status;
	char	**args;
	char	*first_arg;

	args = NULL;
	close_fds(info);
	while (node && status == SUCCESS)
	{
		args = ft_split(node->right->data, ARR_SEP);
		first_arg = ft_strdup(args[0]);
		expand_and_strip_quotes_in_args(args, info);
		if (is_valid_ambiguous_redirect(first_arg, args[0]))
			status = handle_ft_redirection(args[0], node, info);
		else
			status = fd_log_error(first_arg, NULL, "ambiguous redirect");
		(process_redir_args(args, info), free_args(args));
		if (first_arg)
			free(first_arg);
		if (status > SUCCESS)
			info->is_pipe = false;
		if (!node->left->left)
			break ;
		node = node->left->left;
	}
	return (status);
}
