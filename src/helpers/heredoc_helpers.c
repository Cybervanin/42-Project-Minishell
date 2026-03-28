/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 19:44:58 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/28 17:26:58 by jode-cas         ###   ########.fr       */
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

void	set_signals_heredoc(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

static void	expand_line(char **line, t_ms *shell)
{
	int	i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '$')
			rebuild_string(line, i, shell);
		i++;
	}
}

void	heredoc_loop(int *fd, char *delimiter, t_ms *shell)
{
	char	*line;
	int		line_count;

	line_count = 0;
	line = readline("> ");
	while (line)
	{
		line_count += 1;
		if (heredoc_stop_condition(line, delimiter))
		{
			free(line);
			return ;
		}
		if (!is_delimiter_quotted(delimiter))
			expand_line(&line, shell);
		ft_putstr_fd(line, fd[1]);
		write(fd[1], "\n", 1);
		free(line);
		line = readline("> ");
	}
	printf("minishell warning: here-document at line %d delimited"
			" by end-of-file (wanted `%s')\n",
			line_count,
			delimiter);
}
