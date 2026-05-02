#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>
# include <unistd.h>

# define FIFO 0
# define EDF  1

typedef struct s_coder	t_coder;

/* --- heap implementation ---*/

typedef struct s_heap_node {
	int         coder_id;
	long long   priority; // The time value used for sorting
} t_heap_node;

typedef struct s_heap {
	t_heap_node *array;
	int         size;
	int         capacity;
} t_heap;

typedef struct s_dongle {
	pthread_mutex_t mutex;          // Protects the queue and variables below
	pthread_cond_t  cond;           // Where coders sleep while waiting
	int             is_taken;       // 0 = free, 1 = taken
	long long       available_at;   // Timestamp for when the cooldown expires
	t_heap          *queue;         // The priority queue (your heap!)
} t_dongle;

/* The "Big Struct" holding the entire simulation environment */
typedef struct s_system
{
	int					nb_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int req_compiles; /* -1 if not provided */
	int					dongle_cooldown;
	int scheduler; /* SCHED_FIFO or SCHED_EDF */

	/* --- 2. Simulation State --- */
	long long start_time; /* Timestamp when threads launch */
	int stop_flag;        /* 1 if someone burns out or goal met */
	int ready_flag;
	int finished_coders;  /* Tracks how many met req_compiles */
	pthread_cond_t start_gun_cv;
	/* --- 3. Shared Resource Mutexes --- */
	t_dongle *dongles;      /* Array: one mutex per dongle */
	pthread_mutex_t log_mutex;     /* Protects printf output */
	pthread_mutex_t state_mutex; /* Protects reads/writes to stop_flag */

	/* --- 4. The Scheduler (FIFO/EDF) --- */
	void *heap;                   /* Pointer to your Priority Queue */
	pthread_mutex_t queue_mutex;  /* Protects pushing/popping from heap */
	pthread_cond_t *wakeup_calls; /* Array: one cond_var per coder */

	/* --- 5. The Workforce --- */
    pthread_t monitor_thread;
	t_coder *coders; /* Array of individual coders */

}						t_system;

/* The individual profile passed to each thread */
struct					s_coder
{
	pthread_t			thread_id;
	pthread_mutex_t		coder_mutex;
	int id;                       /* 1 to N */
	long long last_compile_start; /* Timestamp of last compile */
	int compiles_done;            /* How many times this coder compiled */

	int left_dongle_id;  /* Index in sys->dongles */
	int right_dongle_id; /* Index in sys->dongles */

	t_system *sys; /* Pointer back to the God Struct */
};

void	swap_nodes(t_heap_node *node1, t_heap_node *node2);
void	destroy_node(t_heap_node *node);
t_heap	*init_heap(int capacity);
int	heap_peek(t_heap *heap);
void	heap_push(t_heap *heap, int coder_id, long long priority);
int	heap_pop(t_heap *heap);

/* --- init.c --- */
void					*coder_routine(void *arg);
void					*monitoring_routine(void *arg);
int	init_system_args(t_system *sys, int ac, char **av);
int	init_mutexes(t_system *sys);
int	init_coders(t_system *sys);
int	start_simulation(t_system *sys);
int	init_all(t_system *sys, int ac, char **av);
/* --- simulation.c --- */
int safe_stop_check(t_system *sys);
void action_compile(t_coder *coder);
void action_debug(t_coder *coder);
void action_refactor(t_coder *coder);
void *coder_routine(void *arg);
void *monitoring_routine(void *arg);
/* --- cleanup.c --- */
void cleanup_system(t_system *sys);
void free_all_mutexes_and_cond_vars(t_system *sys);
/* --- resources.c --- */
void take_dongles(t_coder *coder);
void drop_dongles(t_coder *coder);


/* --- utils.c --- */
long long get_relative_time(long long start_time);
void ft_msleep(int time_to_sleep, t_system *sys);
void wait_for_start(t_system *sys);

/* --- parser.c --- */
char	**parse_args(int ac, char **av);
void	free_parsed_args(char **args);

/* --- ft_trim.c --- */
char	*ft_trim(const char *str);

#endif