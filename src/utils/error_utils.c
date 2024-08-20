/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 19:45:39 by junsan            #+#    #+#             */
/*   Updated: 2024/08/20 08:36:52 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fd_log_error(char *cmd, char *arg, char *error)
{
	ft_putstr_fd("minishell$> ", STDERR_FILENO);
	if (cmd != NULL)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg != NULL)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (error != NULL)
		ft_putstr_fd(error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (FAILURE);
}

int	execve_log_error(char *cmd, int error)
{
	ft_putstr_fd("minishell$> ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (ft_strlen(cmd) == 1 && ft_strncmp(cmd, ".", 1) == 0)
	{
		ft_putstr_fd("filename argument required\n", STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
	}
	else if (error == EACCES)
		ft_putstr_fd("Permission denied.\n", STDERR_FILENO);
	else if (error == EFAULT)
		ft_putstr_fd("command not found\n", STDERR_FILENO);
	else if (error == ENOENT)
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
	else
	{
		ft_putstr_fd("An unknown error occurred ", STDERR_FILENO);
		ft_putstr_fd("errno = ", STDERR_FILENO);
		ft_putnbr_fd(error, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (FAILURE);
}

void	parse_log_error(int err, int *exit_status)
{
	if (err == UNCLOSED_QUOTE)
	{
		*exit_status = 258;
		ft_putstr_fd("minishell$> ", STDERR_FILENO);
		ft_putstr_fd("Unclosed quote\n", STDERR_FILENO);
	}
	else if (err == SYNTAX_ERROR)
	{
		*exit_status = 2;
		ft_putstr_fd("minishell$> ", STDERR_FILENO);
		ft_putstr_fd("syntax error unexpected token\n", STDERR_FILENO);
	}
	else if (err == TOKEN_ERROR)
	{
		*exit_status = 258;
		ft_putstr_fd("minishell$> ", STDERR_FILENO);
		ft_putstr_fd("error near unexpected token\n", STDERR_FILENO);
	}
	else if (err == PARSE_ERROR)
		*exit_status = 1;
}
