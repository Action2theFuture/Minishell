/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:49:23 by junsan            #+#    #+#             */
/*   Updated: 2024/07/03 20:46:09 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_type	get_type(const char *data)
{
	if (is_logical_operator(data))
		return (LOGICAL);
	if (is_pipe_operator(data))
		return (PIPE);
	if (is_redirection_operator(data))
		return (REDIRECTION);
	if (is_subshell_operator(data))
		return (SUBSHELL);
	return (CMD);
}

t_type	get_type_redir(const char *data)
{
	if (is_input_redirection(data))
		return (IN_REDIR);
	else if (is_output_redirection(data))
		return (OUT_REDIR);
	else if (is_append_redirection(data))
		return (OUT_APPEND);
	else if (is_heredoc_redirection(data))
		return (IN_HEREDOC);
	else if (is_herestr_redirection(data))
		return (IN_HERESTR);
	return (NOT_REDIR);
}
