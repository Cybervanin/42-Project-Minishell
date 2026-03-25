/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:56:50 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/07 09:56:50 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

typedef struct s_env_breakpoints
{
	int	i;
	int	env_start;
	int	env_end;
}	t_env_breakpoints;

static void	mount_string(char **og_str, t_env_breakpoints bkpts, t_ms *shell)
{
	char	*before_chunk;
	char	*after_chunk;
	char	*var_value;
	char	*temp;
	char	*result;

	before_chunk = ft_substr(*og_str, 0, bkpts.env_start);
	if ((*og_str)[bkpts.env_start + 1] == '?')
		var_value = ft_itoa(shell->last_status);
	else
		var_value = get_env_val(ft_substr(*og_str, bkpts.env_start + 1,
					bkpts.env_end - bkpts.env_start), shell);
	if (!var_value)
		var_value = ft_strdup("");
	after_chunk = ft_substr(*og_str, bkpts.env_end + 1, bkpts.i - bkpts.env_end
			+ 1);
	temp = ft_strjoin(before_chunk, var_value);
	result = ft_strjoin(temp, after_chunk);
	free(*og_str);
	*og_str = result;
	free(temp);
	free(before_chunk);
	free(after_chunk);
}

void	rebuild_string(char **og_str, int i, t_ms *shell)
{
	int					env_start;
	int					env_end;
	t_env_breakpoints	breakpoints;

	env_start = i;
	if (!(*og_str)[i + 1])
		return ;	
	if ((*og_str)[i + 1] == '?')
		env_end = i + 1;
	else if (ft_isalpha((*og_str)[i + 1]) || (*og_str)[i + 1] == '_')
	{
		i++;
		while ((*og_str)[i + 1] && (ft_isalnum((*og_str)[i + 1])
				|| (*og_str)[i + 1] == '_'))
			i++;
		env_end = i;
	}
	else
		return ;
	while ((*og_str)[i])
		i++;
	breakpoints.i = i;
	breakpoints.env_end = env_end;
	breakpoints.env_start = env_start;
	mount_string(og_str, breakpoints, shell);
}

static void	expand_arg(char **arg, t_ms *shell)
{
	int		i;
	char	state;

	i = 0;
	state = 0;
	while ((*arg)[i])
	{
		if (state == 0 && ((*arg)[i] == '\'' || (*arg)[i] == '"'))
			state = (*arg)[i];
		else if (state == (*arg)[i])
			state = 0;
		if ((*arg)[i] == '$' && state != '\'')
			rebuild_string(arg, i, shell);
		i++;
	}
}

static void	strip_arg_quotes(char **arg)
{
	char	*no_quotes;

	if (!ft_strchr(*arg, '"') && !ft_strchr(*arg, '\''))
		return ;
	no_quotes = remove_quotes(*arg);
	if (!no_quotes)
		return ;
	free(*arg);
	*arg = no_quotes;
}

void	expander(t_ms *shell)
{
	int		j;

	j = 0;
	while (shell->cmd_list->args[j])
	{
		expand_arg(&(shell->cmd_list->args[j]), shell);
		strip_arg_quotes(&(shell->cmd_list->args[j]));
		j++;
	}
}
