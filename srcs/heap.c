#include "codexion.h"
#include "scheduler.h"


void swap_nodes(t_heap_node *node1, t_heap_node *node2)
{
    t_heap_node *temp;

    temp->coder_id = node1->coder_id;
    temp->priority = node1->priority;
    node1->coder_id = node2->coder_id;
    node1->priority = node2->priority;
    node2->coder_id = temp->coder_id;
    node2->priority = temp->priority;
}

void destroy_node(t_heap_node *node)
{
    node->coder_id = 0;
    node->priority = -1;
}

t_heap *init_heap(int capacity)
{
    t_heap *heap;

    heap = (t_heap*)malloc(sizeof(t_heap));
    heap->size = 0;
    heap->array = (t_heap_node *)malloc(sizeof(t_heap_node) * capacity);
    return (heap);
}

int heap_peak(t_heap *heap)
{
    if (heap->size == 0)
        return -1;

    return (heap->array[0].coder_id);
}

void heap_push(t_heap *heap, int coder_id, long long priority)
{
    int node_index;

    node_index = heap->size;
    heap->array[node_index].coder_id = coder_id;
    heap->array[node_index].priority = priority;
    while (heap->array[node_index].priority < heap->array[(node_index - 1) / 2].priority || node_index == 0){
        swap_nodes(&heap->array[node_index], &heap->array[(node_index - 1) / 2]);
        node_index = (node_index - 1) / 2;
    }
    heap->size++;
}

int heap_pop(t_heap *heap)
{
    int coder_id;
    int index;

    index = 0;
    coder_id = heap->array[0].coder_id;
    if (heap->size == 0)
        return -1;
    swap_nodes(&heap->array[0], &heap->array[heap->size]);
    heap->size--;
    while(heap->array[index].priority > heap->array[index * 2 + 1].priority 
        && heap->array[index].priority > heap->array[index * 2 + 2].priority
        || index != heap->size)
    {
        if (heap->array[index * 2 + 1].priority > heap->array[index * 2 + 2].priority){
            swap_nodes(&heap->array[index], &heap->array[index * 2 + 1]);
            index = index * 2 + 1;
        }
        else
        {
            swap_nodes(&heap->array[index], &heap->array[index * 2 + 2]);
            index = index * 2 + 2;
        }
    }
    destroy_node(&heap->array[heap->size]);
    heap->size--;
}