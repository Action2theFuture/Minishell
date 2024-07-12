/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env_vars.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:08:22 by junsan            #+#    #+#             */
/*   Updated: 2024/07/11 18:37:24 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_outer_double_quotes(t_env_var *env_var)
{
	while (env_var->str[0] == '\"' && \
		env_var->str[ft_strlen(env_var->str) - 1] == '\"')
	{
		remove_double_quotes((char *)env_var->str);
		env_var->is_value_expansion = true;
	}
}

static void	remove_outer_single_quotes(t_env_var *env_var)
{
	while (env_var->str[0] == '\'' && \
		env_var->str[ft_strlen(env_var->str) - 1] == '\'' && \
		!env_var->is_value_expansion)
		remove_single_quotes((char *)env_var->str);
}

static void	process_str(t_env_var *env_var)
{
	while (env_var->str[env_var->i])
	{
		pass_double_quotes(env_var);
		if ((env_var->str[env_var->i] == '\'' && \
			(env_var->str[env_var->i - 1] == '\"' || \
			env_var->str[env_var->i + 1] == '\"')) && \
			env_var->is_value_expansion == true)
			env_var->i++;
		else if (env_var->str[env_var->i] == '$' && env_var->is_value_expansion)
			handle_dollar_sign(env_var);
		else
			env_var->res[env_var->j++] = env_var->str[env_var->i++];
	}
	env_var->res[env_var->j] = '\0';
}

void	replace_env_vars(const char *str, char *res, t_info *info)
{
	t_env_var	env_var;

	init_env_var(str, res, &env_var, info);
	if (env_var.str[0] == '$')
	{
		handle_dollar_sign(&env_var);
		if (env_var.res)
			env_var.res[env_var.j] = '\0';
		return ;
	}
	remove_outer_double_quotes(&env_var);
	remove_outer_single_quotes(&env_var);
	process_str(&env_var);
}
