/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:09:24 by junsan            #+#    #+#             */
/*   Updated: 2024/07/29 18:26:57 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell_types.h"

// parsing_utils.c
void					free_tree(t_ast *node);
t_ast					*new_node(const char *data, t_type type);
t_ast					*attach_to_tree(t_ast *root, t_ast *node, int side);
void					remove_outer_parentheses(char **str, t_ast **root);

// parsing_utils_2.c
void					add_parent(t_ast *node, t_ast *left, t_ast *right);
char					*append_with_semicolon(char *original, char *to_append);
t_ast					*attach_to_tree_side(\
					t_ast *root, t_ast *node, int side);

// parsing.c
bool					parsing_tree(t_token_list **tokens, t_ast **root);

// parse_logical.c
bool					parse_logical(t_token **token, t_ast **node);

// parse_pipe.c
bool					parse_pipe(t_token **token, t_ast **node);

// parse_phrase.c
bool					parse_phrase(t_token **token, t_ast **node);
bool					parse_phrase_part(\
		t_token **token, t_ast **node, \
		bool (*parse_func)(t_token **, t_ast **, t_ast **));

// parse_phrase_part.c
bool					parse_cmd(t_token **token, t_ast **node);
bool					parse_redirection_part(\
		t_token **token, t_ast **phrase_node, t_ast **node);
bool					parse_cmd_part(\
		t_token **token, t_ast **phrase_node, t_ast **node);

// parse_redirection.c
bool					parse_redirection(t_token **token, t_ast **node);
bool					parse_io_redirection(t_token **token, t_ast **node);

// parse_subshell.c
bool					parse_subshell(t_token **token, t_ast **node);

// parsing_quotes_in_cmd.c
char					*remove_all_quotes(const char *input);
char					**parse_cmd_line_with_quotes(\
		const char *input, int *cnt);

// type_functions.c
bool					is_logical_operator(const char *token);
bool					is_pipe_operator(const char *token);
bool					is_subshell_operator(const char *token);
bool					is_redirection_operator(const char *token);
bool					is_file_name(const char *token);

// subshell_utils.c
char					*remove_nested_subshell(t_token **token);

// collect_data_until_subshell.c
void					ensure_capacity(\
		char **data_in_subshell, size_t *capacity, size_t space_needed);
char					*collect_data_until_subshell(t_token **token);

// type_redir_functions.c
bool					is_input_redirection(const char *data);
bool					is_output_redirection(const char *data);
bool					is_append_redirection(const char *data);
bool					is_heredoc_redirection(const char *data);
bool					is_herestr_redirection(const char *data);

// arg_parse.c
char					*arg_parsing(t_token **token);

// get_type.c
t_type					get_type(const char *data);
t_type					get_type_redir(const char *data);

#endif // PARSING_H
