/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_heredoc_signal.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:11:02 by junsan            #+#    #+#             */
/*   Updated: 2024/08/07 14:12:59 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handler_heredoc_sigint(int signo)
{
	if (signo == SIGINT)
		g_heredoc_interrupted = 1;
}

void	set_heredoc_signal_handler(void)
{
	struct sigaction	sa;

	sa.sa_handler = handler_heredoc_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
