/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   universal_helpers_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:15:37 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/24 19:53:08 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void display_error(char *str, const char *error_msg)
{
	char *result;

	result = ft_strjoin(str, error_msg);
	ft_putstr_fd(result ,2);
	free(result);
}