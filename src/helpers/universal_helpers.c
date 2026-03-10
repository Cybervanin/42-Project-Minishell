/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   universal_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:15:37 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/07 09:58:09 by jode-cas         ###   ########.fr       */
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

char	heredoc_stop_condition(char *line, char *delimiter)
{
	char	*unquoted_delimiter;

	if (is_delimiter_quotted(delimiter))
	{
		unquoted_delimiter = remove_quotes(delimiter);
		if (ft_strncmp(line, unquoted_delimiter,
				ft_strlen(unquoted_delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(unquoted_delimiter))
		{
			free(unquoted_delimiter);
			return (1);
		}
	}
	else
	{
		(void)unquoted_delimiter;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(delimiter))
			return (1);
	}
	return (0);
}
