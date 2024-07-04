/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bin_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 10:00:06 by junsan            #+#    #+#             */
/*   Updated: 2024/07/02 10:14:44 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_bin_path(const char *cmd)
{
	const char	*base_path;
	char		*full_path;
	size_t		total_len;

	base_path = "/bin/";
	total_len = ft_strlen(base_path) + ft_strlen(cmd) + 1;
	full_path = (char *)malloc(sizeof(char) * total_len);
	if (full_path == NULL)
		return (perror("malloc error"), NULL);
	ft_strlcpy(full_path, base_path, total_len);
	ft_strlcat(full_path, cmd, total_len);
	return (full_path);
}
