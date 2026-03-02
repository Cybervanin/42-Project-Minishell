/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 16:23:50 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 16:23:50 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	builtin_echo(t_ms *shell)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (shell->cmd_list->args[i] && is_n_flag(shell->cmd_list->args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (shell->cmd_list->args[i])
	{
		ft_putstr_fd(shell->cmd_list->args[i], 1);
		if (shell->cmd_list->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	shell->last_status = 0;
}
