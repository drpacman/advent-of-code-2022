#include "../util/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int BUFFER_SIZE = 200;

struct ring_buffer {
    uint64_t* items;
    int head;
    int tail;
    int len;
};

struct ring_buffer* new_rb(){
    struct ring_buffer* rb = malloc(sizeof(struct ring_buffer));
    rb->items = calloc(BUFFER_SIZE, sizeof(uint64_t));
    rb->head = 0;
    rb->tail = 0;
    rb->len = BUFFER_SIZE;
    return rb;
}

int is_empty_rb(struct ring_buffer* rb){
    return rb->tail == rb->head;
}

void append_rb(struct ring_buffer* rb, uint64_t n){
    assert(rb->tail + 1 % rb->len != rb->head);
    rb->items[rb->tail] = n;
    rb->tail = (rb->tail + 1) % rb->len;
}

uint64_t remove_rb(struct ring_buffer* rb){
    assert(rb->head != rb->tail);
    uint64_t item = rb->items[rb->head];
    rb->head = (rb->head + 1) % rb->len;
    return item;    
}

struct monkey { 
    struct ring_buffer* items;
    int monkey_number;
    int true_monkey;
    int false_monkey;
    char action;
    char* target;
    int divisor; 
    uint64_t inspection_count;
};

struct game {
    struct monkey** monkeys;
    int monkey_count;
};

struct game* parse(struct file_contents fc){
    struct game* game  = malloc(sizeof(struct game));
    game->monkeys=malloc(10*sizeof(struct monkey*));
    int monkey_count = 0;
    for (int i = 0; i < fc.line_count; i=i+7){
        struct monkey* m = malloc(sizeof(struct monkey));  
        sscanf(fc.lines[i], "Monkey %d:", &m->monkey_number);    
        char* items = malloc(100*sizeof(char));
        sscanf(fc.lines[i+1], "  Starting items: %[^\n]s", items);
        m->items = new_rb();
        char* item;
        while ((item = strtok_r(items, ",", &items)) != NULL){
            append_rb(m->items, (uint64_t)atoi(item));
        }
        m->inspection_count = 0;
        m->target = malloc(100*sizeof(char));
        sscanf(fc.lines[i+2], "  Operation: new = old %c %s", &m->action, m->target);
        sscanf(fc.lines[i+3], "  Test: divisible by %d", &m->divisor);
        sscanf(fc.lines[i+4], "    If true: throw to monkey %d", &m->true_monkey);
        sscanf(fc.lines[i+5], "    If false: throw to monkey %d", &m->false_monkey); 
        game->monkeys[m->monkey_number] = m;
        game->monkey_count += 1;
    }
    return game;
}

int compare(const void* item1, const void* item2){
    int m1 = *((int *) item1);
    int m2 = *((int *) item2);
    return m2 - m1;
}

uint64_t run_game(struct game* g, int rounds, int global_divisor){
    uint64_t cycle = 1;
    for (int i=0;i< g->monkey_count; i++){
        cycle = cycle * g->monkeys[i]->divisor;
    }
    for (int j=0;j<rounds;j++){
        for (int i=0;i<g->monkey_count;i++){
            struct monkey* m = g->monkeys[i];
            while (!is_empty_rb(m->items)){
                m->inspection_count += 1;
                // update old
                uint64_t item = remove_rb(m->items);
                if (m->action == '+'){
                    item = item + atoi(m->target);
                } else if (strncmp(m->target, "old", 3)==0){
                    item = item * item;
                } else {
                    item = item * atoi(m->target);
                }
                if (global_divisor > 0){
                    item = item / global_divisor;
                }
                // keep the numbers in hand by using modulo all the cycles
                item = item % cycle;
                if (item % m->divisor == 0){                
                    append_rb(g->monkeys[m->true_monkey]->items, item);
                } else {
                    append_rb(g->monkeys[m->false_monkey]->items, item);
                }
            }
        }
    }
    // find 2 most active monkeys
    uint64_t inspection_counts[g->monkey_count];
    for (int i=0;i<g->monkey_count;i++){
        inspection_counts[i] = g->monkeys[i]->inspection_count;
    }    
    qsort(inspection_counts,g->monkey_count,sizeof(uint64_t), compare);
    return inspection_counts[0] * inspection_counts[1];
}

int main(){
    struct file_contents fc = read_lines("input");
    struct game* g = parse(fc);
    printf("Part 1 - %llu\n", run_game(g, 20, 3));
    g = parse(fc);
    printf("Part 2 - %llu\n", run_game(g, 10000, -1));
}