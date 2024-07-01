/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:29:28 by junsan            #+#    #+#             */
/*   Updated: 2024/06/29 13:12:04 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_logical_operator(const char *token)
{
	return (ft_strncmp(token, "&&", 2) == 0 || \
		ft_strncmp(token, "||", 2) == 0);
}

bool	is_pipe_operator(const char *token)
{
	return (ft_strncmp(token, "|", 1) == 0);
}

bool	is_redirection_operator(const char *token)
{
	int	cnt;

	cnt = count_repeated_chars(token, token[0]);
	if (cnt <= 2)
	{
		return (ft_strncmp(token, "<", 1) == 0 || \
			ft_strncmp(token, "<<", 2) == 0 || \
			ft_strncmp(token, ">", 1) == 0 || \
			ft_strncmp(token, ">>", 2) == 0);
	}
	else if (cnt == 3)
		return (ft_strncmp(token, "<<<", 3) == 0);
	return (false);
}

bool	is_subshell_operator(const char *token)
{
	size_t	len;

	len = ft_strlen(token);
	return (token[0] == '(' || token[len - 1] == ')');
}

bool	is_file_name(const char *token)
{
	t_file_list	*file_list;
	const char	*path;
	bool		flag;
	int			i;

	flag = false;
	path = get_path(token);
	if (path == NULL)
		return (flag);
	file_list = get_file_list(path);
	i = -1;
	while (++i < (int)file_list->count)
	{
		if (ft_strncmp(file_list->names[i], token, ft_strlen(token)) == 0)
		{
			flag = true;
			break ;
		}
	}
	print_file_list(file_list);
	free_file_list(file_list);
	free((char *)path);
	return (flag);
}
