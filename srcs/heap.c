/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 23:58:33 by airkha            #+#    #+#             */
/*   Updated: 2026/04/27 01:14:42 by airkha           ###   ########.fr       */
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

t_heap	*init_heap(int capacity)
{
	t_heap	*heap;

	heap = (t_heap *)malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->capacity = capacity;
	heap->size = 0;
	heap->array = (t_heap_node *)malloc(sizeof(t_heap_node) * capacity);
	if (!heap->array)
	{
		free(heap);
		return (NULL);
	}
	return (heap);
}

int	heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (-1);
	return (heap->array[0].coder_id);
}

void	heap_push(t_heap *heap, int coder_id, long long priority)
{
	int	idx;

	if (heap->size == heap->capacity)
		return ;
	idx = heap->size;
	heap->array[idx].coder_id = coder_id;
	heap->array[idx].priority = priority;
	while (idx > 0 && heap->array[idx].priority < heap->array[(idx - 1)
		/ 2].priority)
	{
		swap_nodes(&heap->array[idx], &heap->array[(idx - 1) / 2]);
		idx = (idx - 1) / 2;
	}
	heap->size++;
}

static void	bubble_down(t_heap *heap, int index)
{
	int	smallest;

	while (1)
	{
		smallest = index;
        
		if ((index * 2 + 1) < heap->size && heap->array[index * 2
			+ 1].priority < heap->array[smallest].priority)
			smallest = index * 2 + 1;
            
		if ((index * 2 + 2) < heap->size && heap->array[index * 2
			+ 2].priority < heap->array[smallest].priority)
			smallest = index * 2 + 2;
            
		if (smallest == index)
			break ;

		swap_nodes(&heap->array[smallest], &heap->array[index]);
		index = smallest;
	}
}

int	heap_pop(t_heap *heap)
{
	int	coder_id;

	if (heap->size == 0)
		return (-1);

	coder_id = heap->array[0].coder_id;
	heap->size--;
	heap->array[0] = heap->array[heap->size];
    destroy_node(&heap->array[heap->size]);
	bubble_down(heap, 0);
	return (coder_id);
}
