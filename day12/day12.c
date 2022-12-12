#include "../util/util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void display_board(int* steps, int width, int height, int end_pos){
    char* result = malloc(width*height*sizeof(char));    
    for (int y=0;y<height;y++){
        for (int x=0;x<width;x++){
            int pos = y*width + x;
            result[pos] = ' ';
        }
    }
    int pos = end_pos;
    while (steps[pos] > 0){
        int up = pos - width;
        int down = pos + width;
        int left = pos - 1;
        int right = pos + 1;
        if (up >= 0 && steps[up] == steps[pos] - 1){
            result[up] = 'V';
            pos = up;
        } else if (down < width * height && steps[down] == steps[pos] - 1){
            result[down] = '^';
            pos = down;
        } else if (left >= 0 && steps[left] == steps[pos] - 1){
            result[left] = '>';
            pos = left;
        } else if (right >= 0 && steps[right] == steps[pos] - 1){
            result[right] = '<';
            pos = right;
        }
    }    
    result[end_pos]='E';
    
    for (int i=0;i<width*height;i=i+width){
        printf("%.*s\n", width, result+i);
    }   
}

int main(){
    struct file_contents fc = read_lines("input");
    struct stack moves;
    new(&moves);
    // parse all the lines - treat S as 'a' and E as 'z''
    int height=fc.line_count;
    int width=strlen(fc.lines[0]);
    int* board = calloc(height*width, sizeof(int));
    int* steps = calloc(height*width, sizeof(int));
    int end_pos = 0;
    for (int y=0;y<height;y++){
        for (int x=0;x<width;x++){
            int pos = (y*width)+x;
            steps[pos]=999;
            if (fc.lines[y][x] == 'S') {
                board[pos]=0;
                push(&moves, pos); 
                push(&moves, 0); 
                steps[pos]=0;
            } else if (fc.lines[y][x] == 'E'){
                board[pos]=25;
                end_pos=pos;
            } else {
                board[pos]=fc.lines[y][x]-'a';
            }
        }
    }

    int lrud[4] = { -1, 1, -width, width };
    // walk the board from available moves
    while (!is_empty(&moves)){
        int dist = pop(&moves);  
        int existing_pos = pop(&moves);
        // find valid moves from that position
        for (int i=0;i<4;i++) {
            int new_pos = existing_pos + lrud[i];
            if (new_pos >= 0 && new_pos < width*height &&
                board[new_pos] - board[existing_pos] <= 1 &&  // check for valid move                
                steps[new_pos] > dist + 1)// check its a new shortest path so far for this position                
            {
                steps[new_pos] = dist + 1;
                push(&moves, new_pos);
                push(&moves, dist+1);
            }
        }
    }
    display_board(steps, width, height, end_pos);
    
    printf("Part1 - %i\n", steps[end_pos]);
}

