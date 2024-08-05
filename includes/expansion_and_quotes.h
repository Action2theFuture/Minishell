/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_and_quotes.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:19:39 by junsan            #+#    #+#             */
/*   Updated: 2024/08/05 18:23:32 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_AND_QUOTES_H
# define EXPANSION_AND_QUOTES_H

# include "minishell_types.h"

// var_expansion_with_args.c
void					expand_and_strip_quotes_in_args(\
		char **args, t_info *info);

// replace_env_vars.c
void					replace_env_vars(\
		const char *str, char *res, t_info *info);

// replace_env_vars_utils.c
void					pass_double_quotes(t_env_var *env_var);
void					extract_var_name(\
		const char *str, size_t *i, char *var_name);
char					*process_replace_env_vars(\
		char *arg, t_info *info, bool is_value_expansion);
char					*process_replace_expansion_var(t_info *info);

// replace_env_vars_utils_2.c
void					handle_expansion_var_without_quotes(\
					t_handler_info *h_info, char c);
void					handle_normal_var_without_quotes(\
					t_handler_info *h_info);
void					extract_var_name_from_input(\
					t_handler_info *h_info, const char *input);
bool					handle_quotes_from_input(\
					t_handler_info *h_info, const char *input);

// expand_wildcard.c
bool					expand_wildcard(char ***args);

// expand_wildcard_utils.c
char					**reallocate_matches(\
		char **matches, int *capacity, int required_capacity);
bool					is_path_visited(\
		const char *path, t_visited_paths *visited);
void					mark_path_visited(\
		const char *path, t_visited_paths *visited);
bool					match_pattern(const char *pattern, const char *str);
void					clear_visited_paths(t_visited_paths *visited);
// expand_wildcard_utils_2.c
void					clear_expand_info(t_expand_info *head);

// process_expand_strip_quotes.c
char					*process_expand_strip_quotes(\
		char *new_str, const char *input, t_info *info);

// handle_replace_env_vars.c
void					handle_dollar_sign(t_env_var *env_var);

// handle_replace_env_vars_without_quotes.c
char					*handler_dollar_sign_wihout_quotes(\
		char *new_str, char *input, t_info *info);

// handle_under_score.c
void					fetch_last_arg(char **chunk, t_info *info);

// init_utils_for_expansion.c
void					init_env_var(\
	const char *str, char *res, t_env_var *env_var, t_info *info);
void					init_handler_info(\
			t_handler_info *h_info, char *new_str, t_info *info);
void					init_visited_paths(t_visited_paths *visited);
void					free_visited_paths(t_visited_paths *visited);
t_expand_info			*new_expand_info(void);

// init_utils_for_expansion_2.c
void					clear_expand_info(t_expand_info *head);

// normalize_spaces.c
char					*normalize_spaces(const char *str);

#endif // EXPANSION_AND_QUOTES_H
