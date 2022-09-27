/**
 * runner.c
 */
#include <Windows.h>
#include <stdio.h>
#include <assert.h>

char *global_executable_memory = 0;

// Allocates size bytes of executable memory.
void *get_executable_memory(size_t size) {
    void *p;
    
    assert(p = VirtualAlloc (
        0,
        size,
        MEM_COMMIT,
        PAGE_EXECUTE_READWRITE
    ));
    
    return p;
}

// Releases memory obtained through calling get_executable_memory.
// Size technically not needed for windows, it is for munmap version.
void free_executable_memory(void *addr, size_t size) {
    VirtualFree(addr, size, MEM_RELEASE);
}

// Gets the current performance milliseconds without using a cached performance frequency.
double get_approx_time_ms() {
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;    
    double retval;
    
    QueryPerformanceFrequency(&frequency); // ticks per second
    QueryPerformanceCounter(&counter); // current counter
    retval = 1000.0 * ((double)counter.QuadPart) / frequency.QuadPart;
    
    return retval;
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
