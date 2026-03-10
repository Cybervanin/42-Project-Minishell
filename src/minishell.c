/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:13:46 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/07 09:54:39 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (*input == '\0')
			continue;
		if (*input)
			add_history(input);
		token_list = lexit(input);
		free(input);
		shell->cmd_list = parser(token_list);
		free(token_list);
		expander(shell);
		executor(shell);
	}
}

int	main(int argc, char *argv[], char *envs[])
{
	t_ms	*shell;

	config_terminal_signals();
	(void)argc;
	(void)argv;
	shell = create_shell_instance(envs);
	shell_loop(shell);
	free_matrix(shell->envs);
	free(shell);
	return (0);
}
