/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:24:08 by junsan            #+#    #+#             */
/*   Updated: 2024/07/01 09:15:27 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	show_ascii(void)
{
	char	*line;
	int		fd;

	fd = open(ASCII_ART_PATH, O_RDONLY);
	while ("kashell")
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
	char	*check_env_vars;

	show_ascii();
	tty = ttyname(STDIN_FILENO);
	if (tty)
		printf("The current terminal for this process is: %s\n", tty);
	else
		printf("No terminal associated with this process.\n");
	*env = env_init(envp);
	if (*env == NULL)
		exit(1 + printf("Fatal error: initialization failed."));
	check_env_vars = valid_required_env_vars();
	if (check_env_vars != NULL)
		exit(1 + \
		printf("Fatal error: %s environment variable is not set.\n", \
		check_env_vars));
	if (increment_shlvl(*env) == FAILURE)
		exit(1 + printf("Fatal error: initialization failed."));
}
