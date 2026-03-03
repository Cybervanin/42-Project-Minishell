/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:42:23 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 17:42:23 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	array_length(char **array)
{
	int	counter;

	counter = 0;
	while (array[counter])
	{
		counter++;
		if (array[counter] == NULL)
			break ;
	}
	return (counter);
}

t_token_type	get_token_type(char *word)
{
	if (ft_strncmp(word, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(word, ">>", 2) == 0)
		return (REDIRECT_APPEND);
	else if (ft_strncmp(word, ">", 1) == 0)
		return (REDIRECT_OUT);
	else if (ft_strncmp(word, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(word, "<", 1) == 0)
		return (REDIRECT_IN);
	else
		return (WORD);
}

void	skip_until_next_quote(char *s, int *i)
{
	char	quote;

	quote = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] == quote)
		(*i)++;
}
