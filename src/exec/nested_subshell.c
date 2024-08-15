/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nested_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 16:31:43 by junsan            #+#    #+#             */
/*   Updated: 2024/08/15 16:39:52 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_process(\
		t_info *info, t_info *subshell_info, t_ast *subshell_node)
{
	if (info->token)
		(free_token(info->token), info->token = NULL);
	if (info->nested_subshell_root)
		(free_tree(info->nested_subshell_root), \
		info->nested_subshell_root = NULL);
	if (backup_stdio(info) == FAILURE)
		fd_log_error(NULL, NULL, strerror(errno));
	set_signal_handler(DFL);
	traverse_tree_in_subshell(subshell_node, subshell_info);
	if (restore_stdio(info) == FAILURE)
		fd_log_error(NULL, NULL, strerror(errno));
	free_tree(subshell_info->nested_subshell_root);
	free_token(subshell_info->token);
	info->exit_status = subshell_info->exit_status;
	cleanup_and_exit(info->exit_status, NULL, NULL, info);
}

static int	process_nested_subshell_node(\
			t_ast *nested_subshell_node, t_token *token, \
			t_info *subshell_info, t_info *info)
{
	pid_t	pid;
	t_ast	*subshell_node;

	subshell_node = nested_subshell_node->left;
	subshell_info->nested_subshell_root = nested_subshell_node;
	subshell_info->token = token;
	pid = fork();
	if (pid == -1)
		return (fd_log_error("fork_error", NULL, NULL));
	if (pid == 0)
		handle_child_process(info, subshell_info, subshell_node);
	else
		wait_for_child_task(info);
	cleanup_tmp_file();
	return (info->exit_status);
}

void	start_nested_subshell_node(t_ast *node, t_info *info)
{
	t_info	subshell_info;
	t_token	*tokens_in_subshell;
	t_token	*token_head;
	t_ast	*nested_subshell_node;

	tokens_in_subshell = NULL;
	nested_subshell_node = NULL;
	tokenize(node->data, &tokens_in_subshell);
	token_head = tokens_in_subshell;
	if (!parse_subshell(&tokens_in_subshell, &nested_subshell_node))
	{
		free_token(tokens_in_subshell);
		return ;
	}
	init_info(&subshell_info, info->env, info->root);
	info->exit_status = process_nested_subshell_node(\
			nested_subshell_node, token_head, &subshell_info, info);
	free_token(subshell_info.token);
	free_tree(subshell_info.nested_subshell_root);
	clear_info(&subshell_info);
}
