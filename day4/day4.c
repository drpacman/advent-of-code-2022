#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

int main(){

    FILE* fp;
    fp = fopen("input", "r");
    char* line=malloc(200*sizeof(char));
    char* first;
    char* second;
    int first_start, first_end, second_start, second_end;
    int total=0;
    int overlap=0;
    while (fgets(line, 100, fp) != NULL){
        // split line on ,
        first = strtok_r(line, ",", &line);
        second = strtok_r(line, ",", &line);
        // split entry on -
        first_start=atoi(strtok_r(first, "-", &first));
        first_end=atoi(strtok_r(first, "-", &first));
        second_start=atoi(strtok_r(second, "-", &second));
        second_end=atoi(strtok_r(second, "-", &second));
        if (first_start == second_start){
            total=total+1;
            overlap=overlap+1;
        } else if (first_start < second_start){
            if (first_end >= second_end) {
                total=total+1;
            } 
            
            if (first_end >= second_start) {
                overlap=overlap+1;
            }
        } else if (second_start < first_start){
            if (second_end >= first_end) {
                total=total+1;
            } 
            
            if (second_end >= first_start) {
                overlap=overlap+1;
            }
        }
        free(line);
        line=malloc(200*sizeof(char));
    }
    printf("Part 1 - %i\n", total);
    printf("Part 2 - %i\n", overlap);
    fclose(fp);
}