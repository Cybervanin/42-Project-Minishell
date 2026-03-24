/* ************************************************************************** */
/*		                                                                     */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victde-s <victde-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:53:07 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/22 16:36:30 by victde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	call_builtins(t_ms *shell)
{
	char	cwd[PATH_MAX];

	if (ft_strncmp(shell->cmd_list->args[0], "pwd", 3) == 0)
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
	else if (ft_strncmp(shell->cmd_list->args[0], "echo", 4) == 0)
		builtin_echo(shell);
	else if (ft_strncmp(shell->cmd_list->args[0], "env", 3) == 0)
		builtin_env(shell);
	else if (ft_strncmp(shell->cmd_list->args[0], "cd", 2) == 0)
		builtin_cd(shell->cmd_list->args, shell);
	else if (ft_strncmp(shell->cmd_list->args[0], "export", 6) == 0)
		builtin_export(shell->cmd_list->args, shell);
	else if (ft_strncmp(shell->cmd_list->args[0], "unset", 5) == 0)
		builtin_unset(shell, shell->cmd_list->args[1]);
	else if (ft_strncmp(shell->cmd_list->args[0], "exit", 4) == 0)
		builtin_exit(shell);
}

static void	get_return_status(t_ms *shell, int return_status)
{
	int	return_status_code;

	return_status_code = 0;
	if (WIFEXITED(return_status))
		return_status_code = WEXITSTATUS(return_status);
	else if (WIFSIGNALED(return_status))
	{
		return_status_code = 128 + WTERMSIG(return_status);
		if (WTERMSIG(return_status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(return_status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
	}
	shell->last_status = return_status_code;
}

void	call_path(t_ms *shell, char *cmd_path)
{
	int		return_status;
	pid_t	child_pid;

	if (!cmd_path || !shell->cmd_list->args || !shell->cmd_list->args[0])
	{
		printf("%s: command not found\n", shell->cmd_list->args[0]);
		shell->last_status = 127;
		return ;
	}
	child_pid = fork();
	if (child_pid == 0)
	{
		set_signals_child();
		execve(cmd_path, shell->cmd_list->args, shell->envs);
		perror(cmd_path);
	}
	set_signals_exec();
	while (waitpid(child_pid, &return_status, 0) == -1)
		if (errno != EINTR)
			break ;
	set_signals();
	get_return_status(shell, return_status);
}
