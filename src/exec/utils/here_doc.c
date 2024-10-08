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
	while (env)
	{
		if (ft_strncmp(env->name, key, ft_strlen(key)) == 0)
			return (env->content);
		env = env->next;
	}
	return (NULL);
}

static void	handle_variable_replacement(\
		const char **str, char *res, const char **start, t_info *info)
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
	var_start = *str;
	while (**str && (ft_isalnum(**str) || **str == '-'))
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
	size_t		len;
	char		*res;
	char		tmp[MAX_ARGS];

	start = str;
	len = ft_strlen(str);
	res = (char *)malloc(MAX_ARGS);
	if (!res)
		return (perror("malloc error"), NULL);
	res[0] = '\0';
	while (*str)
	{
		if (*str == '$')
		{
			if (ft_strlen(str) == 2 && *(str + 1) == '?')
				return (free(res), ft_itoa(info->exit_status));
			handle_variable_replacement(&str, res, &start, info);
		}
		else
			str++;
	}
	return (ft_strlcpy(tmp, start, str - start + 1), \
		ft_strlcat(res, tmp, len * 2 + 1), res);
}

// limiter -> "EOF" - not Expansion env vars
// limiter -> EOF - Expansion env vars
static int	process_line_and_write(\
		int infile, char *line, char *limiter, t_info *info)
{
	char	*env_var;

	env_var = NULL;
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 && \
		ft_strlen(line) == ft_strlen(limiter))
		return (free(line), line = NULL, 0);
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
	char	*line;
	char	*limiter_without_quotes;

	set_heredoc_signal_handler();
	g_heredoc_interrupted = 0;
	limiter_without_quotes = remove_shell_quotes(limiter);
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL || g_heredoc_interrupted || \
			!process_line_and_write(infile, line, limiter_without_quotes, info))
			break ;
		(free(line), line = NULL);
	}
	(free(limiter_without_quotes), set_signal_handler(SIGNAL_HANDLER));
	if (g_heredoc_interrupted)
		return (ft_putstr_fd("^C\n", STDERR_FILENO), rl_redisplay(), \
		free(line), cleanup_tmp_file(), FAILURE);
	if (infile != -1)
		close(infile);
	info->stdin_fd = open(HEREDOC_TMP, O_RDONLY, 0644);
	if (info->stdin_fd == -1)
		return (perror("heredoc open error"), FAILURE);
	return (SUCCESS);
}
