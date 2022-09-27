/**
 * lrunner.c
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include <time.h>

char *global_executable_memory = 0;

// Allocates size bytes of executable memory.
void *get_executable_memory(size_t size) {
    void *p;

    assert(p = mmap (
        0,
        size,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_ANONYMOUS | MAP_PRIVATE,
        0,
        0
    ));
    
    return p;
}

// Releases memory obtained through calling get_executable_memory.
void free_executable_memory(void *addr, size_t size) {
    munmap(addr, size);
}

// Gets the current time milliseconds as a double.
double get_approx_time_ms() 
{
    long ms;
    time_t s;
    struct timespec spec;
    
    clock_gettime(CLOCK_REALTIME, &spec);
    s = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6);
    if (ms > 999) {
        ++s;
        ms = 0;
    }
    
    return ms;
}

// Loads the given filename and passes control to it with added dependency injection.
void run_program(char *name) {
    // Used for dependency injection.
    typedef struct {
        int (*puts)(const char *s);
    } deps;

    void *binary_blob = global_executable_memory;
    FILE *f;
    long file_size;   
    deps ds;
    double tstart, tend, dt;    
    
    ds.puts = puts;
    
    assert(f = fopen(name, "rb"));
    
    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("preparing to read %ld bytes from %s.\n", file_size, name);
    
    if (file_size > 4096) {
        puts("File size exceeds demo capabilities.");
    }
    
    fread(binary_blob, file_size, 1, f);
    printf("loaded %ld bytes from %s.\n", file_size, name);

    tstart = get_approx_time_ms();

    // Pass control to the binary blob as a void *f(void *args) function.
    ((void *(*)(void *))binary_blob)(&ds);

    tend = get_approx_time_ms();
    dt = tend - tstart;
    
    printf("taken approximately: %lf ms.\n", dt);
    
    fclose(f);
}

int main()
{
    // Disable stdout buffering to avoid having to put fflush(stdout) everywhere.
    setvbuf(stdout, 0, _IONBF, 0);    
    
    global_executable_memory = get_executable_memory(4096);
    assert(global_executable_memory);
    
    for (;;) {            
        char buf[81];
        printf("Enter a program to run: $ ");
        scanf("%81s", buf);
        if (!strcmp(buf, ":q")) {
            puts("The program is shutting down.");
            break;
        }
        printf("running: %s.\n", buf);
        run_program(buf);
    }
    
    free_executable_memory(global_executable_memory, 4096);
    
    return 0;
}
