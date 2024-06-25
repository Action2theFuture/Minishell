/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 11:49:26 by junsan            #+#    #+#             */
/*   Updated: 2024/06/25 18:34:20 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*get_type_str_redir(int type)
{
	if (type == IN_REDIR)
		return ("IN_REDIR");
	else if (type == IN_HEREDOC)
		return ("IN_HEREDOC");
	else if (type == IN_HERESTR)
		return ("IN_HERESTR");
	else if (type == OUT_REDIR)
		return ("OUT_REDIR");
	else if (type == OUT_APPEND)
		return ("OUT_APPEND");
	else if (type == NOT_REDIR)
		return ("NOT_REDIR");
	return ("UNKNOWN");
}

static const char	*get_type_str(int type)
{
	if (type == LOGICAL)
		return ("LOGICAL");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECTION)
		return ("REDIRECTION");
	else if (type == IO)
		return ("IO");
	else if (type == CMD)
		return ("CMD");
	else if (type == SUBSHELL)
		return ("SUBSHELL");
	else if (type == PHRASE)
		return ("PHRASE");
	else if (type == FILE_NAME)
		return ("FILE_NAME");
	else if (type == ARGS)
		return ("ARGS");
	return (get_type_str_redir(type));
}

void	print_token(t_token *head)
{
	if (!head)
	{
		printf("empty head\n");
		return ;
	}
	while (head)
	{
		printf("data : %s, type : %s\n", head->data, get_type_str(head->type));
		head = head->next;
	}
}

void	print_file_list(t_file_list *file_list)
{
	int	i;

	i = -1;
	while (++i < (int)file_list->count)
		printf("file_list %d.name :  %s\n", i, file_list->names[i]);
}
