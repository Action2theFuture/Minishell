/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_types.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:25:12 by junsan            #+#    #+#             */
/*   Updated: 2024/08/13 11:53:35 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

# define MAX_PATH_LENGTH 4096
# define MAX_BUFFER_SIZE 1024
# define MAX_ARGS 4096
# define MEMORY_CAPACITY 256
# define HISTSIZE 500
# define MAX_PIPES 64
# define MAX_RECURSION_DEPTH 1
# define INITIAL_CAPACITY 100
# define DELIMS "|&<>"
# define SHELL_METACHARS "\"\'()&|<>"
# define SHELL_OPERATORS "()&|<>"
# define ARR_SEP '`'
# define ASCII_ART_PATH "assets/ascii_art_colossal"
# define HEREDOC_TMP "heredoc_tmp"
# define DEV_NULL_PATH "/dev/null"
# define BASE_PATH '.'
# define LOC_MINISHELL "/./minishell"

extern volatile sig_atomic_t	g_heredoc_interrupted;

typedef enum fd_type
{
	READ_END = 0,
	WRITE_END = 1,
}	t_fd_type;

typedef enum signal_type
{
	SIGNAL_HANDLER,
	IGN,
	DFL,
}	t_signal_type;

typedef enum pipe_loc
{
	FIRST,
	MIDDLE,
	LAST,
}	t_pipe_loc;

typedef enum tree_direction
{
	LEFT,
	RIGHT,
}						t_tree_direction;

typedef enum subshell_type
{
	OPEN,
	CLOSE,
	SINGLE,
	NESTED,
}	t_subshell_type;

typedef enum type_dir
{
	IS_DIRECTORY = 126,
	CMD_NOT_FOUND = 127,
}						t_type_dir;

typedef enum type_descriptor
{
	READ,
	WRITE,
	APPEND,
}						t_type_descriptor;

typedef enum status
{
	SUCCESS = 0,
	FAILURE = 1,
	MALLOC_ERR,
}						t_status;

typedef enum type_logical
{
	AND,
	OR,
}						t_type_logical;

typedef enum type
{
	ROOT = -1,
	SUBSHELL = 100,
	NESTED_SUBSHELL = 101,
	ARGS = 22,
	CMD = 20,
	FILE_NAME = 18,
	IO = 6,
	REDIRECTION = 5,
	PIPE = 3,
	LOGICAL = 1,
	PHRASE = 0,
	IN_REDIR = 7,
	IN_HEREDOC = 8,
	IN_HERESTR = 9,
	OUT_REDIR = 10,
	OUT_APPEND = 11,
	NOT_REDIR = 12,
}						t_type;

typedef enum error
{
	UNCLOSED_QUOTE,
	TOKEN_ERROR,
	PARSE_ERROR,
	SYNTAX_ERROR,
}						t_error;

typedef enum path_type
{
	PATH_ABSOLUTE,
	PATH_RELATIVE,
	PATH_COMMAND,
	PATH_INVALID
}						t_path_type;

typedef enum built_in
{
	M_ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	NONE,
}						t_built_in;

typedef struct s_env
{
	char				*name;
	char				*content;
	struct s_env		*pwd;
	struct s_env		*old_pwd;
	struct s_env		*next;
	struct s_env		*last_arg;
}						t_env;

typedef struct s_quote_info
{
	bool	in_single_quotes;
	bool	in_double_quotes;
	size_t	new_str_len;
	size_t	tmp_str_len;
	char	tmp_str[MAX_ARGS];
}	t_quote_info;

typedef struct s_info
{
	pid_t				pid;
	bool				is_pipe;
	bool				is_re_pipe;
	bool				is_heredoc;
	bool				is_expansion_heredoc;
	bool				has_multiple_pipes;
	bool				pipe_exists;
	bool				in_subshell;
	char				**redir_args;
	char				*path;
	char				**args;
	int					child_pids[MAX_PIPES];
	int					stdin_fd;
	int					stdout_fd;
	int					backup_stdin;
	int					backup_stdout;
	int					origin_stdin_fd;
	int					origin_stdout_fd;
	int					stdin_backup;
	int					stdout_backup;
	int					pipe[2];
	int					tmp_pipe[3];
	int					pipe_loc;
	int					stdin_pipe;
	int					tmp_fd;
	int					exit_status;
	int					status;
	struct s_ast		*root;
	struct s_token		*token;
	struct s_env		*env;
	struct s_quote_info	*quote_info;
}						t_info;

typedef struct s_handler_info
{
	char	*new_str;
	size_t	new_str_len;
	size_t	var_len;
	int		i;
	bool	in_single_quotes;
	bool	in_double_quotes;
	t_info	*info;
	char	var[MAX_ARGS];
}	t_handler_info;

typedef struct s_token
{
	int					num;
	char				*data;
	t_type				type;
	struct s_token		*next;
}						t_token;

typedef struct s_token_list
{
	t_token				*head;
	t_token				*tail;
}						t_token_list;

typedef struct s_ast
{
	char				*data;
	t_type				type;
	t_token				*token;
	struct s_ast		*parent;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

typedef struct s_cmd_list
{
	char				*cmd;
	struct s_cmd_list	*prev;
	struct s_cmd_list	*next;
}						t_cmd_list;

typedef struct s_file_list
{
	char				**names;
	size_t				count;
}						t_file_list;

typedef struct s_env_var
{
	const char	*str;
	bool		is_value_expansion;
	bool		in_double_quotes;
	char		*res;
	t_info		*info;
	size_t		i;
	size_t		j;
}	t_env_var;

typedef struct s_visited_paths
{
	char	**paths;
	int		count;
	int		capacity;
}	t_visited_paths;

typedef struct s_expand_info
{
	char					**matches;
	int						cnt;
	int						capacity;
	struct s_expand_info	*next;
}	t_expand_info;
#endif // MINISHELL_TYPES_H
