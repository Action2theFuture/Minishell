/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils_for_expansion.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:36:39 by junsan            #+#    #+#             */
/*   Updated: 2024/07/11 18:37:15 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_env_var(\
	const char *str, char *res, t_env_var *env_var, t_info *info)
{
	env_var->is_value_expansion = false;
	env_var->str = str;
	env_var->res = res;
	env_var->info = info;
	env_var->i = 0;
	env_var->j = 0;
	env_var->in_double_quotes = 0;
}

void	init_handler_info(\
			t_handler_info *h_info, char *new_str, t_info *info)
{
	h_info->new_str = new_str;
	h_info->new_str_len = 0;
	h_info->var_len = 0;
	h_info->i = 0;
	h_info->in_single_quotes = false;
	h_info->in_single_quotes = false;
	h_info->info = info;
}
