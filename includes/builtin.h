/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:14:06 by junsan            #+#    #+#             */
/*   Updated: 2024/07/12 13:38:44 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell_types.h"
// built_in.c
void					init_builtin(int (*func[])(const char *, const char **,
								t_env *));
int						handler_builtin(const char *cmd);

// file_dir_operations.c
int						change_dir(const char *path, t_env *lst);
bool					get_cur_dir(void);
bool					file_exist(const char *filename);
void					list_dir(const char *dirname);

// ft_cd.c
void					ist_swap(t_env *a);
int						ft_cd(const char *cmd, const char **args, t_env *list);

// ft_echo.c
int						ft_echo(const char *cmd, const char **args,
							t_env *list);

// ft_env.c
void					add_builtin_node(t_env **head, char *name,
							char *content);
void					printf_env(t_env *list);
int						ft_env(const char *cmd, const char **args, t_env *list);

// ft_exit.c
int						ft_exit(const char *cmd, const char **args,
							t_env *list);

// ft_export.c
int						ft_export(\
		const char *cmd, const char **args, t_env *list);

// ft_export_utils.c
int						check_first_arg(const char arg);
int						ft_strcmp(char *s1, char *s2);
t_env					*find_next_lower(t_env *lst, char *prec);
t_env					*find_lowest(t_env *lst);
bool					change_var_if_exist(\
		const char *name, const char *content, t_env *lst);

// ft_export_utils_2.c
t_env					*builtin_new_node(char *name, char *content);
void					add_builtin_node(\
		t_env **head, char *name, char *content);

// ft_pwd.c
int						ft_pwd(const char *cmd, const char **args, t_env *list);

// ft_unset.c
int						ft_unset(const char *cmd, const char **args,
							t_env *list);

// subshell_utils.c
char					*remove_nested_subshell(t_token **token);
#endif // BUILTIN_H
