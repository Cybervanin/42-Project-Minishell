/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_env_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 19:45:08 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/24 19:52:52 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**get_path_dirs(t_ms *shell)
{
	char	*path;
	char	**splitted_path;
	char *error_msg;

	path = get_env_val("PATH", shell);
	if (!path)
	{
		error_msg = ft_strjoin(shell->cmd_list->args[0], ": No such file or directory\n");
		ft_putstr_fd(error_msg, 2);
		free(error_msg);
		shell->last_status = 127;
		return (NULL);
	}
	splitted_path = ft_split(path, ':');
	if (!splitted_path)
		return (NULL);
	return (splitted_path);
}

static char	*handle_file(t_ms *shell)
{
	if (shell->cmd_list->args[0] && (shell->cmd_list->args[0][0] == '/'
		|| shell->cmd_list->args[0][0] == '.'))
	{
		if (access(shell->cmd_list->args[0], F_OK | X_OK) == 0)
			return (ft_strdup(shell->cmd_list->args[0]));
	}
	return (NULL);
}

static char	*mount_full_path(t_ms *shell, char **path_dirs)
{
	int		i;
	char	*full_path;
	char	*temp;

	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(temp, shell->cmd_list->args[0]);
		free(temp);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_full_cmd_path(t_ms *shell)
{
	char	*file;
	char	**path_dirs;
	char	*full_path;

	file = handle_file(shell);
	if (file != NULL)
		return (file);
	free(file);
	path_dirs = get_path_dirs(shell);
	if (!path_dirs)
		return (NULL);
	full_path = mount_full_path(shell, path_dirs);
	free_matrix(path_dirs);
	if (!full_path)
		return (NULL);
	return (full_path);
}
