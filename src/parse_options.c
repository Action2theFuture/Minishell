/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 09:14:01 by junsan            #+#    #+#             */
/*   Updated: 2024/05/12 09:19:02 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// getopt ... 
void parse_options(int ac, char **av)
{
	int i;

	i = -1;
	while (++i < ac)
	{
		if (!ft_strcmp(av[i], "-a"))
			printf("Option... -a\n");
		else if (!ft_strcmp(av[i], "-b"))
			printf("Option... -b\n");
		else if (!ft_strcmp(av[i], "-f"))
		{
			if (i + 1 < ac)
			{
				printf("Option -f is set with value %s\n", av[i + 1]);
			}
			else
				printf("Option -f requires an argument'\n");
		}
		else
			printf("getopt: missing optstring argument\n");
	}
}
