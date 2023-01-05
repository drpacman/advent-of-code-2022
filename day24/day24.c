#include "../util/util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int empty=0, up=0b0001, down=0b0010, left=0b0100, right=0b1000, wall=0b10000;
int millisecond = 1000000;

struct game_state {
    int* board;
    int width;
    int height;
};

void print_state(struct game_state* gs){
    for (int y=0;y<gs->height;y++){
        for (int x=0;x<gs->width;x++){
            int pos = y*gs->width + x;
            switch(gs->board[pos]){
                case 1: printf("^"); break;
                case 2: printf("v"); break;
                case 4: printf("<"); break;
                case 8: printf(">"); break;
                case 16: printf("#"); break;
                case 0: printf("."); break;
                default: printf("*"); break;
            }
        }
        printf("\n");
    }
}

void display_states(struct game_state** states, int count){
   struct timespec delay = { .tv_sec = 0, .tv_nsec = 300 * millisecond };
   for (int i=0;i<count;i++){
        //printf("\e[1;1H\e[2J");
        printf("State %i\n", i);
        print_state(states[i]);
        printf("\n");

    nanosleep(&delay, &delay);
    }
}

struct game_state* load_initial_state(char* filename){
    struct file_contents fc = read_lines(filename);
    int line_width= strlen(fc.lines[0]);
    int height = fc.line_count;
    int width = line_width;
    struct game_state* initial_state = malloc(sizeof(struct game_state));;
    initial_state->board = malloc(width*height*sizeof(int));
    initial_state->width = width;
    initial_state->height = height;
    for (int y=0;y<fc.line_count;y++){
        for (int x=0;x<line_width;x++){
            int pos = y*width + x;
            switch (fc.lines[y][x]){
                case '^': initial_state->board[pos]=up; break;
                case 'v': initial_state->board[pos]=down; break;
                case '>': initial_state->board[pos]=right; break;
                case '<': initial_state->board[pos]=left; break;
                case '.': initial_state->board[pos]=empty; break;
                case '#': initial_state->board[pos]=wall; break;
            }
        }
    }
    return initial_state;
}

struct game_state* iterate_state(struct game_state* gs){
    struct game_state* next = malloc(sizeof(struct game_state));
    int elem_count = gs->width*gs->height;
    next->board = malloc(elem_count*sizeof(int));
    next->height = gs->height;
    next->width = gs->width;

    for (int y=0;y<gs->height;y++){
        for (int x=0;x<gs->width;x++){
            int pos = y*gs->width + x;
            if (gs->board[pos] & wall){
                next->board[pos] = wall;
            }
            if (gs->board[pos] & up){
                int next_y = y - 1;
                if (next_y == 0){
                    next_y = gs->height - 2;
                }
                int next_pos = next_y*gs->width + x;
                next->board[next_pos] |= up; 
            }
            if (gs->board[pos] & down){
                int next_y = y + 1;
                if (next_y >= gs->height - 1){
                    next_y = 1;
                }
                int next_pos = next_y*gs->width + x;
                next->board[next_pos] |= down; 
            }
            if (gs->board[pos] & left){
                int next_x = x - 1;
                if (next_x == 0){
                    next_x = gs->width - 2;
                }
                int next_pos = y*gs->width + next_x;
                next->board[next_pos] |= left; 
            }
            if (gs->board[pos] & right){
                int next_x = x + 1;
                if (next_x >= gs->width - 1){
                    next_x = 1;
                }
                int next_pos = y*gs->width + next_x;
                next->board[next_pos] |= right; 
            }
        }
    }
    return next;
}

int main(){
    struct game_state* init = load_initial_state("input");
    struct game_state** states = malloc(1000*sizeof(struct game_state));
    // create states for 1000
    states[0] = init;
     for (int i=1;i<1000;i++){
        states[i] = iterate_state(states[i-1]);
    }

    struct move {
        int pos[2];
        int count;
    };

    // display_states(states, 4);

    struct move* init_move = malloc(sizeof(struct move));
    init_move->pos[0] = 1;
    init_move->pos[1] = 0;
    init_move->count = 0;
        
    struct game_state* state;
    struct move* m;
    
    // check neighbours, if they are empty then add next move
    struct ring_buffer* rb = ringbuffer_new(1000);
    // for each state, count the least moves to reach that position
    int game_board_size = states[0]->width*states[0]->height;
    int seen[game_board_size];
    int curr_state_num = 0;
    ringbuffer_append(rb, init_move);
    int targets[3][2] = {
        { states[0]->width - 2, states[0]->height - 1},
        { 1, 0 },
        { states[0]->width - 2, states[0]->height - 1}          
    };
    for (int n=0;n<3;n++){
        int done = 0;    
        while (!ringbuffer_is_empty(rb) && !done){            
            m = (struct move*) ringbuffer_remove(rb);
            int state_num = m->count+1;
            state = states[state_num];
            if (state_num > curr_state_num){
                // reset least moves
                for (int i=0;i<game_board_size;i++){
                    seen[i] = 0;
                }
                curr_state_num = state_num;
            }
            int candidates[5][2] = {
                { m->pos[0] - 1, m->pos[1] },
                { m->pos[0] + 1, m->pos[1] },
                { m->pos[0], m->pos[1] + 1 },
                { m->pos[0], m->pos[1] - 1},
                { m->pos[0], m->pos[1] }
            };
            for (int i=0; i<5 && !done; i++){     
                if (candidates[i][0] >= 0 && 
                    candidates[i][0] < state->width &&
                    candidates[i][1] >= 0 &&
                    candidates[i][1] < state->height){
                    int pos = candidates[i][1]*state->width + candidates[i][0];
                    if (seen[pos]) continue;
                    if (state->board[pos] == 0){
                        seen[pos] = 1;
                        struct move* next = malloc(sizeof(struct move));
                        next->pos[0] = candidates[i][0];
                        next->pos[1] = candidates[i][1];
                        next->count = m->count + 1;
                        if (next->pos[0] == targets[n][0] && next->pos[1] == targets[n][1]){
                            done = 1;
                            ringbuffer_clear(rb);
                            ringbuffer_append(rb, next);
                            break;
                        } else {                                        
                            // add move to moves;
                            ringbuffer_append(rb, next);  
                        }
                    }        
                }
            } 
        } 
        if (!done){
            printf("ERROR - no result found\n");
        }
        if (n == 0){
            printf("Part 1 - %i\n", curr_state_num);
        }  
    }
    printf("Part 2 - %i\n", curr_state_num);
}