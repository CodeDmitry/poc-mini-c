/**
 * advanced.c 
 */
void *test(void *dependencies) {
    char buf[] = "Hello, World!";
    typedef struct {
        int (*puts)(const char *s);
    } deps;
    
    ((deps *)dependencies)->puts(buf);
    
    return 0;
}
