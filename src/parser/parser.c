/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2026/03/07 09:56:39 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/22 14:40:00 by victde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_token_args(t_token *token_list, int i)
{
	int	arg_count;

	arg_count = 0;
	while (token_list[i].value != NULL && token_list[i].type != PIPE)
	{
		if (token_list[i].type == WORD)
			arg_count++;
		else
		{
			if (token_list[i + 1].value != NULL)
				i += 1;
		}
		i += 1;
	}
	return (arg_count);
}

static int	add_new_redir(t_token *token_list, t_cmd *cmd, int *i)
{
	t_redir	*novo;
	t_redir	*ultimo;

	if (token_list[*i + 1].value == NULL)
		printf("minishell: syntax error near unexpected token 'newline'\n");
	else if (token_list[*i + 1].type != WORD)
		printf("minishell: syntax error near unexpected token '%s'\n",
			token_list[*i + 1].value);
	if (token_list[*i + 1].value == NULL || token_list[*i + 1].type != WORD)
		return (1);
	novo = ft_calloc(1, sizeof(t_redir));
	if (!novo)
		return (1);
	novo->type = token_list[*i].type;
	novo->target = ft_strdup(token_list[*i + 1].value);
	if (!novo->target)
		return (free(novo), 1);
	novo->heredoc_fd = -1;
	novo->next = NULL;
	if (cmd->redirs == NULL)
		return (cmd->redirs = novo, 0);
	ultimo = cmd->redirs;
	while (ultimo->next != NULL)
		ultimo = ultimo->next;
	ultimo->next = novo;
	return (0);
}

static int	parse_token_to_cmd(t_token *token_list, t_cmd *cmd, int *i)
{
	int	args_id;

	args_id = 0;
	while (token_list[*i].value != NULL && token_list[*i].type != PIPE)
	{
		if (token_list[*i].type == WORD)
		{
			cmd->args[args_id++] = ft_strdup(token_list[*i].value);
			if (!cmd->args[args_id - 1])
				return (1);
			*i += 1;
		}
		else
		{
			if (add_new_redir(token_list, cmd, i) != 0)
				return (1);
			*i += 2;
		}
	}
	cmd->args[args_id] = NULL;
	return (0);
}

static int	create_and_parse_cmd(t_token *token_list, t_cmd **last, int *i)
{
	t_cmd	*new;

	new = ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (1);
	new->args = ft_calloc(sizeof(char *), count_token_args(token_list, *i) + 1);
	if (!new->args || parse_token_to_cmd(token_list, new, i) != 0)
		return (free_cmd_list(new), 1);
	new->next = NULL;
	if (*last)
		(*last)->next = new;
	*last = new;
	if (token_list[*i].value != NULL && token_list[*i].type == PIPE)
		(*i)++;
	return (0);
}

t_cmd	*parser(t_token *token_list)
{
	t_cmd	*first;
	t_cmd	*last;
	int		i;

	i = 0;
	first = NULL;
	last = NULL;
	while (token_list[i].value != NULL)
	{
		if (create_and_parse_cmd(token_list, &last, &i) != 0)
		{
			free_cmd_list(first);
			return (NULL);
		}
		if (!first)
			first = last;
	}
	return (first);
}
