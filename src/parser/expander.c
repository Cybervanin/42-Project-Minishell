#include "../minishell.h"

typedef struct s_env_breakpoints {
  int i;
  int env_start;
  int env_end;
} t_env_breakpoints;

static void mount_string(char **og_str, t_env_breakpoints bkpts, t_ms *shell)
{
  char *before_chunk;
  char *after_chunk;
  char *var_value;
  char *temp;
  char *result;

  before_chunk = ft_substr(*og_str, 0, bkpts.env_start);
  if ((*og_str)[bkpts.env_start + 1] == '?')
    var_value = ft_itoa(shell->last_status);
  else
    var_value = get_env_val(ft_substr(*og_str, bkpts.env_start + 1, bkpts.env_end - bkpts.env_start), shell);
  if (!var_value)
    var_value = ft_strdup("");
  after_chunk = ft_substr(*og_str, bkpts.env_end + 1, bkpts.i - bkpts.env_end + 1);
  temp = ft_strjoin(before_chunk, var_value);
  result = ft_strjoin(temp, after_chunk);
  free(*og_str);
  *og_str = result;
  free(temp);
  free(before_chunk);
  free(after_chunk);
}

void rebuild_string(char **og_str, int i, t_ms *shell)
{
  int env_start;
  int env_end;
  t_env_breakpoints breakpoints;

  env_start = i;
  while ((*og_str)[i + 1] && (*og_str)[i + 1] != ' ' && (*og_str)[i + 1] != '"')
    i++;
  env_end = i;
  while ((*og_str)[i])
    i++;
  breakpoints.i = i;
  breakpoints.env_end = env_end;
  breakpoints.env_start = env_start;
  mount_string(og_str, breakpoints, shell);
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
        rebuild_string(&(shell->cmd_list->args[j]), i, shell);
      i++;
    }
    if (ft_strchr(shell->cmd_list->args[j], '"') || ft_strchr(shell->cmd_list->args[j], '\''))
      shell->cmd_list->args[j] = remove_quotes(shell->cmd_list->args[j]);
    j++;
  }
}
