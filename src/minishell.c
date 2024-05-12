/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 19:14:21 by junsan            #+#    #+#             */
/*   Updated: 2024/05/12 09:20:48 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* example
int main(int ac, char **av, char **env)
{
	init_minishell(env);

	set_signal_handler();

	while (true)
	{
		char *input = get_user_input();

		process_input(input);

		free(input);
	}
	return (0);
}
*/

int main(int ac, char **av, char **env)
{
	init_minishell(env);

	set_signal_handler();

	while (true)
	{
		char *input = get_user_input();

		process_input(input);

		free(input);
	}
	return (0);
}
