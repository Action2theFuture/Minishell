/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:24:08 by junsan            #+#    #+#             */
/*   Updated: 2024/07/21 10:10:20 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	show_ascii(void)
{
	char	*line;
	int		fd;

	fd = open(ASCII_ART_PATH, O_RDONLY);
	while ("minishell")
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		printf("%s", line);
		free(line);
	}
	close(fd);
}

void	init_minishell(char **envp, t_env **env)
{
	char	*tty;

	show_ascii();
	tty = ttyname(STDIN_FILENO);
	if (tty)
		printf("The current terminal for this process is: %s\n", tty);
	else
		printf("No terminal associated with this process.\n");
	*env = env_init(envp);
	if (*env == NULL)
		exit(1 + printf("Fatal error: initialization failed."));
	if (increment_shlvl(*env) == FAILURE)
		exit(1 + printf("Fatal error: initialization failed."));
}
