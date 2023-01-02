#include "../util/util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct move {
    int steps;
    char dir;
};

 struct cube_face_neighbours {
    struct cube_face* left_cube_face;
    int left_rotation_count;
    struct cube_face* right_cube_face;
    int right_rotation_count;
    struct cube_face* up_cube_face;
    int up_rotation_count;
    struct cube_face* down_cube_face;
    int down_rotation_count;
};

struct cube_face {
    char** entries;
    int face_size;
    int grid_x;
    int grid_y;
    struct cube_face_neighbours neighbours;
};

enum direction { right, down, left, up };
enum direction directions[4] = { right, down, left, up };

void set_cube_face_neighbours(struct cube_face* cube_faces[6], int cube_face, int cube_face_settings[8]){
    cube_faces[cube_face]->neighbours.right_cube_face = cube_faces[cube_face_settings[0]];
    cube_faces[cube_face]->neighbours.right_rotation_count = cube_face_settings[1];
    cube_faces[cube_face]->neighbours.down_cube_face = cube_faces[cube_face_settings[2]];
    cube_faces[cube_face]->neighbours.down_rotation_count = cube_face_settings[3];
    cube_faces[cube_face]->neighbours.left_cube_face = cube_faces[cube_face_settings[4]];
    cube_faces[cube_face]->neighbours.left_rotation_count = cube_face_settings[5];
    cube_faces[cube_face]->neighbours.up_cube_face = cube_faces[cube_face_settings[6]];
    cube_faces[cube_face]->neighbours.up_rotation_count = cube_face_settings[7];
}


void set_neighbours_input_test_part1(struct cube_face* cube_faces[6]){
    int settings0[8] = { 0, 0, 3, 0, 0, 0, 4, 0 };
    set_cube_face_neighbours(cube_faces, 0, settings0);

    int settings1[8] = { 2, 0, 1, 0, 3, 0, 1, 0 };
    set_cube_face_neighbours(cube_faces, 1, settings1);
    
    int settings2[8] = { 3, 0, 2, 0, 1, 0, 2, 0 };
    set_cube_face_neighbours(cube_faces, 2, settings2);
    
    int settings3[8] = { 1, 0, 4, 0, 2, 0, 0, 0 };
    set_cube_face_neighbours(cube_faces, 3, settings3);
    
    int settings4[8] = { 5, 0, 0, 0, 5, 0, 3, 0 };
    set_cube_face_neighbours(cube_faces, 4, settings4);
    
    int settings5[8] = { 4, 0, 5, 0, 4, 0, 5, 0 };
    set_cube_face_neighbours(cube_faces, 5, settings5);    
}

void set_neighbours_input_part1(struct cube_face* cube_faces[6]){
    int settings0[8] = { 1, 0, 2, 0, 1, 0, 4, 0 };
    set_cube_face_neighbours(cube_faces, 0, settings0);

    int settings1[8] = { 0, 0, 1, 0, 0, 0, 1, 0 };
    set_cube_face_neighbours(cube_faces, 1, settings1);
    
    int settings2[8] = { 2, 0, 4, 0, 2, 0, 0, 0 };
    set_cube_face_neighbours(cube_faces, 2, settings2);
    
    int settings3[8] = { 4, 0, 5, 0, 4, 0, 5, 0 };
    set_cube_face_neighbours(cube_faces, 3, settings3);
    
    int settings4[8] = { 3, 0, 0, 0, 3, 0, 2, 0 };
    set_cube_face_neighbours(cube_faces, 4, settings4);
    
    int settings5[8] = { 5, 0, 3, 0, 5, 0, 3, 0 };
    set_cube_face_neighbours(cube_faces, 5, settings5);    
}


void set_neighbours_input_test_part2(struct cube_face* cube_faces[6]){
    int settings0[8] = { 5, 2, 3, 0, 2, 1, 1, 2 };
    set_cube_face_neighbours(cube_faces, 0, settings0);

    int settings1[8] = { 2, 0, 4, 2, 5, 3, 0, 2 };
    set_cube_face_neighbours(cube_faces, 1, settings1);
    
    int settings2[8] = { 3, 0, 4, 1, 1, 0, 0, 3};
    set_cube_face_neighbours(cube_faces, 2, settings2);
    
    int settings3[8] = { 5, 3, 4, 0, 2, 0, 0, 0 };
    set_cube_face_neighbours(cube_faces, 3, settings3);
    
    int settings4[8] = { 5, 0, 1, 2, 2, 3, 3, 0 };
    set_cube_face_neighbours(cube_faces, 4, settings4);
    
    int settings5[8] = { 0, 2, 1, 1, 4, 0, 3, 1 };
    set_cube_face_neighbours(cube_faces, 5, settings5);    
}

