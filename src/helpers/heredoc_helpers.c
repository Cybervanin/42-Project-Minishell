/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 19:44:58 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/24 19:44:58 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	heredoc_stop_condition(char *line, char *delimiter)
{
	char	*unquoted_delimiter;

	unquoted_delimiter = NULL;
	if (is_delimiter_quotted(delimiter))
	{
		unquoted_delimiter = remove_quotes(delimiter);
		if (!unquoted_delimiter)
			return (0);
		if (ft_strncmp(line, unquoted_delimiter,
				ft_strlen(unquoted_delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(unquoted_delimiter))
		{
			free(unquoted_delimiter);
			return (1);
		}
		free(unquoted_delimiter);
	}
	else
	{
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(delimiter))
			return (1);
	}
	return (0);
}

char	is_delimiter_quotted(char *delimiter)
{
	int	delim_size;

	delim_size = ft_strlen(delimiter);
	if (delimiter[0] == '"' && delimiter[delim_size - 1] == '"')
		return (1);
	if (delimiter[0] == '\'' && delimiter[delim_size - 1] == '\'')
		return (1);
	return (0);
}
