#include "../minishell.h"

char	*expand_env_var_value(const char *og_str, t_env_breakpoints bkpts,
		t_ms *shell)
{
	char	*var_value;
	char	*env_key;

	if (og_str[bkpts.env_start + 1] == '?')
	{
		var_value = ft_itoa(shell->last_status);
		return (var_value);
	}
	env_key = ft_substr(og_str, bkpts.env_start + 1, bkpts.env_end
			- bkpts.env_start);
	if (!env_key)
	{
		var_value = ft_strdup("");
		return (var_value);
	}
	var_value = get_env_val(env_key, shell);
	free(env_key);
	if (!var_value)
		var_value = ft_strdup("");
	else
		var_value = ft_strdup(var_value);
	return (var_value);
}

void	remove_empty_arg(char **args)
{
	int	i;

	i = 0;
	if (ft_strlen(*args) == 0 && array_length(args) > 1)
	{
		i = 0;
		while (args[i])
		{
			args[i] = args[i + 1];
			i++;
		}
	}
}
