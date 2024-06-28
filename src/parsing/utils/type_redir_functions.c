/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_redir_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 18:41:02 by junsan            #+#    #+#             */
/*   Updated: 2024/06/28 16:12:24 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_input_redirection(const char *data)
{
	int	len;

	len = ft_strlen(data);
	if (len != 1)
		return (false);
	return (ft_strncmp(data, "<", 1) == 0 && ft_strlen(data) == 1);
}

bool	is_output_redirection(const char *data)
{
	int	len;

	len = ft_strlen(data);
	if (len != 1)
		return (false);
	return (ft_strncmp(data, ">", 1) == 0 && ft_strlen(data) == 1);
}

bool	is_append_redirection(const char *data)
{
	int	len;

	len = ft_strlen(data);
	if (len != 2)
		return (false);
	return (ft_strncmp(data, ">>", 2) == 0 && ft_strlen(data) == 2);
}

bool	is_heredoc_redirection(const char *data)
{
	int	len;

	len = ft_strlen(data);
	if (len != 2)
		return (false);
	return (ft_strncmp(data, "<<", 2) == 0 && ft_strlen(data) == 2);
}

bool	is_herestr_redirection(const char *data)
{
	int	len;

	len = ft_strlen(data);
	if (len != 3)
		return (false);
	return (ft_strncmp(data, "<<<", 3) == 0 && ft_strlen(data) == 3);
}
