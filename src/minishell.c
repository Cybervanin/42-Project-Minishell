/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:13:46 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/17 19:56:39 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int sigint = 0;

void	shell_loop(t_ms *shell)
{
	char	*input;
	t_token	*token_list;

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
		token_list = lexit(input);
		if (!token_list)
			continue;
		free(input);
		shell->cmd_list = parser(token_list);
		free_token_list(token_list);
		expander(shell);
		executor(shell);
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
	free_minishell_memory(shell);
	return (0);
}
