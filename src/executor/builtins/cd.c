/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 16:00:05 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 16:23:25 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char had_too_many_arguments(char **args, t_ms *shell)
{
	if (array_length(args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		shell->last_status = 1;
		return 1;
	}
	return 0;
}

void	builtin_cd(char **args, t_ms *shell)
{
	char	cwd[1024];
	char	*oldpwd;
	char	*dest;

	if(had_too_many_arguments(args, shell))
		return ;
	oldpwd = getcwd(cwd, sizeof(cwd));
	if (!args[1])
		dest = get_env_val("HOME", shell);
	else if (ft_strncmp(args[1], "-", 1) == 0 && !args[1][1])
		dest = get_env_val("OLDPWD", shell);
	else
		dest = args[1];
	if (!dest || chdir(dest) != 0)
	{
		shell->last_status = 1;
		if (!dest)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		else if (chdir(dest) != 0)
			perror("minishell: cd");
		return ;
	}
	update_env_val("OLDPWD", oldpwd, shell);
	update_env_val("PWD", getcwd(cwd, sizeof(cwd)), shell);
	shell->last_status = 0;
}
