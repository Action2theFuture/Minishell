/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:27:16 by junsan            #+#    #+#             */
/*   Updated: 2024/08/01 09:15:43 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell_types.h"

// string_utils.c
bool					ft_isspace(char c);
bool					is_all_whitespace(const char *str);
void					remove_control_characters(char *str);
int						count_repeated_chars(const char *str, int c);

// string_utils_2.c
char					*trim_first_last(char *str);
char					*trim_whitespace(const char *str);
char					*ft_strndup(const char *str, size_t n);
bool					is_operator(const char *cmd);
bool					is_special_char(char c);

// normalize_spaces.c
char					*normalize_spaces(const char *str);

// quotes_str.c
void					remove_quotes(char *str);
void					remove_surrounding_single_quotes(char *str);
void					remove_surrounding_double_quotes(char *str);
void					remove_empty_quotes(char *str);
void					remove_double_quotes_inside_single_quotes(char *str);

// subshell_and_quote_str.c
bool					are_parentheses_balanced(const char *input);

// error_utils.c
int						fd_log_error(char *cmd, char *arg, char *error);
int						execve_log_error(char *cmd, int error);
void					parse_log_error(int err, int *exit_status);

#endif // UTILS_H
