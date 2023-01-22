#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include <assert.h>

const int STACK_SIZE = 100000;
struct file_contents read_lines(char* filename) {
    FILE* fp = fopen(filename, "r");
    size_t len;
    ssize_t read;
    char* line = NULL;    
    int array_size=100;
    char** lines = malloc(array_size * sizeof(char *));
    int count = 0;
    while((read=getline(&line, &len, fp)) != -1){
        char* p;
        // strip newline - find it in string and replace it with null terminator
        if ((p = strchr(line, '\n')) != NULL) {
          *p = '\0';
          read=read-1;
        }
        if (count >= array_size){
            array_size = array_size*2;
            lines = realloc(lines, array_size * sizeof(char *));
        }        
        lines[count] = strdup(line);
        count=count+1;
    }
    fclose(fp);
    struct file_contents results;
    results.lines = lines;
    results.line_count = count;
    return results;
}


void push(struct stack* s, int n) {
    if (s->top > STACK_SIZE) {
        printf("Stack overflow");
        exit(1);
    }
    s->stack[s->top] = n;
    s->top += 1;
}

int pop(struct stack* s) {
    if (s->top == 0) {
        printf("Stack underflow");
        exit(1);
    }
    s->top -= 1;
    return s->stack[s->top];
}

int is_empty(struct stack* s){
    return s->top == 0;
}
void new(struct stack* s) {
    s->stack = malloc(STACK_SIZE * sizeof(int));
    s->top = 0;
}

void struct_stack_push(struct struct_stack* s, void* e) {
    if (s->top > s->stack_size) {
        printf("Stack overflow");
        exit(1);
    }
    s->stack[s->top] = e;
    s->top += 1;
}

int struct_stack_size(struct struct_stack* s){
    return s->top;
}

void* struct_stack_pop(struct struct_stack* s) {
    if (s->top == 0) {
        printf("Stack underflow");
        exit(1);
    }
    s->top -= 1;
    return s->stack[s->top];
}

int struct_stack_is_empty (struct struct_stack* s){
    return s->top == 0;
}
void struct_stack_new (struct struct_stack* s, int stack_size) {
    s->stack = malloc(stack_size * sizeof(void *));
    s->top = 0;
    s->stack_size = stack_size;
}

struct ring_buffer* ringbuffer_new(int buffer_size){
    struct ring_buffer* rb = malloc(sizeof(struct ring_buffer));
    rb->items = calloc(buffer_size, sizeof(void *));
    rb->head = 0;
    rb->tail = 0;
    rb->len = buffer_size;
    return rb;
}

int ringbuffer_is_empty(struct ring_buffer* rb){
    return rb->tail == rb->head;
}

int ringbuffer_size(struct ring_buffer* rb){
    if (rb->head <= rb->tail){
        return rb->tail - rb->head;
    } 
    return ((rb->len-rb->head)+rb->tail) % rb->len;
}

void ringbuffer_clear(struct ring_buffer* rb){
    rb->head=rb->tail;
}

void ringbuffer_append(struct ring_buffer* rb, void * entry){
    assert(rb->tail + 1 % rb->len != rb->head);
    rb->items[rb->tail] = entry;
    rb->tail = (rb->tail + 1) % rb->len;
}

void* ringbuffer_remove(struct ring_buffer* rb){
    assert(rb->head != rb->tail);
    void* item = rb->items[rb->head];
    rb->head = (rb->head + 1) % rb->len;
    return item;    
}

void* ringbuffer_item(struct ring_buffer* rb, int offset){
    assert(offset < rb->len );
    return rb->items[rb->head + offset % rb->len];    
}