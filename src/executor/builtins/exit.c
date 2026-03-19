#include "../../minishell.h"

void builtin_exit(t_ms *shell)
{
  int exit_status;
  
  printf("exit\n");
  if (count_args(shell->cmd_list->args) > 2)
  {
    shell->last_status = 1;
    printf("exit: too many arguments\n");
    return ;
  }
  exit_status = 0;
  if (arg)
    exit_status = ft_atoi(arg);
  shell->last_status = exit_status;
  exit(exit_status);
}