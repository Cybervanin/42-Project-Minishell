/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victde-s <victde-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:13:46 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/22 15:20:08 by victde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int sigint = 0;

int shell_execution(char *input, t_ms *shell)
{
	t_token	*token_list;

	token_list = lexit(input);
	if (!token_list)
	{
		free(input);
		return (1);
	}
	free(input);
	shell->cmd_list = parser(token_list);
	free_token_list(token_list);
	if (!shell->cmd_list)
		return (1);
	expander(shell);
	executor(shell);
	free_cmd_list(shell->cmd_list);
	shell->cmd_list = NULL;
	return (0);
}

void	shell_loop(t_ms *shell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		handle_sigint_last_status(shell);
		if (is_empty_command(input))
			continue;
		if (*input)
			add_history(input);
		shell_execution(input, shell);
	}
}

int	main(int argc, char *argv[], char *envs[])
{
	t_ms	*shell;

	(void)argc;
	(void)argv;
	set_signals();
	shell = create_shell_instance(envs);
	shell_loop(shell);
	free_matrix(shell->envs);
	free(shell);
	return (0);
}
