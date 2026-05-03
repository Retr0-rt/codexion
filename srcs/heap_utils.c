/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 23:41:18 by airkha            #+#    #+#             */
/*   Updated: 2026/05/03 00:27:18 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_nodes(t_heap_node *node1, t_heap_node *node2)
{
	t_heap_node	temp;

	temp.coder_id = node1->coder_id;
	temp.priority = node1->priority;
	node1->coder_id = node2->coder_id;
	node1->priority = node2->priority;
	node2->coder_id = temp.coder_id;
	node2->priority = temp.priority;
}

void	destroy_node(t_heap_node *node)
{
	node->coder_id = 0;
	node->priority = -1;
}

int	has_higher_priority(t_heap_node *a, t_heap_node *b)
{
	if (a->priority != b->priority)
		return (a->priority < b->priority);
	return (a->coder_id < b->coder_id);
}
