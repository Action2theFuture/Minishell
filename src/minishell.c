/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 19:14:21 by junsan            #+#    #+#             */
/*   Updated: 2024/07/02 12:27:52 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_shell(void)
{
	printf("\033[1A");
	printf("\033[10C");
	printf("exit\n");
}

int	main(int ac, char **av, char **envp)
{
	static int	exit_status;
	t_env		*env;
	char		*input;

	(void)ac;
	(void)av;
	env = NULL;
	exit_status = 0;
	init_minishell(envp, &env);
	set_signal_handler();
	while (1)
	{
		input = readline("kashell$> ");
		if (!input)
		{
			exit_shell();
			break ;
		}
		if (!*input)
			continue ;
		else
			add_history(input);
		process_input(input, env, &exit_status);
	}
	return (clear_env(env), 0);
}
