/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 19:43:31 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/24 19:43:31 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	builtin_exit(t_ms *shell)
{
	int	exit_status;

	printf("exit\n");
	if (array_length(shell->cmd_list->args) > 2)
	{
		shell->last_status = 1;
		printf("exit: too many arguments\n");
		return ;
	}
	else if (!ft_isdigit(ft_atoi(shell->cmd_list->args[1])))
	{
		shell->last_status = 1;
		printf("exit: %s: numeric argument required\n",
			shell->cmd_list->args[1]);
		return ;
	}
	exit_status = 0;
	if (shell->cmd_list->args[1])
		exit_status = ft_atoi(shell->cmd_list->args[1]);
	shell->last_status = exit_status;
	exit(exit_status);
}
