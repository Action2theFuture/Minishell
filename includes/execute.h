/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:12:49 by junsan            #+#    #+#             */
/*   Updated: 2024/07/31 17:03:04 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell_types.h"
// execute.c
void					execute(t_ast *root, t_env *env, int *exit_status);
void					traverse_tree(t_ast *node, t_info *info);
void					categorize_tree(t_ast *node, t_info *info);

// process_execute.c
void					process_logical_node(t_ast *node, t_info *info);
void					process_pipe_node(t_ast *pipe_node, t_info *info);
void					process_phrase_node(t_ast *node, t_info *info);

// info_utils.c
void					init_info(t_info *info, t_env *env);
void					clear_info(t_info *info);
void					reset_consts_fd(t_info *info);

// args_utils.c
void					free_args(char **args);
void					replace_env_vars_in_args(char **args, t_info *info);
char					**allocate_null_and_cmd_chunk(char **cmd, int cmd_cnt);

// quotes_utils.c
void					remove_consecutive_double_quotes_from_args(char **args);
void					remove_double_quotes_from_args(char **args);
void					remove_single_quotes_from_args(char **args);
bool					find_unquoted_dollar(const char *input);

// var_expansion_with_args.c
void					expand_and_strip_quotes_in_args(\
		char **args, t_info *info);
char					*process_replace_env_vars(char *arg, t_info *info);
char					*process_replace_expansion_var(t_info *info);

// process_expand_strip_quotes.c
char					*process_expand_strip_quotes(\
		char *new_str, const char *input, t_info *info);

// replace_env_vars.c
void					replace_env_vars(\
		const char *str, char *res, t_info *info);

// replace_env_vars_utils.c
void					pass_double_quotes(t_env_var *env_var);
void					extract_var_name(\
		const char *str, size_t *i, char *var_name);
char					*process_replace_env_vars(char *arg, t_info *info);
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

// init_utils_for_expansion.c
void					init_env_var(\
	const char *str, char *res, t_env_var *env_var, t_info *info);
void					init_handler_info(\
			t_handler_info *h_info, char *new_str, t_info *info);
void					init_visited_paths(t_visited_paths *visited);
void					free_visited_paths(t_visited_paths *visited);
t_expand_info			*new_expand_info(void);

// handle_replace_env_vars.c
void					handle_dollar_sign(t_env_var *env_var);

// handle_replace_env_vars_without_quotes.c
char					*handler_dollar_sign_wihout_quotes(\
		char *new_str, char *input, t_info *info);

// handle_under_score.c
void					fetch_last_arg(char **chunk, t_info *info);

// get_path_type.c
t_path_type				get_path_type(const char *path, t_info *info);

// find_cmd_in_path.c
char					*find_cmd_in_path(const char *cmd, t_env *env);

// get_absolute_path.c
char					*get_absolute_path(const char *path);

// get_bin_path.c
char					*get_bin_path(const char *cmd);

// fd_utils.c
int						backup_stdio(t_info *info);
int						restore_stdio(t_info *info);
int						backup_fds(t_info *info);
int						restore_fds(t_info *info);
int						redirect_to_null(t_info *info);

// get_file_list.c
void					free_file_list(t_file_list *file_list);
const char				*get_path(const char *full_path);
t_file_list				*get_file_list(const char *path);

// get_file_list_utils.c
DIR						*get_dir(const char *path, int file_count,
							t_file_list **file_list);
t_file_list				*get_entry_list(t_file_list *file_list, DIR *dir);

// list_to_array.c
char					**list_to_array(t_env *env);
void					clear_env_arr(char **arr);

// dispatch_cmd.c
int						dispatch_cmd(t_ast *node, t_info *info);

// launch_process_cmd.c
int						launch_process_cmd(\
				char *cmd, char **args, t_info *info);

// launch_process_pipe.c
int						launch_process_pipe(\
				char *cmd, char **args, t_info *info);
int						wait_for_child_task(t_info *info);

// execution_pipeline.c
void					first_pipe(\
		char *cmd, char **env, char **args, t_info *info);
void					middle_pipe(\
		char *cmd, char **env, char **args, t_info *info);
void					last_pipe(\
		char *cmd, char **env, char **args, t_info *info);

// redir.c
int						handle_io_redirection(t_ast *node, t_info *info);

// redir_utils.c
int						open_file_with_mode(char *file, int mode);
void					cleanup_tmp_file(void);
void					process_quotes_in_arg(char *str);

// here_doc.c
int						here_doc(int infile, char *limiter, t_info *info);

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

// subshell.c
int						process_subshell_node(t_ast *node, t_info *info);
#endif // EXECUTE_H
