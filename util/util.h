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