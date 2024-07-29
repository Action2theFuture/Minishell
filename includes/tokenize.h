/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:11:43 by junsan            #+#    #+#             */
/*   Updated: 2024/07/29 09:19:34 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell_types.h"

// tokenize.c
int						tokenize(const char *input, t_token **tokens);

// tokenize_utils.c
t_token_list			*get_token_list(t_token *token);
void					add_token(t_token **head, const char *start,
							size_t len);

// handle_quotes.c
int						handle_quotes(const char **input,
							const char **start, t_token **list);

// handle_subshell.c
int						handle_open_subshell(const char **input, int *depth,
							const char **start, t_token **list);
int						handle_close_subshell(const char **input, int *depth,
							const char **start, t_token **list);
int						add_depth_token(\
						const char **input, int *depth, t_token **tokens);

// handle_operators_and_spaces.c
void					handle_operators_and_spaces(const char **input,
							const char **start, t_token **list);
// tokenize_utlls_2.c
t_token					*tokens_last(t_token *tokens);
void					free_token(t_token *head);
size_t					tokens_size(t_token *head);

// ---------------------- validation for token ------------------//
// valid_token.c
int						valid_token(t_token *head);

// valid_token_utlls.c
bool					check_quotes_in_tokens(t_token *head);
bool					check_subshell_closed(t_token *head);

// valid_token_err.c
bool					check_redir_err(t_token *head);
bool					check_operator_before_after_err(t_token *head);
bool					check_subshell_err(t_token *head);

// valid_token_err_2.c
bool					check_logical_err(t_token *head);
bool					is_cmd_valid(const char *cmd);

#endif // TOKENIZE_H
