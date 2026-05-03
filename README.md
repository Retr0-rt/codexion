*This project has been created as part of the 42 curriculum by airkha*

# Codexion

## Description

Codexion is a concurrency simulation project that models a scenario where multiple coders compete for limited USB dongles to compile code in a shared co-working space. The goal is to implement a thread-safe program using POSIX threads, mutexes, and condition variables to manage resource allocation, prevent deadlocks, starvation, and ensure fair scheduling while avoiding coder burnout.

The simulation involves coders cycling through compile, debug, and refactor phases, requiring two dongles simultaneously for compilation. The program must handle different scheduling policies (FIFO or EDF) and enforce cooldown periods for dongles after release. A monitor thread detects burnout conditions precisely, and all logging is serialized to prevent interleaved output.

## Instructions

### Compilation

To compile the project, use the provided Makefile:

```bash
make
```

This will compile the source files with the required flags: `-Wall -Wextra -Werror -pthread`.

### Execution

Run the program with the following mandatory arguments:

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

- `number_of_coders`: Number of coders (and dongles).
- `time_to_burnout`: Time in milliseconds before a coder burns out.
- `time_to_compile`: Compilation time in milliseconds.
- `time_to_debug`: Debugging time in milliseconds.
- `time_to_refactor`: Refactoring time in milliseconds.
- `number_of_compiles_required`: Minimum compiles per coder to end simulation.
- `dongle_cooldown`: Cooldown time in milliseconds after dongle release.
- `scheduler`: Either "fifo" or "edf".

Example:

```bash
./codexion 4 1000 200 100 100 5 100 fifo
```

### Cleaning

- `make clean`: Remove object files.
- `make fclean`: Remove object files and executable.
- `make re`: Recompile everything.

## Resources

- POSIX Threads Programming: https://broman.dev/download/The%20Linux%20Programming%20Interface.pdf
- Condition Variables and Mutexes: https://code-vault.net/course/6q6s9eerd0:1609007479575
- AI Usage: AI was used to assist in searching for multiple approaches to solve multithreading programmings problems, debugging concurrency issues, and generating initial code structures for the heap implementation. All generated code was reviewed by me to ensure correctness and adherence to project requirements.

## Blocking Cases Handled

- **Deadlock Prevention**: Ensured by requiring coders to acquire dongles in a specific order (left then right) and releasing them simultaneously.
- **Starvation Prevention**: Implemented fair scheduling with FIFO and EDF policies to prevent any coder from being indefinitely blocked.
- **Cooldown Handling**: Dongles are unavailable for a specified cooldown period after release, managed with timestamps and condition waits.
- **Precise Burnout Detection**: A dedicated monitor thread checks for burnout conditions within 10ms accuracy.
- **Log Serialization**: All output messages are protected by a mutex to prevent interleaving.

## Thread Synchronization Mechanisms

- **pthread_mutex_t**: Used to protect access to shared resources like dongles, the heap for scheduling, and the output stream.
- **pthread_cond_t**: Employed for waiting queues on dongles, allowing coders to wait for availability or cooldown expiration.
- **Custom Heap Implementation**: A priority queue for EDF scheduling, using mutexes to ensure thread-safe insertions and extractions.
- **Race conditions** are prevented by locking critical sections during state changes and resource access. Thread-safe communication between coders and the monitor is achieved through shared variables protected by mutexes, with condition broadcasts for notifications.
