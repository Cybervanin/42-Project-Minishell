/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:13:46 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 15:13:55 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *envs[])
{
	char	*input;
	t_token	*token_list;
	t_ms	*shell;

	(void)argc;
	(void)argv;
	shell = create_shell_instance(envs);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		token_list = lexit(input);
		free(input);
		shell->cmd_list = parser(token_list);
		free(token_list);
		expander(shell);
		executor(shell);
	}
	return (0);
}
