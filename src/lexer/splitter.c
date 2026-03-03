/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:41:46 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 17:42:12 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_array(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

static char	*find_token_end(char *s)
{
	int	i;

	i = 0;
	if (is_metachar(s[i]))
	{
		if ((s[i] == '>' && s[i + 1] == '>') || (s[i] == '<' && s[i
					+ 1] == '<'))
			return (s + 2);
		return (s + 1);
	}
	while (s[i] && !is_whitechar(s[i]) && !is_metachar(s[i]))
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			skip_until_next_quote(s, &i);
			break ;
		}
		else
			i++;
	}
	return (s + i);
}

static int	fill_array(char **split, size_t word_count, char *start)
{
	size_t	i;
	char	*end;

	i = 0;
	while (i < word_count)
	{
		while (is_whitechar(*start))
			start++;
		end = find_token_end(start);
		split[i] = ft_calloc(end - start + 1, sizeof(char));
		if (!split[i])
		{
			free_array(split);
			return (0);
		}
		ft_memcpy(split[i], start, end - start);
		start = end;
		i++;
	}
	return (1);
}

static size_t	count_words(char *input)
{
	size_t	count;
	char	*end;

	count = 0;
	while (*input)
	{
		while (is_whitechar(*input))
			input++;
		if (*input)
		{
			count++;
			end = find_token_end(input);
			input = end;
		}
	}
	return (count);
}

char	**word_splitter(char const *str)
{
	size_t	word_count;
	char	**split_array;

	if (!str)
		return (0);
	word_count = count_words((char *)str);
	split_array = ft_calloc(word_count + 1, sizeof(char *));
	if (!split_array)
		return (0);
	if (!fill_array(split_array, word_count, (char *)str))
		return (0);
	return (split_array);
}
