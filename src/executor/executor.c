/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2026/03/02 17:53:07 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 17:53:07 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	piece_cmd_exec(t_ms *shell, int *fd)
{
	char	*command_path;

	set_signals_child();
	close(fd[0]);
	close(shell->initial_stdout);
	close(shell->initial_stdin);
	if (get_env_val("PATH", shell) && shell->cmd_list->next)
		dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	if (shell->cmd_list->redirs && apply_redirects(shell) < 0)
		exit(EXIT_FAILURE);
	if (is_builtin(shell->cmd_list->args[0]))
	{
		call_builtins(shell);
		exit(shell->last_status);
	}
	else
	{
		command_path = get_full_cmd_path(shell);
		execve(command_path, shell->cmd_list->args, shell->envs);
		free(command_path);
		shell->last_status = EXIT_FAILURE;
		exit(shell->last_status);
	}
}

static pid_t	multi_cmd_exec(t_ms *shell)
{
	int		fd[2];
	pid_t	child_id;

	while (shell->cmd_list)
	{
		expander(shell);
		if (shell->cmd_list->next && pipe(fd) < 0)
			perror("pipe");
		child_id = fork();
		if (child_id == 0)
			piece_cmd_exec(shell, fd);
		if (shell->last_status != 0)
		{
			close(fd[1]);
			break ;
		}
		if (shell->cmd_list->next)
		{
			dup2(fd[0], STDIN_FILENO);
			close(fd[1]);
		}
		shell->cmd_list = shell->cmd_list->next;
	}
	close(fd[0]);
	return (child_id);
}

static void	single_cmd_exec(t_ms *shell)
{
	char	*command_path;

	expander(shell);
	if (shell->cmd_list->redirs && apply_redirects(shell) < 0)
	{
		if (g_sigint)
		{
			g_sigint = 0;
			printf("> \n");
			shell->last_status = 130;
			return ;
		}
		exit(EXIT_FAILURE);
	}
	if (is_builtin(shell->cmd_list->args[0]))
		call_builtins(shell);
	else
	{
		command_path = get_full_cmd_path(shell);
		call_path(shell, command_path);
		free(command_path);
	}
}

void	executor(t_ms *shell)
{
	pid_t	child_pid;
	t_cmd	*first;

	first = shell->cmd_list;
	shell->initial_stdout = dup(STDOUT_FILENO);
	shell->initial_stdin = dup(STDIN_FILENO);
	if (shell->cmd_list->next)
	{
		set_signals_exec();
		child_pid = multi_cmd_exec(shell);
		shell->cmd_list = first;
		await_results(shell, child_pid);
		set_signals();
	}
	else
		single_cmd_exec(shell);
	dup2(shell->initial_stdout, STDOUT_FILENO);
	close(shell->initial_stdout);
	dup2(shell->initial_stdin, STDIN_FILENO);
	close(shell->initial_stdin);
}
