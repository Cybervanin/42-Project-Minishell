/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:41:32 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 17:41:33 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*create_token_list(char **str_tokens)
{
	size_t	x;
	size_t	length;
	t_token	*token_list;
	t_token	new_token;

	length = array_length(str_tokens);
	token_list = ft_calloc(sizeof(t_token), length + 1);
	if (!token_list)
		return (0);
	x = 0;
	while (x < length)
	{
		new_token.value = str_tokens[x];
		new_token.type = get_token_type(str_tokens[x]);
		token_list[x] = new_token;
		x++;
	}
	return (token_list);
}

static char	were_quotes_closed(char *input, size_t i)
{
	size_t	double_quote_count;
	size_t	simple_quote_count;
	char	inside_double_quotes;
	char	inside_simple_quotes;

	inside_double_quotes = 0;
	inside_simple_quotes = 0;
	simple_quote_count = 0;
	double_quote_count = 0;
	while (input[i])
	{
		if (input[i] == '"' && inside_simple_quotes == 0)
		{
			double_quote_count++;
			inside_double_quotes = boolean_invert(inside_double_quotes);
		}
		if (input[i] == '\'' && inside_double_quotes == 0)
		{
			simple_quote_count++;
			inside_simple_quotes = boolean_invert(inside_simple_quotes);
		}
		i++;
	}
	return (double_quote_count % 2 == 0 && simple_quote_count % 2 == 0);
}

t_token	*lexit(char *input)
{
	char	**words;
	t_token	*token_list;

	if (!were_quotes_closed(input, 0))
		printf("Aspas não fechadas\n");
	else
	{
		words = word_splitter(input);
		token_list = create_token_list(words);
		return (token_list);
	}
	return (NULL);
}
