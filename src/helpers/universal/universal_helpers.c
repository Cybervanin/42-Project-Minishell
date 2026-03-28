/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   universal_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victde-s <victde-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:15:37 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/28 20:31:05 by victde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	boolean_invert(char value)
{
	if (value == 0)
		return (1);
	else if (value == 1)
		return (0);
	return (2);
}

inline char	is_whitechar(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

inline char	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	is_builtin(char *input)
{
	if (!input)
		return (0);
	if (ft_strncmp(input, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(input, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(input, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(input, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(input, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(input, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(input, "exit", 5) == 0)
		return (1);
	else
		return (0);
}

char	*remove_quotes(char *str)
{
	char	*new;
	char	state;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	new = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new)
		return (NULL);
	i = -1;
	j = 0;
	state = 0;
	while (str[++i])
	{
		if (state == 0 && (str[i] == '\'' || str[i] == '\"'))
			state = str[i];
		else if (state != 0 && str[i] == state)
			state = 0;
		else
			new[j++] = str[i];
	}
	return (new);
}
