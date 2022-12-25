
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../util/util.h"

struct rock {
    int height;
    long* rows;
};

int can_move(long* rock_rows, int rock_height, long* cavern, int cavern_pos){
    for (int i=0;i<rock_height;i++){
        if ((rock_rows[i] ^ cavern[cavern_pos+i]) != (rock_rows[i] | cavern[cavern_pos+i])){
            return 0;
        }
    }
    return 1;
}

void print_row(int row){
    for (int i=8;i>=0;i--){
        if (row & (1<<i)){
            printf("#");
        } else {
            printf(" ");
        }
    }
    printf("\n");
}

int main(){
    struct rock rock1;
    rock1.height = 1;
    rock1.rows = malloc(sizeof(long));
    rock1.rows[0] = 0b000111100;
    
    struct rock rock2;
    rock2.height = 3;
    rock2.rows = malloc(3 * sizeof(long));
    rock2.rows[0] = 0b000010000;
    rock2.rows[1] = 0b000111000;
    rock2.rows[2] = 0b000010000;
    
    struct rock rock3;
    rock3.height = 3;
    rock3.rows = malloc(3 * sizeof(long));
    rock3.rows[0] = 0b000111000;
    rock3.rows[1] = 0b000001000;
    rock3.rows[2] = 0b000001000;
    
    struct rock rock4;
    rock4.height = 4;
    rock4.rows = malloc(4 * sizeof(long));
    rock4.rows[0] = 0b000100000;
    rock4.rows[1] = 0b000100000;
    rock4.rows[2] = 0b000100000;
    rock4.rows[3] = 0b000100000;

    struct rock rock5;
    rock5.height = 2;
    rock5.rows = malloc(2 * sizeof(long));
    rock5.rows[0] = 0b000110000;
    rock5.rows[1] = 0b000110000;

    struct rock rocks[5] = { rock1, rock2, rock3, rock4, rock5 };
    int max_cavern_height = 100000;
    // initialise cavern - 7 wide
    long* cavern = malloc(max_cavern_height*sizeof(long));
    cavern[0] = 0b111111111;
    for (int i=1;i<max_cavern_height;i++){
        cavern[i] = 0b100000001;
    }

    struct file_contents fc = read_lines("input");
    int jet_count = strlen(fc.lines[0]);
    printf("Jet count is %i\n", jet_count);
    short* jets = malloc(jet_count*sizeof(short));
    for (int i=0;i<jet_count;i++){
        if (fc.lines[0][i] == '<'){
            jets[i] = 1;
        } else {
            jets[i] = 0;
        }
    }
    int jet_pos = 0;
    int rock_count=0;
    int rock_pile_height = 0;
    int cycle_start_rock_index = -1;
    int cycle_start_rock_count = -1;
    int cycle_start_rock_height = -1;
    u_int64_t cycle_length = -1;
    u_int64_t cycle_height = -1;
    int cycle_jet_pos = -1;
    int part1 = -1;
    int* cycle_heights = NULL;
    int cycle_base = 0;
    int done = 0;

    while (!done && rock_pile_height < max_cavern_height){
        if (rock_count == 2022){
            part1 = rock_pile_height;
        }
        int rock_index = rock_count % 5;
        // clone the rock
        struct rock r;
        r.height = rocks[rock_index].height;
        r.rows = malloc(r.height * sizeof(long));
        for (int i=0;i<r.height;i++){
            r.rows[i] = rocks[rock_index].rows[i];
        }
        int rock_pos = rock_pile_height + 3;
        int moved = 1;
        while (moved){
            short shiftl = jets[jet_pos];
            jet_pos = (jet_pos+1) % jet_count;
            long* shifted = malloc(r.height*sizeof(long));
            for (int i=0;i<r.height;i++){
                if (shiftl){
                    shifted[i]=r.rows[i]<<1;
                } else {
                    shifted[i]=r.rows[i]>>1;
                }
            }
            // see if we can shift
            if (can_move(shifted, r.height, cavern, rock_pos)){
                r.rows = shifted;
            }
            // see if we can we move down
            moved = can_move(r.rows, r.height, cavern, rock_pos-1);
            if (moved){
                rock_pos = rock_pos - 1;
            } else {
                // we are at rest - update the cavern
                for (int i=0;i<r.height;i++){
                    cavern[rock_pos] = cavern[rock_pos] | r.rows[i];
                    rock_pos += 1;
                }
                if (rock_pos > rock_pile_height){
                    rock_pile_height = rock_pos;
                }
                if (cycle_heights != NULL){
                    int index = (rock_count - cycle_start_rock_count) % cycle_length;
                    cycle_heights[index] = rock_pile_height - cycle_base;
                    done = index == cycle_length - 1;
                }
                // have we re-created the floor?
                if (cavern[rock_pile_height-1] == 0b111111111 && rock_count != 1){
                    if (cycle_start_rock_index == -1){
                        cycle_start_rock_index = rock_index;
                        cycle_start_rock_count = rock_count;
                        cycle_start_rock_height = rock_pile_height;
                        cycle_jet_pos = jet_pos;
                    } else {
                        if (jet_pos == cycle_jet_pos && cycle_start_rock_index == rock_index){
                            cycle_height = rock_pile_height - cycle_start_rock_height;
                            cycle_length = rock_count - cycle_start_rock_count;
                            printf("Cycle from %i with length %llu - start height %i - cycle height %llu at jet pos %i - rock %i\n",  cycle_start_rock_count, cycle_length,cycle_start_rock_height, cycle_height, jet_pos, rock_index);
                            cycle_heights = malloc(cycle_length*sizeof(int));
                            cycle_base = rock_pile_height;
                        }
                    }
                }
            }
        }
        rock_count++;        
    }

    // don't count the floor in the rock pile height...
    printf("Part 1 - rock height is %i\n", part1);
    u_int64_t cycles = (1000000000000 - cycle_start_rock_count)/cycle_length;
    u_int64_t remainder = (1000000000000 - cycle_start_rock_count)%cycle_length;
    u_int64_t part2 = (cycle_start_rock_height - 1) + (cycle_height*cycles) + cycle_heights[remainder] - 1;
    printf("Part 2 - rock height is %llu\n", part2);
}