void set_neighbours_input_part2(struct cube_face* cube_faces[6]){

    int settings0[8] = { 1, 0, 2, 0, 3, 2, 5, 3 };
    set_cube_face_neighbours(cube_faces, 0, settings0);

    int settings1[8] = { 4, 2, 2, 3, 0, 0, 5, 0 };;
    set_cube_face_neighbours(cube_faces, 1, settings1);
    
    int settings2[8] = { 1, 1, 4, 0, 3, 1, 0, 0 };
    set_cube_face_neighbours(cube_faces, 2, settings2);
    
    int settings3[8] = { 4, 0, 5, 0, 0, 2, 2, 3 };
    set_cube_face_neighbours(cube_faces, 3, settings3);
    
    int settings4[8] = { 1, 2, 5, 3, 3, 0, 2, 0 };
    set_cube_face_neighbours(cube_faces, 4, settings4);
    
    int settings5[8] = { 4, 1, 1, 0, 0, 1, 3, 0 };
    set_cube_face_neighbours(cube_faces, 5, settings5);    
}

int walk_moves(struct cube_face** cube_faces, struct move* moves, int moves_len){
    int x=0, y=0, next_x = 0, next_y = 0;
    struct cube_face* cube_face = cube_faces[0];
    struct cube_face* next_cube_face;
    int CUBE_MAX = cube_face->face_size - 1;
    int d = 0;
    int next_d = 0;
    for (int i=0;i<moves_len;i++){
        struct move m = moves[i];
        // printf("%i - (%i,%i) - (%i,%i) and move %i in dir %i\n", i, x+cube_face->grid_x, y+cube_face->grid_y, x, y, m.steps, d);

        for (int n=0; n<m.steps;n++){
            next_d = d;                
            next_cube_face = cube_face;

            switch (directions[d]){
                case right:
                    if (x == CUBE_MAX) {
                        next_cube_face = cube_face->neighbours.right_cube_face;
                        switch (cube_face->neighbours.right_rotation_count){
                            case 0:
                                next_x = 0;
                                next_y = y;
                                break;
                            case 1:
                                next_x = y;
                                next_y = CUBE_MAX;
                                break;                                
                            case 2:
                                next_x = CUBE_MAX;
                                next_y = CUBE_MAX - y;
                                break;
                            case 3:
                                next_x = CUBE_MAX - y;
                                next_y = 0;
                                break;
                        }
                        next_d = (d + 4 - cube_face->neighbours.right_rotation_count) % 4;
                    } else {
                        next_x = x + 1;
                        next_y = y;
                    }
                    break;                

                case down:
                    if (y == CUBE_MAX) {
                        next_cube_face = cube_face->neighbours.down_cube_face;
                        switch (cube_face->neighbours.down_rotation_count){
                            case 0:
                                next_x = x;
                                next_y = 0;
                                break;
                            case 1:
                                next_x = 0;
                                next_y = CUBE_MAX - x;
                                break;                                
                            case 2:
                                next_x = CUBE_MAX - x;
                                next_y = CUBE_MAX;
                                break;
                            case 3:
                                next_x = CUBE_MAX;
                                next_y = x;
                                break;
                        }
                        next_d = (d + 4 - cube_face->neighbours.down_rotation_count) % 4;
                    } else {
                        next_x = x;
                        next_y = y + 1;    
                    }
                    break;
                
                case left:
                     if (x == 0) {
                        next_cube_face = cube_face->neighbours.left_cube_face;
                        switch (cube_face->neighbours.left_rotation_count){
                            case 0:
                                next_x = CUBE_MAX;
                                next_y = y;
                                break;
                            case 1:
                                next_x = y;
                                next_y = 0;
                                break;                                
                            case 2:
                                next_x = 0;
                                next_y = CUBE_MAX - y;
                                break;
                            case 3:
                                next_x = CUBE_MAX - y;
                                next_y = CUBE_MAX;
                                break;
                        }
                        next_d = (d + 4 - cube_face->neighbours.left_rotation_count) % 4;
                    } else {
                        next_x = x - 1;
                        next_y = y;
                    }
                    break;
                
                case up: 
                    if (y == 0) {
                        next_cube_face = cube_face->neighbours.up_cube_face;
                        switch (cube_face->neighbours.up_rotation_count){
                            case 0:
                                next_x = x;
                                next_y = CUBE_MAX;
                                break;
                            case 1:
                                next_x = CUBE_MAX;
                                next_y = CUBE_MAX - x;
                                break;                                
                            case 2:
                                next_x = CUBE_MAX - x;
                                next_y = 0;
                                break;
                            case 3:
                                next_x = 0;
                                next_y = x;
                                break;
                        }
                        next_d = (d + 4 - cube_face->neighbours.up_rotation_count) % 4;
                    } else {
                        next_x = x;
                        next_y = y - 1;                    
                    }
                    break;

                
            }
            // check if its a wall
            if (next_cube_face->entries[next_y][next_x] == '#'){
                break;
            } else {
                cube_face = next_cube_face;
                x = next_x;
                y = next_y;
                d = next_d;
            }      
        }
        // update dir
        if (m.dir == 'R'){
            d = (d+1)%4;
        } else if (m.dir == 'L'){
            d = (4+d-1)%4;
        }
    }
    // return result
    // printf("(%i,%i) on cube at (%i,%i) and dir %d\n", x, y, cube_face->grid_x, cube_face->grid_y, d);
    return 1000*(y+cube_face->grid_y + 1) + 4*(x+cube_face->grid_x + 1) + d;
}

