
#include "../util/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int STACK_SIZE = 1000;
struct move {
    int count;
    int from;
    int to;
};

struct stack {
    char* stack;
    int top;
};

struct game {
    struct move* moves;
    int move_count;
    struct stack* stacks;
    int stack_count;
};

void push(struct stack* s, char c) {
    if (s->top > STACK_SIZE) {
        printf("Stack overflow");
        exit(1);
    }
    s->stack[s->top] = c;
    s->top += 1;
}

char pop(struct stack* s) {
    if (s->top == 0) {
        printf("Stack underflow");
        exit(1);
    }
    s->top -= 1;
    return s->stack[s->top];
}

void new(struct stack* s) {
    s->stack = malloc(STACK_SIZE * sizeof(char));
    s->top = 0;
}

void destroy_game(struct game* game){
   free(game->stacks);
   free(game->moves);
   free (game);
}

struct game* load_game(char* filename) {
    struct file_contents fc = read_lines(filename);
    int col_length = strlen(fc.lines[0]);
    int col_count = (col_length + 1) / 4;
    int row_count = 0;
    // find the first blank line
    for (int i = 0; i < fc.line_count;i++){
        if (row_count == 0 && strlen(fc.lines[i]) == 0){
            row_count = i - 1;
            break;
        }
    }

    int offset = row_count + 2;
    int move_count = fc.line_count - offset;
    struct move* moves = malloc(move_count * sizeof(struct move));
    for (int i = 0; i < move_count; i++){
        // split on spaces
        char* move_line = strdup(fc.lines[offset+i]);
        strtok_r(move_line, " ", &move_line);
        struct move m;
        m.count = atoi(strtok_r(move_line, " ", &move_line));
        strtok_r(move_line, " ", &move_line);
        m.from = atoi(strtok_r(move_line, " ", &move_line));
        strtok_r(move_line, " ", &move_line);
        m.to = atoi(strtok_r(move_line, " ", &move_line)); 
        moves[i] = m;                   
    }

    // rotate the first rows and turn into a stack per column
    struct stack* stacks = malloc(col_count*sizeof(struct stack)); 
    for (int col=0;col<col_count;col++){
        new(&stacks[col]);
        for (int row=row_count - 1;row>=0;row--){
            int col_pos = 1+(col*4);
            int c = fc.lines[row][col_pos];
            if (c != ' ') {
                push(&stacks[col], c);
            }
        }
    }

    struct game* g = malloc(sizeof(struct game));
    g->moves = moves;
    g->move_count = move_count;
    g->stacks = stacks;
    g->stack_count = col_count;
    
    return g;
}

char* part2(struct game* g){
    struct stack tmp;
    new(&tmp);        
    for (int i = 0;i < g->move_count;i++){
        struct move m = g->moves[i];
        for (int j=0; j < m.count; j++){
            push(&tmp, pop(&g->stacks[m.from - 1]));
        }
        for (int j=0; j < m.count; j++){
            push(&g->stacks[m.to - 1], pop(&tmp));
        }
    }
    char* result = malloc(g->stack_count+1 * sizeof(char));
    for (int i=0;i<g->stack_count;i++){
        result[i] = pop(&g->stacks[i]);
    }    
    result[g->stack_count+1]='\0';
    return result;
}

char* part1(struct game* g){
    for (int i = 0;i < g->move_count;i++){
        struct move m = g->moves[i];
        for (int j=0; j < m.count; j++){            
            push(&g->stacks[m.to - 1], pop(&g->stacks[m.from - 1]));
        }
    }
    char* result = malloc(g->stack_count+1*sizeof(char));
    for (int i=0;i<g->stack_count;i++){
        result[i] = pop(&g->stacks[i]);
    }    
    result[g->stack_count+1]='\0';
    return result;
}

int main(){
    struct game* game = load_game("input");
   printf("Part 1 - %s\n", part1(game));
   destroy_game(game);
   game = load_game("input");
   printf("Part 2 - %s\n", part2(game));    
   destroy_game(game);   
}
