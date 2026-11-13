/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:40:54 by junsan            #+#    #+#             */
/*   Updated: 2024/08/22 19:28:59 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_heredoc_interrupted;

static char	*get_env_value(t_env *env, const char *key)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strlen(env->name) == key_len && ft_strncmp(env->name, key,
				key_len) == 0)
			return (env->content);
		env = env->next;
	}
	return (NULL);
}

static void	append_exit_status(const char **str, char *res, t_info *info)
{
	char	*status_str;

	(*str)++;
	status_str = ft_itoa(info->exit_status);
	if (status_str)
	{
		ft_strlcat(res, status_str, MAX_ARGS);
		free(status_str);
	}
}

static void	handle_variable_replacement(const char **str, char *res,
		const char **start, t_info *info)
{
	const char	*var_start;
	char		tmp[MAX_ARGS];
	char		var_name[MAX_ARGS];
	char		*var_value;
	size_t		tmp_len;

	tmp_len = *str - *start;
	ft_strlcpy(tmp, *start, tmp_len + 1);
	ft_strlcat(res, tmp, MAX_ARGS);
	(*str)++;
	if (**str == '?')
	{
		append_exit_status(str, res, info);
		*start = *str;
		return ;
	}
	var_start = *str;
	while (**str && (ft_isalnum(**str) || **str == '_'))
		(*str)++;
	tmp_len = *str - var_start;
	ft_strlcpy(var_name, var_start, tmp_len + 1);
	var_value = get_env_value(info->env, var_name);
	if (var_value)
		ft_strlcat(res, var_value, MAX_ARGS);
	*start = *str;
}

static char	*replace_env_vars_from_here_doc(const char *str, t_info *info)
{
	const char	*start;
	char		*res;
	char		tmp[MAX_ARGS];

	start = str;
	res = (char *)malloc(MAX_ARGS);
	if (!res)
		return (perror("malloc error"), NULL);
	res[0] = '\0';
	while (*str)
	{
		if (*str == '$' && (*(str + 1) == '?' || ft_isalnum(*(str + 1)) || *(str
					+ 1) == '_'))
			handle_variable_replacement(&str, res, &start, info);
		else
			str++;
	}
	ft_strlcpy(tmp, start, str - start + 1);
	ft_strlcat(res, tmp, MAX_ARGS);
	return (res);
}

static int	process_line_and_write(int infile, char *line, char *limiter,
		t_info *info)
{
	char	*env_var;

	env_var = NULL;
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
		&& ft_strlen(line) == ft_strlen(limiter))
		return (0);
	if (info->is_expansion_heredoc)
		env_var = replace_env_vars_from_here_doc(line, info);
	if (env_var)
	{
		write(infile, env_var, ft_strlen(env_var));
		free(env_var);
	}
	else
		write(infile, line, ft_strlen(line));
	write(infile, "\n", 1);
	return (1);
}

int	here_doc(int infile, char *limiter, t_info *info)
{
	char *line;
	char *limiter_without_quotes;

	set_heredoc_signal_handler();
	g_heredoc_interrupted = 0;
	limiter_without_quotes = remove_shell_quotes(limiter);
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL || g_heredoc_interrupted
			|| !process_line_and_write(infile, line, limiter_without_quotes,
				info))
		{
			free(line);
			break ;
		}
		free(line);
	}
	free(limiter_without_quotes);
	set_signal_handler(SIGNAL_HANDLER);
	if (g_heredoc_interrupted)
	{
		if (infile != -1)
		{
			close(infile);
			info->stdin_fd = -1;
		}
		dup2(info->origin_stdin_fd, STDIN_FILENO); // 닫았던 STDIN 즉시 복구
		cleanup_tmp_file();
		info->exit_status = 130; // Ctrl+C 종료 상태 코드
		return (130);            // 또는 FAILURE 처리 후 상위에서 130 반영
	}
	if (infile != -1)
		close(infile);
	info->stdin_fd = open(HEREDOC_TMP, O_RDONLY, 0644);
	if (info->stdin_fd == -1)
		return (perror("heredoc open error"), FAILURE);
	return (SUCCESS);
}