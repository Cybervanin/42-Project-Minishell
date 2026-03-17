/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:53:07 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/17 19:48:26 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_path_dirs(t_ms *shell)
{
	char	*path;
	char	**splitted_path;

	path = get_env_val("PATH", shell);
	splitted_path = ft_split(path, ':');
	if (!splitted_path)
		return (NULL);
	return (splitted_path);
}

char	*get_full_command_path(char *cmd, char **path_dirs)
{
	char	*full_path;
	char	*temp;
	int		i;

	if (cmd && (cmd[0] == '/' || cmd[0] == '.'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

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
		exit(0);
}

static void get_return_status(t_ms *shell, int return_status)
{
	int return_status_code;

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

void	call_path(t_ms *shell, char *cmd)
{
	int		return_status;
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid == 0)
	{
		if (!cmd || !shell->cmd_list->args || !shell->cmd_list->args[0])
		{
			perror(cmd);
			exit(127);
		}
		set_signals_child();
		execve(cmd, shell->cmd_list->args, shell->envs);
		perror(cmd);
    if (errno == EACCES)
			exit(126);
    else
			exit(1);
	}
	set_signals_exec();
	waitpid(child_pid, &return_status, 0);
	get_return_status(shell, return_status);
}
