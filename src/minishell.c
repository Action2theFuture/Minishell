/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 19:14:21 by junsan            #+#    #+#             */
/*   Updated: 2024/07/21 10:10:40 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_shell(void)
{
	printf("\033[1A");
	printf("\033[10C");
	printf("exit\n");
}

static void	set_terminal_attributes(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static char	*read_user_input(void)
{
	char	*input;

	input = readline("minishell$> ");
	if (!input)
	{
		exit_shell();
		return (NULL);
	}
	if (!*input)
	{
		free(input);
		return (NULL);
	}
	add_history(input);
	return (input);
}

static void	shell_loop(t_env *env, int *exit_status)
{
	char	*input;

	while ("minishell")
	{
		input = read_user_input();
		if (!input)
			break ;
		process_input(input, env, exit_status);
	}
}

int	main(int ac, char **av, char **envp)
{
	struct termios	term;
	static int		exit_status;
	t_env			*env;

	(void)ac;
	(void)av;
	env = NULL;
	exit_status = 0;
	tcgetattr(STDIN_FILENO, &term);
	set_terminal_attributes();
	set_signal_handler(SIGNAL_HANDLER);
	init_minishell(envp, &env);
	shell_loop(env, &exit_status);
	clear_env(env);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	return (EXIT_SUCCESS);
}
