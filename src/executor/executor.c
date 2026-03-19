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

static void	piece_cmd_exec(t_ms *shell)
{
	char	**path_dirs;
	char	*command_path;

	if (shell->cmd_list->redirs && apply_redirects(shell) < 0)
		exit(EXIT_FAILURE);
	if (is_builtin(shell->cmd_list->args[0]))
	{
		call_builtins(shell);
		exit(shell->last_status);
	}
	else
	{
		path_dirs = get_path_dirs(shell);
		command_path = get_full_command_path(shell->cmd_list->args[0],
				path_dirs);
		free_matrix(path_dirs);
		execve(command_path, shell->cmd_list->args, shell->envs);
		free(command_path);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

static pid_t	multi_cmd_exec(t_ms *shell)
{
	int		fd[2];
	pid_t	child_id;

	while (shell->cmd_list)
	{
		if (shell->cmd_list->next && pipe(fd) < 0)
			perror("pipe");
		child_id = fork();
		if (child_id == 0)
		{
			close(fd[0]);
			if (shell->cmd_list->next)
				dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			piece_cmd_exec(shell);
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
	char	**path_dirs;
	char	*command_path;

	if (shell->cmd_list->redirs && apply_redirects(shell) < 0)
	{
		if (sigint) {
      sigint = 0;
			printf("> \n");
			shell->last_status = 130;
      return;
    }
		exit(EXIT_FAILURE);
	}
	if (is_builtin(shell->cmd_list->args[0]))
		call_builtins(shell);
	else
	{
		path_dirs = get_path_dirs(shell);
		command_path = get_full_command_path(shell->cmd_list->args[0],
				path_dirs);
		free_matrix(path_dirs);
		call_path(shell, command_path);
		free(command_path);
	}
}

static void	await_results(t_ms *shell, pid_t child_pid)
{
	int	return_status;

	while (shell->cmd_list->next)
	{
		while (wait(NULL) == -1) {
      if (errno != EINTR)
        break;
    }
		shell->cmd_list = shell->cmd_list->next;
	}
	while (waitpid(child_pid, &return_status, 0) == -1)
    if (errno != EINTR)
      break;
	if (WIFEXITED(return_status))
		shell->last_status = WEXITSTATUS(return_status);
}

void	executor(t_ms *shell)
{
	int		initial_stdout;
	int		initial_stdin;
	pid_t	child_pid;
	t_cmd	*first;

	first = shell->cmd_list;
	initial_stdout = dup(STDOUT_FILENO);
	initial_stdin = dup(STDIN_FILENO);
	if (shell->cmd_list->next)
	{
		child_pid = multi_cmd_exec(shell);
		shell->cmd_list = first;
		await_results(shell, child_pid);
	}
	else
		single_cmd_exec(shell);
	dup2(initial_stdout, STDOUT_FILENO);
	close(initial_stdout);
	dup2(initial_stdin, STDIN_FILENO);
	close(initial_stdin);
}
