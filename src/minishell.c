/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victde-s <victde-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:13:46 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/22 14:24:05 by victde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_input(char *input, t_ms *shell)
{
	t_token	*token_list;

	if (is_empty_command(input))
		return (free(input), 0);
	if (*input)
		add_history(input);
	token_list = lexit(input);
	free(input);
	if (!token_list)
		return (0);
	shell->cmd_list = parser(token_list);
	free(token_list);
	if (!shell->cmd_list)
		return (0);
	expander(shell);
	executor(shell);
	return (0);
}

void	shell_loop(t_ms *shell)
{
	char	*input;

	set_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		process_input(input, shell);
	}
}

int	main(int argc, char *argv[], char *envs[])
{
	t_ms	*shell;

	(void)argc;
	(void)argv;
	shell = create_shell_instance(envs);
	shell_loop(shell);
	free_minishell_memory(shell);
	return (0);
}