int main(){
    
    // int cube_face_size=4;
    // char* filename="input_test";
    // void (*set_neighbours_part1)(struct cube_face* cube_faces[6]) = set_neighbours_input_test_part1;
    // void (*set_neighbours_part2)(struct cube_face* cube_faces[6]) = set_neighbours_input_test_part2;

    int cube_face_size=50;
    char* filename="input";
    void (*set_neighbours_part1)(struct cube_face* cube_faces[6]) = set_neighbours_input_part1;
    void (*set_neighbours_part2)(struct cube_face* cube_faces[6]) = set_neighbours_input_part2;

    struct file_contents fc = read_lines(filename);
    // for all the grid lines, note the maximum length
    int grid_height = fc.line_count-2;
    int max_len = 0;
    for (int i=0;i<grid_height;i++){
        int len = strlen(fc.lines[i]);
        if (len> max_len) max_len = len;
    }
    // create a array of entries each of maximum length
    char* grid[grid_height];
    // pad any shorter lines with ' '
    for (int i=0;i<grid_height;i++){
        grid[i] = malloc(max_len*sizeof(char));
        int len = strlen(fc.lines[i]);
        for (int j=0;j<max_len;j++){
            if (j<len){
                grid[i][j] = fc.lines[i][j];
            } else {
                grid[i][j] = ' ';
            }
        }
    }

    // cut the input into squares
    int cube_index=0;
    struct cube_face* cube_faces[6];
    for (int y=0;y<grid_height;y=y+cube_face_size){
        for (int x=0;x<max_len;x=x+cube_face_size){
            if (grid[y][x] != ' '){
                struct cube_face* cube = malloc(sizeof(struct cube_face));
                cube->grid_x = x;
                cube->grid_y = y;
                cube->face_size = cube_face_size;
                cube->entries = malloc(cube_face_size*sizeof(char *));
                for (int i=0;i<cube_face_size;i++){
                    cube->entries[i] = malloc(cube_face_size*sizeof(char));
                    memcpy(cube->entries[i], &grid[y+i][x], cube_face_size);
                }
                cube_faces[cube_index++] = cube;
            }
        }
    }

    // read the passcode into pairs of integer and char
    char* passcode = strdup(fc.lines[fc.line_count - 1]);
    // reserve space for moves
    int passcode_len = strlen(passcode);
    struct move moves[passcode_len/2];
    int steps = 0;
    char dir;
    int move_index = 0;
    for (int i=0;i<passcode_len;i++){
        if (passcode[i] >= '0' && passcode[i] <= '9'){
            steps = steps*10 + (passcode[i] - '0');
        } else {
            dir = passcode[i];
            struct move* m = malloc(sizeof(struct move));
            m->steps = steps;
            m->dir = dir;
            moves[move_index++] = *m;
            steps = 0;
        }        
    }
    // add last step (which has no direction change at end)
    struct move* m = malloc(sizeof(struct move));
    m->dir = 'X';
    m->steps = steps;
    moves[move_index++] = *m;    

    set_neighbours_part1(cube_faces);
    printf("Part 1 - %i\n", walk_moves(cube_faces, moves, move_index));
    set_neighbours_part2(cube_faces);
    printf("Part 2 - %i\n", walk_moves(cube_faces, moves, move_index));
}

