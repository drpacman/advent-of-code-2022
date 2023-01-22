struct file_contents {
    char** lines;
    int line_count;
};
struct file_contents read_lines(char* filename);
struct stack {
    int* stack;
    int top;
};

void push(struct stack* s, int n);
int pop(struct stack* s);
void new(struct stack* s);
int is_empty(struct stack* s);

struct struct_stack {
    void** stack;
    int top;
    int stack_size;
};

void struct_stack_push(struct struct_stack* s, void* e);
void* struct_stack_pop(struct struct_stack* s);
int struct_stack_is_empty (struct struct_stack* s);
void struct_stack_new (struct struct_stack* s, int stack_size);
int struct_stack_size(struct struct_stack* s);

struct ring_buffer {
    void** items;
    int head;
    int tail;
    int len;
};

struct ring_buffer* ringbuffer_new(int buffer_size);
int ringbuffer_is_empty(struct ring_buffer* rb);
int ringbuffer_size(struct ring_buffer* rb);
void ringbuffer_append(struct ring_buffer* rb, void * entry);
void* ringbuffer_remove(struct ring_buffer* rb);
void ringbuffer_clear(struct ring_buffer* rb);
void* ringbuffer_item(struct ring_buffer* rb, int offset);
