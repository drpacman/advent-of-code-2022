#include "../util/util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define GRID_WIDTH 800

int pour_sand(char grid[][GRID_WIDTH], int max_y){
    int count = 0;
    for (;;){
        // add new grain of sand
        int sandx = 500;
        int sandy = 0;
        if (grid[sandy][sandx] == 'o'){
            printf("FULL\n");
            return count;
        }
        for(;;){
            if (sandy > max_y){
                return count;
            }
            
            if (grid[sandy+1][sandx]==' '){
                sandy += 1;
            } else if (grid[sandy+1][sandx-1]==' '){
                sandy += 1;
                sandx -= 1;
            } else if (grid[sandy+1][sandx+1]==' '){
                sandy += 1;
                sandx += 1;
            } else {
                // sand is now at rest
                grid[sandy][sandx] = 'o';
                count++;
                break;
            }
        }
    }
}

int min(int a, int b){
    if (a<b){
        return a;
    } else {
        return b;
    }
}

int max(int a, int b){
    if (a>b){
        return a;
    } else {
        return b;
    }
}

struct grid {
    char grid[200][GRID_WIDTH];
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};

void load_grid(struct file_contents fc, struct grid* g) {
    // create a grid to write to
    for (int y=0; y<200;y++){
        for (int x=0; x<GRID_WIDTH;x++){
            g->grid[y][x]=' ';
        }
    }
    g->min_x=999;
    g->min_y=0;
    g->max_x=0;
    g->max_y=0;
    for (int i=0;i<fc.line_count;i++){
        char* line = strdup(fc.lines[i]);
        int n = 0;
        int pairs[200];
        for(;;){
            char* pair1 = strtok_r(line, " ", &line);
            pairs[n] = atoi(strtok_r(pair1, ",", &pair1));
            pairs[n+1] = atoi(pair1);
            n=n+2;
            if (!line){
                break;
            }
            // skip next ->
            strtok_r(line, " ", &line);
        }
            
        for (int i=0;i<n-2;i=i+2){
            int x1, y1, x2, y2, tmp;
            x1 = pairs[i];
            y1 = pairs[i+1];
            x2 = pairs[i+2];
            y2 = pairs[i+3];
            if (y2 < y1){
                tmp = y2;
                y2 = y1;
                y1 = tmp;
            }
            if (x2 < x1){
                tmp = x2;
                x2 = x1;
                x1 = tmp;
            } 
            g->min_x = min(g->min_x, x1);
            g->min_y = min(g->min_y, y1);
            g->max_x = max(g->max_x, x2);
            g->max_y = max(g->max_y, y2);
            for (int y=y1;y<=y2;y++){
                for (int x=x1; x<=x2; x++){
                    g->grid[y][x]='#';
                }
            }
        }
    }
}

int main(){
    struct file_contents fc = read_lines("input");
    // create a grid to write to
    struct grid g;
    load_grid(fc, &g);
    
    int count = pour_sand(g.grid, g.max_y);
    printf("%i -> %i, %i -> %i", g.min_x, g.max_x, g.min_y, g.max_y);
    for (int y=g.min_y; y<=g.max_y;y++){
        printf("%.*s\n", (g.max_x - g.min_x), g.min_x + g.grid[y]);
    }
    printf("Part 1 = count - %i\n", count);    

    int y_floor = g.max_y + 2;
    int x_min = g.min_x;
    int x_max = g.max_x;
    load_grid(fc, &g);
    // add a floor
    for (int x = x_min-200; x < x_max + 200; x++){
        g.grid[y_floor][x] = '#';
    }
    count = pour_sand(g.grid, y_floor);  
    for (int y=g.min_y; y<=g.max_y+2;y++){
        printf("%.*s\n", GRID_WIDTH, g.grid[y]);
    }
      
    printf("Part 2 = count - %i\n", count);    
}