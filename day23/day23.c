#include "../util/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Elf {
    int x;
    int y;
    int x_next;
    int y_next;
};

int north[2] = { 0, -1 };
int north_east[2] = { 1, -1 };
int east[2] = { 1, 0 };
int south_east[2] = { 1, 1 };
int south[2] = { 0, 1 };
int south_west[2] = { -1, 1 };
int west[2] = { -1, 0 };
int north_west[2] = { -1, -1 };

int* neighbours[8] = { north, north_east, east, south_east, south, south_west, west, north_west };
int north_quadrant[3] = { 7, 0, 1 };
int south_quadrant[3] = { 3, 4, 5 };
int west_quadrant[3] = { 5, 6, 7 };
int east_quadrant[3] = { 1, 2, 3 };

int* proposed_direction[4] = { north_quadrant, south_quadrant, west_quadrant, east_quadrant };

int compare_elf(const void* e1, const void* e2){
    struct Elf* elf1 = *(struct Elf**)e1;
    struct Elf* elf2 = *(struct Elf**)e2;
    if (elf1->y_next < elf2->y_next){
        return -1;
    } else if (elf1->y_next > elf2->y_next){
        return 1;
    } else if (elf1->x_next < elf2->x_next){
        return -1;
    } else if (elf1->x_next > elf2->x_next){
        return 1;
    }
    return 0;
}

struct Elf* find_elf(struct Elf** elves, int elf_count, int x, int y){
    for (int i=0;i<elf_count;i++){
        if (elves[i]->x == x && elves[i]->y == y){
            return elves[i];
        }
    }
    return NULL;

}

void get_bounds(struct Elf** elves, int elf_count, int* min_x, int* max_x, int* min_y, int* max_y){
    // find smallest rectangle
    *min_x=99999, *max_x=-99999, *min_y=99999, *max_y=-99999;
    for (int e=0;e<elf_count;e++){
        if (elves[e]->x < *min_x) *min_x = elves[e]->x;
        if (elves[e]->x > *max_x) *max_x = elves[e]->x;
        if (elves[e]->y < *min_y) *min_y = elves[e]->y;
        if (elves[e]->y > *max_y) *max_y = elves[e]->y;
    }
}

void print_grid(struct Elf** elves, int elf_count){
    // print grid
    int min_x, max_x, min_y, max_y;
    get_bounds(elves, elf_count, &min_x, &max_x, &min_y, &max_y);
    int x_range = abs(max_x-min_x+1);
    int y_range = abs(max_y-min_y+1);
    char grid[x_range*y_range];
    for (int i=0;i<x_range*y_range;i++){
        grid[i] = '.';
    }
    for (int e=0;e<elf_count;e++){
        int grid_pos = (elves[e]->y - min_y)*x_range + (elves[e]->x - min_x);
        grid[grid_pos] = '#';
    }
    printf("\nGrid [%i elves]\n", elf_count);
    for (int y=0;y<y_range;y++){
        printf("%.*s\n", x_range, &grid[y*x_range]);
    }
}

int do_round(struct Elf** elves, int elf_count, int* proposed_direction_index){
    int done = 1;
    struct Elf* neighbour_elves[8];
    // - walk all the elves
    for (int e=0;e<elf_count;e++){
        struct Elf* elf = elves[e];
        // check for any neighbouring elves
        int neighbour_count = 0;
        // look back find 
        for (int i=0;i<8;i++){
            int dx = neighbours[i][0];
            int dy = neighbours[i][1];
            neighbour_elves[i] = find_elf(elves, elf_count, elf->x+dx, elf->y+dy);  
            if (neighbour_elves[i] != NULL){
                neighbour_count++;
            }              
        }

        if (neighbour_count > 0){
            for (int i=0;i<4;i++){
                int p = (*proposed_direction_index+i)%4;
                int* quadrant = proposed_direction[p];
                // check quadrant
                neighbour_count = 0;
                for (int j=0;j<3;j++){                        
                    if (neighbour_elves[quadrant[j]] != NULL){
                        neighbour_count++;
                    }
                }
                if (neighbour_count == 0){
                    elf->x_next = elf->x + neighbours[quadrant[1]][0];
                    elf->y_next = elf->y + neighbours[quadrant[1]][1];
                    break;
                }
            }
        }
    }
    // - sort elves by next move
    qsort(elves, elf_count, sizeof(struct Elf*), compare_elf);
    // reset next x / y for any duplicates back to their current
    struct Elf* current_elf = elves[0];
    int duplicate = 0;
    for (int e=1;e<elf_count;e++){
        struct Elf* elf = elves[e];
        if (elf->x_next == current_elf->x_next && elf->y_next == current_elf->y_next){
            elf->x_next = elf->x;
            elf->y_next = elf->y;
            duplicate = 1;
        } else if (duplicate == 1){
            // clear first one
            current_elf->x_next = current_elf->x;
            current_elf->y_next = current_elf->y;
            duplicate = 0;
            current_elf = elf;
        } else {
            current_elf = elf;
        }
    }
    // apply all non-duplicate updates
    for (int e=0;e<elf_count;e++){
        if (elves[e]->x != elves[e]->x_next || elves[e]->y != elves[e]->y_next){
            // we are still moving elves
            done = 0;
        }
        elves[e]->x = elves[e]->x_next;
        elves[e]->y = elves[e]->y_next;
    }
    //print_grid(elves, elf_count);
    // - rotate direction
    *proposed_direction_index += 1;
    return done;
}

int main(){    
    struct file_contents fc = read_lines("input");
    int grid_size = fc.line_count;
    struct Elf** elves = malloc( grid_size*grid_size * sizeof(struct Elf*));
    int elf_count = 0;
    for (int y=0;y<grid_size;y++){
        for (int x=0;x<grid_size;x++){
            if (fc.lines[y][x] == '#'){
                struct Elf* elf = malloc(sizeof(struct Elf));
                elf->x = x;
                elf->y = y;
                elf->x_next=x;
                elf->y_next=y;
                elves[elf_count++]=elf;
            }
        }
    }
    qsort(elves, elf_count, sizeof(struct Elf*), compare_elf);
    //print_grid(elves, elf_count);

    struct Elf* neighbour_elves[8];
    int proposed_direction_index = 0;
    for (int i=0;i<10;i++){
        do_round(elves, elf_count, &proposed_direction_index);
    }       

    // Part 1
    // find smallest rectangle
    int min_x=99999, max_x=-99999, min_y=99999, max_y=-99999;
    for (int e=0;e<elf_count;e++){
        if (elves[e]->x < min_x) min_x = elves[e]->x;
        if (elves[e]->x > max_x) max_x = elves[e]->x;
        if (elves[e]->y < min_y) min_y = elves[e]->y;
        if (elves[e]->y > max_y) max_y = elves[e]->y;
    }
    
    // count empty slots in that rectangle
    int total_empty = (max_y-min_y+1)*(max_x-min_x+1) - elf_count;
    printf("Part 1 - %i\n", total_empty);

    // Part 2 - keep going until no move movement
    int round_count = 11;
    while (do_round(elves, elf_count, &proposed_direction_index) != 1){ round_count++; }
    printf("Part 2 - %i\n", round_count);
}