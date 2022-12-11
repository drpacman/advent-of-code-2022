#include "../util/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/tiny-bignum-c/bn.h"

int BUFFER_SIZE = 200;

struct ring_buffer {
    struct bn** items;
    int head;
    int tail;
    int len;
};

struct ring_buffer* new_rb(){
    struct ring_buffer* rb = malloc(sizeof(struct ring_buffer));
    rb->items = calloc(BUFFER_SIZE, sizeof(struct bn));
    rb->head = 0;
    rb->tail = 0;
    rb->len = BUFFER_SIZE;
    return rb;
}

int is_empty_rb(struct ring_buffer* rb){
    return rb->tail == rb->head;
}

void append_rb(struct ring_buffer* rb, struct bn* n){
    if (rb->tail + 1 % rb->len != rb->head){
        rb->items[rb->tail] = n;
        rb->tail = (rb->tail + 1) % rb->len;
    }
}

struct bn* remove_rb(struct ring_buffer* rb){
    assert(rb->head != rb->tail);
    struct bn* item = rb->items[rb->head];
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
    struct bn* divisor; 
    u_int64_t inspection_count;
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
            struct bn* item_as_bn = malloc(sizeof(struct bn));
            bignum_from_int(item_as_bn, atoi(item));
            append_rb(m->items, item_as_bn);
        }
        m->inspection_count = 0;
        m->target = malloc(100*sizeof(char));
        sscanf(fc.lines[i+2], "  Operation: new = old %c %s", &m->action, m->target);
        int divisor;
        sscanf(fc.lines[i+3], "  Test: divisible by %d", &divisor);
        m->divisor = malloc(sizeof(struct bn));
        bignum_from_int(m->divisor, divisor);
                
        sscanf(fc.lines[i+4], "    If true: throw to monkey %d", &m->true_monkey);
        sscanf(fc.lines[i+5], "    If false: throw to monkey %d", &m->false_monkey); 
        game->monkeys[m->monkey_number] = m;
        game->monkey_count += 1;
    }
    return game;
}

int compare_by_inspection_count(const void* item1, const void* item2){
    struct monkey* m1 = ((struct monkey**) item1)[0];
    struct monkey* m2 = ((struct monkey**) item2)[0];
    if (m2->inspection_count>m1->inspection_count){ 
        return 1;
    } else if (m2->inspection_count < m1->inspection_count){
        return -1;
    } else {
        return 0;
    }
}

u_int64_t run_game(struct game* g, int rounds, int global_divisor){
    char* display = malloc(256*sizeof(char));
    struct bn global_divisor_bn;
    if (global_divisor > 0){
        bignum_from_int(&global_divisor_bn, global_divisor);
    }
             
    struct bn cycle;
    for (int i=0;i< g->monkey_count; i++){
        bignum_mul(&cycle, g->monkeys[i]->divisor, &cycle);
    }
    bignum_to_string(&cycle, display, 256);
    printf("Cycle is %s", display);
    for (int j=0;j<rounds;j++){
        for (int i=0;i<g->monkey_count;i++){
            struct monkey* m = g->monkeys[i];
            while (!is_empty_rb(m->items)){
                m->inspection_count += 1;
                // update old
                struct bn* item = remove_rb(m->items);
                // bignum_to_string(item, display, 1024);
                // printf("Item is %s\n", display);
                struct bn* result = malloc(sizeof(struct bn));
                if (m->action == '+'){
                    struct bn target;
                    bignum_from_int(&target, atoi(m->target));
                    bignum_add(item, &target, result);
                } else if (strncmp(m->target, "old", 3)==0){
                    bignum_mul(item, item, result);
                } else {
                    struct bn multiplier;
                    bignum_from_int(&multiplier, atoi(m->target));
                    bignum_mul(item, &multiplier, result);
                }
                if (global_divisor > 0){
                    bignum_div(result, &global_divisor_bn, result);
                }

                bignum_mod(result, &cycle, result);
                struct bn mod;
                bignum_mod(result, m->divisor, &mod);
                if (bignum_is_zero(&mod)){                
                    append_rb(g->monkeys[m->true_monkey]->items, result);
                } else {
                    append_rb(g->monkeys[m->false_monkey]->items, result);
                }
            }
        }
    }
    // find 2 most active monkeys
    qsort(g->monkeys,g->monkey_count,sizeof(size_t), compare_by_inspection_count);
    for (int i=0;i<g->monkey_count;i++){
        printf("Monkey %i insepected %llu items\n", g->monkeys[i]->monkey_number, g->monkeys[i]->inspection_count);
    }
    return g->monkeys[0]->inspection_count * g->monkeys[1]->inspection_count;
}

int main(){
    struct file_contents fc = read_lines("input");
    struct game* g = parse(fc);
    printf("Part 1 - %llu\n", run_game(g, 20, 3));
    printf("Part 2 - %llu\n", run_game(g, 10000, -1));
}