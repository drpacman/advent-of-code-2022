#include "../util/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum instructions { noop, addx };

int main(){
    struct file_contents fc = read_lines("input");
    int cycle_num = 0;
    int result = 0;
    int cycle_offset = -20;
    // loop over lines
    int line_num = 0;
    int cycle_count = 0;
    int arg1 = 0;
    int regx = 1;
    char screen[240];
    enum instructions instruction;
    for (;line_num<fc.line_count;){
        if (cycle_count == 0){
            // read the next instruction
            char* entry =strdup(fc.lines[line_num]);
            if (strcmp(entry, "noop") == 0){                
                instruction = noop;
                cycle_count = 1;
            } else {
                instruction = addx;
                arg1 = atoi(entry+5);
                cycle_count = 2;
            }
            line_num+=1;
            continue;
        } 
        cycle_num += 1;
        if ((cycle_offset+cycle_num)%40 == 0){
            int signal_strengh = cycle_num * regx;
            result += signal_strengh;
        }
        int screen_x_pos = (cycle_num-1)%40;
        if (screen_x_pos >= regx-1 && screen_x_pos <= regx+1){
            screen[cycle_num-1] = '#';
        } else {
            screen[cycle_num-1] = '.';
        }
        cycle_count -= 1;
        if (cycle_count == 0){
            if (instruction == addx){
                regx = regx + arg1;
            }
        }
    }
    printf("Part 1 - %i\n", result);
    printf("Part 2:\n");
    for (int i=0;i<6;i++){
        printf("%.*s\n", 40, screen+(40*i));
    }
}
