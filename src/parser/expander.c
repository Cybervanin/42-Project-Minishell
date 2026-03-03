#include "../minishell.h"

typedef struct s_env_breakpoints {
  int i;
  int j;
  int env_start;
  int env_end;
} t_env_breakpoints;

static void mount_string(t_ms *shell, t_env_breakpoints breakpoints)
{
  char *before_chunk;
  char *after_chunk;
  char *var_value;
  char *temp;
  char *result;

  before_chunk = ft_substr(shell->cmd_list->args[breakpoints.j], 0, breakpoints.env_start);
  if (shell->cmd_list->args[breakpoints.j][breakpoints.env_start + 1] == '?')
    var_value = ft_itoa(shell->last_status);
  else
    var_value = get_env_val(ft_substr(shell->cmd_list->args[breakpoints.j], breakpoints.env_start + 1, breakpoints.env_end - breakpoints.env_start), shell);
  if (!var_value)
    var_value = ft_strdup("");
  after_chunk = ft_substr(shell->cmd_list->args[breakpoints.j], breakpoints.env_end + 1, breakpoints.i - breakpoints.env_end + 1);
  temp = ft_strjoin(before_chunk, var_value);
  result = ft_strjoin(temp, after_chunk);
  shell->cmd_list->args[breakpoints.j] = result;
  free(temp);
  free(before_chunk);
  free(after_chunk);
}

static void rebuild_string(t_ms *shell, int i, int j)
{
  int env_start;
  int env_end;
  t_env_breakpoints breakpoints;

  env_start = i;
  while (shell->cmd_list->args[j][i + 1] && shell->cmd_list->args[j][i + 1] != ' ' && shell->cmd_list->args[j][i + 1] != '"')
    i++;
  env_end = i;
  while (shell->cmd_list->args[j][i])
    i++;
  breakpoints.i = i;
  breakpoints.j = j;
  breakpoints.env_end = env_end;
  breakpoints.env_start = env_start;
  mount_string(shell, breakpoints);
}

void expander(t_ms *shell)
{
  int i;
  int j;

  i = 0;
  j = 0;
  char inside_simple_quote;
  inside_simple_quote = 0;
  while (shell->cmd_list->args[j])
  {
    i = 0;
    while (shell->cmd_list->args[j][i])
    {
      if (shell->cmd_list->args[j][i] == '\'')
        inside_simple_quote = boolean_invert(inside_simple_quote);
      else if (shell->cmd_list->args[j][i] == '$' && !inside_simple_quote)
        rebuild_string(shell, i, j);
      i++;
    }
    if (ft_strchr(shell->cmd_list->args[j], '"') || ft_strchr(shell->cmd_list->args[j], '\''))
      shell->cmd_list->args[j] = remove_quotes(shell->cmd_list->args[j]);
    j++;
  }
}
