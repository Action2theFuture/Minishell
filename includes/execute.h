/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:12:49 by junsan            #+#    #+#             */
/*   Updated: 2024/08/07 12:17:55 by junsan           ###   ########.fr       */
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
void					process_phrase_node(t_ast *node, t_info *info);

// process_execute_pipe.c
void					process_pipe_node(t_ast *pipe_node, t_info *info);

// init_info.c
void					init_info(t_info *info, t_env *env, t_ast *root);

// info_utils.c
void					clear_info(t_info *info);
void					reset_consts_fd(t_info *info);

// args_utils.c
int						count_strings(char **arr);
char					**allocate_null_and_cmd_chunk(char **cmd, int cmd_cnt);
char					**copy_str_arr(char **src);
void					free_args(char **args);
void					replace_env_vars_in_args(char **args, t_info *info);

// quotes_utils.c
void					remove_consecutive_double_quotes_from_args(char **args);
void					remove_double_quotes_from_args(char **args);
void					remove_single_quotes_from_args(char **args);
char					*remove_all_quotes(const char *input);

// add_string_to_array.c
char					**add_string_to_array(\
		int *size, const char *str);
char					*remove_shell_quotes(const char *str);

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
void					close_tmp_pipe(int *tmp_pipe);

// redirect_to_null.c
int						redirect_input_to_null(void);
int						redirect_input_to_empty(void);
int						redirect_output_to_null(void);
int						redirect_stdin_to_empty(int *stdin_fd);

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
char					**merge_args_and_add_spaces(\
				char **cmd, char **args, int cmd_cnt);

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
void					cleanup_and_exit(\
		int status, char **args, char **env, t_info *info);

// redir.c
int						handle_io_redirection(t_ast *node, t_info *info);

// process_redir.c

int						here_doc_redir(char *limiter, t_info *info);
int						input_redir(char *arg, t_ast *node, t_info *info);
int						output_redir(char *arg, t_ast *node, t_info *info);
int						handle_ft_redirection(\
				char *arg, t_ast *node, t_info *info);

// redir_utils.c
int						open_file_with_mode(char *file, int mode);
void					close_fds(t_info *info);
void					cleanup_tmp_file(void);
void					process_quotes_in_arg(char *str);

// here_doc.c
int						here_doc(int infile, char *limiter, t_info *info);

// subshell.c
int						process_subshell_node(t_ast *node, t_info *info);

#endif // EXECUTE_H
