#include "../util/util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum block_state { empty, exterior, occupied };
    
int grid_pos(int* point, int max_x, int max_y, int max_z){    
    if (point[0] < 0 || point[0] > max_x) return -1;
    if (point[1] < 0 || point[1] > max_y) return -1;
    if (point[2] < 0 || point[2] > max_z) return -1;
    int x_index = point[0];
    int y_index = point[1];
    int z_index = point[2];
    return z_index*((max_x+1)*(max_y+1)) + (y_index*(max_x+1)) + x_index;
}

int is_unoccupied(int* grid3d, int pos){
    return grid3d[pos] != occupied;
}

int is_exterior(int* grid3d, int pos){
    return grid3d[pos] == exterior;
}

int surface_area(int* grid3d, int max_x, int max_y, int max_z, int (*check)(int*, int)) {
    int surface_area_count = 0;
    for (int x=0;x<=max_x;x++){
        for (int y=0;y<=max_y;y++){
            for (int z=0;z<=max_z;z++){
                int point[3] = {x,y,z};
                int pos = grid_pos(point, max_x, max_y, max_z);
                if (pos == -1 || grid3d[pos] != occupied) continue;
                // position is occupied, check neighbours - if neighbour >=0 then decrement point
                int neighbours[6][3] = {{-1,0,0}, {1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1}};
                for (int i=0;i<6;i++){
                    int neighbour[3] = { x + neighbours[i][0],  y + neighbours[i][1], z + neighbours[i][2] };
                    int neighbour_pos = grid_pos(neighbour, max_x, max_y, max_z);
                    if (neighbour_pos < 0 || check(grid3d, neighbour_pos)){
                        surface_area_count++;
                    } 
                }
            }
        }
    }
    return surface_area_count;
}
    

int main(){
    struct file_contents fc = read_lines("input");
    // parse file into int[3] list
    int point_count= fc.line_count;
    int** points = malloc(point_count*sizeof(int*));
    int max_x = 0, max_y = 0, max_z = 0;
    for (int i=0;i<fc.line_count;i++){
        int* entry = malloc(3*sizeof(int));
        sscanf(fc.lines[i], "%d,%d,%d", &entry[0], &entry[1], &entry[2]);        
        if (entry[0] > max_x) max_x=entry[0];
        if (entry[1] > max_y) max_y=entry[1];
        if (entry[2] > max_z) max_z=entry[2];
        points[i] = entry;
    }
    int grid_size = (max_x+1)*(max_y+1)*(max_z+1);
    int* grid3d = malloc(grid_size*sizeof(enum block_state));
    for (int i=0;i<grid_size;i++){
        grid3d[i] = empty;
    }
    // init all existing points to occupied
    for (int i=0;i<point_count;i++){
        int* point = points[i];
        grid3d[grid_pos(point, max_x, max_y, max_z)] = occupied;
    }
    // identify all exterior points - it is exerior if it is empty and if any of its neighbours is exterior
    for(;;){
        int identified_additional_exterior_item_count = 0;
        for (int x=0;x<=max_x;x++){
            for (int y=0;y<=max_y;y++){
                for (int z=0;z<=max_z;z++){
                    int point[3] = {x,y,z};
                    int pos = grid_pos(point, max_x, max_y, max_z);
                    if (grid3d[pos] == empty) {
                        // position is unoccupied, check neighbours 
                        // if neighbour it exterior then also mark this position as exterior                        
                        int neighbours[6][3] = {{-1,0,0}, {1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1}};
                        for (int i=0;i<6;i++){
                            int neighbour[3] = { x + neighbours[i][0],  y + neighbours[i][1], z + neighbours[i][2] };
                            int neighbour_pos = grid_pos(neighbour, max_x, max_y, max_z);
                            if (neighbour_pos < 0 || grid3d[neighbour_pos] == exterior){
                                grid3d[pos] = exterior;
                                identified_additional_exterior_item_count+=1;
                            } 
                        }
                    }
                }
            }
        }
        if (identified_additional_exterior_item_count == 0){
            break;
        }
    }       
    printf("Part 1 - %i\n", surface_area(grid3d, max_x, max_y, max_z, is_unoccupied));
    printf("Part 2 - %i\n", surface_area(grid3d, max_x, max_y, max_z, is_exterior));
}