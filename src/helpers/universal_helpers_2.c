/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   universal_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:15:37 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/07 09:58:09 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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