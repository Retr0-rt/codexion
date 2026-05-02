# Memory Leak Analysis — Codexion

## Valgrind Report (Before Fix)

```
HEAP SUMMARY:
    in use at exit: 912 bytes in 38 blocks
  total heap usage: 70 allocs, 32 frees, 11,263 bytes allocated

912 (304 direct, 608 indirect) bytes in 19 blocks are definitely lost
    at 0x48407B4: malloc (heap.c:37)
    by 0x109574: init_mutexes (init.c:54)
    by 0x10998E: init_all (init.c:124)
    by 0x1092CB: main (main.c:28)
```

**70 allocations but only 32 frees** — 38 blocks leaked.

---

## Root Cause: Unfreed Priority Queues

Each dongle has a priority queue (`t_heap *queue`) allocated in [init_mutexes](file:///home/retr0/Desktop/1337/M3/codexion/srcs/init.c#L54):

```c
sys->dongles[i].queue = init_heap(2);
```

And [init_heap](file:///home/retr0/Desktop/1337/M3/codexion/srcs/heap.c#L33) performs **two allocations** per call:

```c
t_heap *init_heap(int capacity)
{
    heap = malloc(sizeof(t_heap));       // 16 bytes (the struct)
    heap->array = malloc(sizeof(t_heap_node) * capacity);  // 32 bytes (2 nodes)
    ...
}
```

With **19 coders = 19 dongles = 19 heaps**:

| Allocation | Size per dongle | × 19 dongles | Valgrind category |
|---|---|---|---|
| `malloc(t_heap)` | 16 bytes | **304 bytes** | definitely lost (19 blocks) |
| `malloc(t_heap_node[2])` | 32 bytes | **608 bytes** | indirectly lost (19 blocks) |
| **Total** | 48 bytes | **912 bytes** | 38 blocks |

The old [cleanup_system](file:///home/retr0/Desktop/1337/M3/codexion/srcs/cleanup.c) freed `sys->dongles` (the array of dongle structs) but **never freed the heap structs dangling off each dongle**.

---

## Additional Resource Leaks (Not Memory, But POSIX Resources)

Beyond the heap memory, the old cleanup also missed **destroying condition variables**:

| Resource | Initialized in | Destroyed in old cleanup? |
|---|---|---|
| `dongles[i].mutex` | `init_mutexes` | ✅ Yes |
| `dongles[i].cond` | `init_mutexes` | ❌ **No** |
| `coders[i].coder_mutex` | `init_coders` | ✅ Yes |
| `state_mutex` | `init_mutexes` | ✅ Yes |
| `log_mutex` | `init_mutexes` | ✅ Yes |
| `start_gun_cv` | `init_mutexes` | ❌ **No** |

While condvar leaks don't show up in valgrind's heap summary, they leak kernel-level synchronization resources and can cause issues on systems with limited POSIX object limits.

---

## Potential Deadlock on Shutdown

A subtler issue: when the monitor sets `stop_flag = 1`, coder threads check it via `safe_stop_check()` in the `pthread_cond_wait` loop inside `acquire_dongle`:

```c
while (!safe_stop_check(coder->sys) &&
    (heap_peek(dongle->queue) != coder->id || dongle->is_taken == 1))
    pthread_cond_wait(&dongle->cond, &dongle->mutex);
```

If `stop_flag` is set while a coder is **blocked inside `pthread_cond_wait`**, the coder will never re-check the condition — it's asleep on the condvar. `cleanup_system` then calls `pthread_join` on that thread and **hangs forever**.

---

## The Fix

```diff
 void cleanup_system(t_system *sys)
 {
     int i;
 
+    // 1. Wake any coders stuck waiting on dongle condvars
+    i = 0;
+    while (i < sys->nb_coders)
+    {
+        pthread_mutex_lock(&sys->dongles[i].mutex);
+        pthread_cond_broadcast(&sys->dongles[i].cond);
+        pthread_mutex_unlock(&sys->dongles[i].mutex);
+        i++;
+    }
+
-    // 1. Wait for all coder threads to finish
+    // 2. Wait for all coder threads to finish
     ...
 
-    // 3. Destroy all mutexes
+    // 4. Destroy all mutexes and condition variables
     i = 0;
     while (i < sys->nb_coders)
     {
         pthread_mutex_destroy(&sys->dongles[i].mutex);
+        pthread_cond_destroy(&sys->dongles[i].cond);
+        free(sys->dongles[i].queue->array);
+        free(sys->dongles[i].queue);
         pthread_mutex_destroy(&sys->coders[i].coder_mutex);
         i++;
     }
     pthread_mutex_destroy(&sys->state_mutex);
     pthread_mutex_destroy(&sys->log_mutex);
+    pthread_cond_destroy(&sys->start_gun_cv);
     ...
 }
```

Three categories of fixes:

1. **Heap memory** — `free(queue->array)` then `free(queue)` for each dongle
2. **Condvar resources** — `pthread_cond_destroy` for dongle conds and `start_gun_cv`
3. **Shutdown safety** — broadcast all dongle condvars before joining to unblock stuck threads

---

## Valgrind Report (After Fix)

```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
  total heap usage: 70 allocs, 70 frees, 14,335 bytes allocated

All heap blocks were freed -- no leaks are possible

ERROR SUMMARY: 0 errors from 0 contexts
```

**70 allocs, 70 frees. Zero leaks. Zero errors.**
