/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:14:06 by junsan            #+#    #+#             */
/*   Updated: 2024/08/04 13:35:53 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell_types.h"
// built_in.c
void					init_builtin(int (*func[])(const char *, const char **,
								t_env *));
int						handler_builtin(const char *cmd);

// ft_cd.c
void					ist_swap(t_env *a);
int						ft_cd(const char *cmd, const char **args, t_env *list);

// cd_utlls.c
void					swap_pwd_oldpwd(t_env *env);
void					update_pwd_oldpwd(t_env *env, const char *new_pwd);
int						chdir_to_home(t_env *env);
int						init_oldpwd_node(t_env *lst);
int						change_dir(const char *path, t_env *env);

// handle_pwd_oldpwd.c
void					swap_pwd_oldpwd(t_env *env);
void					update_pwd_oldpwd(t_env *env, const char *new_pwd);
void					init_pwd_oldpwd_nodes(\
		t_env *head, t_env *old_pwd, t_env *pwd);
int						init_oldpwd_node(t_env *lst);
void					add_pwd_oldpwd(\
		t_env *head, const char *name, const char *content);

// ft_echo.c
int						ft_echo(const char *cmd, const char **args,
							t_env *list);

// ft_env.c
void					printf_env(t_env *list);
int						ft_env(const char *cmd, const char **args, t_env *list);

// ft_exit.c
int						ft_exit(t_info *info);

// ft_export.c
int						ft_export(\
		const char *cmd, const char **args, t_env *list);

// ft_export_utils.c
int						ft_strcmp(char *s1, char *s2);
bool					var_if_exist(const char *name, t_env *lst);
bool					is_valid_name(const char *name);
bool					strip_trailing_plus(char *str);

// builtin_node_utils.c
t_env					*builtin_new_node(char *name, char *content);
void					add_builtin_node(\
		t_env *head, char *name, char *content);
void					update_builtin_node(\
		t_env *head, char *name, char *content);
void					append_builtin_node(\
		t_env *head, char *name, char *content);

// ft_pwd.c
int						ft_pwd(const char *cmd, const char **args, t_env *list);

// ft_unset.c
int						ft_unset(const char *cmd, const char **args,
							t_env *list);
#endif // BUILTIN_H
