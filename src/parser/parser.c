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
/*   Updated: 2026/03/07 09:56:39 by jode-cas         ###   ########.fr       */
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

static void	add_new_redir(t_token *token_list, t_cmd *cmd, int *i)
{
	t_redir	*novo;
	t_redir	*ultimo;

	novo = ft_calloc(sizeof(t_redir), 2);
	if (!novo)
		return ;
	novo->type = token_list[*i].type;
	novo->target = ft_strdup(token_list[*i + 1].value);
	novo->next = NULL;
	if (cmd->redirs == NULL)
		cmd->redirs = novo;
	else
	{
		ultimo = cmd->redirs;
		while (ultimo->next != NULL)
			ultimo = ultimo->next;
		ultimo->next = novo;
	}
}

static void	parse_token_to_cmd(t_token *token_list, t_cmd *cmd, int *i)
{
	int	args_id;

	args_id = 0;
	while (token_list[*i].value != NULL && token_list[*i].type != PIPE)
	{
		if (token_list[*i].type == WORD)
		{
			cmd->args[args_id++] = ft_strdup(token_list[*i].value);
			*i += 1;
		}
		else
		{
			add_new_redir(token_list, cmd, i);
			*i += 2;
		}
	}
	cmd->args[args_id] = NULL;
}

t_cmd	*parser(t_token *token_list)
{
	t_cmd	*head;
	t_cmd	*last;
	int		i;
	int		total_args;
	t_cmd	*new;

	i = 0;
	head = NULL;
	last = NULL;
	while (token_list[i].value != NULL)
	{
		new = ft_calloc(sizeof(t_cmd), 1);
		if (head == NULL)
			head = new;
		else
			last->next = new;
		last = new;
		total_args = count_token_args(token_list, i);
		new->args = ft_calloc(sizeof(char *), total_args + 1);
		parse_token_to_cmd(token_list, new, &i);
		if (token_list[i].value != NULL && token_list[i].type == PIPE)
			i++;
	}
	return (head);
}
