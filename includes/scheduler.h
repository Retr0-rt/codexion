#ifndef SCHEDULER_H
# define SCHEDULER_H

typedef struct s_heap_node {
    int         coder_id;
    long long   priority; // The time value used for sorting
} t_heap_node;

typedef struct s_heap {
    t_heap_node *array;
    int         size;
    int         capacity;
} t_heap;

#endif