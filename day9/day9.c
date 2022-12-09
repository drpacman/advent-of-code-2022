#include "../util/util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int cmpstr(const void *a, const void *b) { 
    return strcmp(*(const char **)a, *(const char **)b);
}

int run(int knot_count){
    // initialise rope
    struct file_contents fc = read_lines("input");
    int knots[knot_count][2];
    for (int i=0;i<knot_count;i++){
        knots[i][0] = 0;
        knots[i][1] = 0;
    }

    int* tail_locations = malloc(10000*sizeof(int));
    tail_locations[0] = 0;
    tail_locations[1] = 0;
    int n=2;
    int R[2] = { 1, 0 };
    int L[2] = { -1, 0 };
    int U[2] = { 0, 1 };
    int D[2] = { 0, -1 }; 
    for (int i=0;i<fc.line_count;i++){
        char* entry = strdup(fc.lines[i]);
        char direction = entry[0];
        int count = atoi(entry+2);
        int* move;
        switch (direction) {
            case 'R': move=R; break;
            case 'L': move=L; break;
            case 'U': move=U; break;
            case 'D': move=D; break;
        }
        for (int j=0;j<count;j++){
            knots[0][0] += *move;
            knots[0][1] += *(move + 1);
            for (int k = 0; k < knot_count-1; k++){
                if (knots[k][0] == knots[k+1][0]){
                    // on same col
                    if (knots[k][1] < knots[k+1][1] - 1){
                        knots[k+1][1] -= 1;
                    } else if (knots[k][1] > knots[k+1][1] + 1){
                        knots[k+1][1] += 1;
                    } else {
                        continue;
                    }
                } else if (knots[k][1] == knots[k+1][1]){
                    // on same row
                    if (knots[k][0] < knots[k+1][0] - 1){
                        knots[k+1][0] -= 1;
                    } else if (knots[k][0] > knots[k+1][0] + 1){
                        knots[k+1][0] += 1;
                    } else {
                        continue;
                    }
                } else if (knots[k][0] < knots[k+1][0] - 1){
                    if (knots[k][1] < knots[k+1][1]){
                        knots[k+1][0] -=1;
                        knots[k+1][1] -=1;
                    } else if (knots[k][1] > knots[k+1][1]){
                        knots[k+1][0] -=1;
                        knots[k+1][1] +=1;
                    } else {
                        continue;
                    }
                } else if (knots[k][0] > knots[k+1][0] + 1){
                    if (knots[k][1] < knots[k+1][1]){
                        knots[k+1][0] +=1;
                        knots[k+1][1] -=1;
                    } else if (knots[k][1] > knots[k+1][1]){
                        knots[k+1][0] +=1;
                        knots[k+1][1] +=1;
                    } else {
                        continue;
                    }
                }  else if (knots[k][1] < knots[k+1][1] - 1){
                    if (knots[k][0] < knots[k+1][0]){
                        knots[k+1][1] -=1;
                        knots[k+1][0] -=1;
                    } else if (knots[k][0] > knots[k+1][0]){
                        knots[k+1][1] -=1;
                        knots[k+1][0] +=1;
                    } else {
                        continue;
                    }
                } else if (knots[k][1] > knots[k+1][1] + 1){
                    if (knots[k][0] < knots[k+1][0]){
                        knots[k+1][1] +=1;
                        knots[k+1][0] -=1;
                    } else if (knots[k][0] > knots[k+1][0]){
                        knots[k+1][1] +=1;
                        knots[k+1][0] +=1;
                    } else {
                        continue;
                    }
                }  else {
                    continue;
                }
                if (k==knot_count-2){
                    tail_locations[n] = knots[k+1][0];
                    tail_locations[n+1] = knots[k+1][1];
                }
                n=n+2;
            }
        }
    }
    int locations_len = n/2;  
    char** locations=malloc(locations_len * sizeof(char *));
    // now count unique moves
    for(int i=0;i<n;i=i+2){
        locations[i/2] = malloc(20*sizeof(char));
        sprintf(locations[i/2], "%i-%i", tail_locations[i], tail_locations[i+1]);
    } 
        
    qsort (locations, locations_len, sizeof(char *), cmpstr);
    int count = 1;
    char* curr = locations[0];
    for(int i=1;i<locations_len;i++){
        if (strcmp(curr, locations[i]) != 0){
            curr = locations[i];
            count=count+1;            
        }
    }
    return count;
}


int main() {
    printf("Part 1 - %i\n", run(2));
    printf("Part 2 - %i\n", run(10));
}