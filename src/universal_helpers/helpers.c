/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:15:37 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 15:36:36 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (ft_strncmp(input, "cd", ft_strlen(input)) == 0)
		return (1);
	else if (ft_strncmp(input, "pwd", ft_strlen(input)) == 0)
		return (1);
	else if (ft_strncmp(input, "export", ft_strlen(input)) == 0)
		return (1);
	else if (ft_strncmp(input, "unset", ft_strlen(input)) == 0)
		return (1);
	else if (ft_strncmp(input, "echo", ft_strlen(input)) == 0)
		return (1);
	else if (ft_strncmp(input, "env", ft_strlen(input)) == 0)
		return (1);
	else if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
		return (1);
	else
		return (0);
}
