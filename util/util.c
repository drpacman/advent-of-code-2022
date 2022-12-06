#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

struct file_contents read_lines(char* filename) {
    FILE* fp = fopen(filename, "r");
    size_t len;
    ssize_t read;
    char* line = NULL;    
    int array_size=100;
    char** lines = malloc(array_size * sizeof(char *));
    int count = 0;
    while((read=getline(&line, &len, fp)) != -1){
        char* p;
        // strip newline - find it in string and replace it with null terminator
        if ((p = strchr(line, '\n')) != NULL) {
          *p = '\0';
          read=read-1;
        }
        if (count >= array_size){
            array_size = array_size*2;
            lines = realloc(lines, array_size * sizeof(char *));
        }        
        lines[count] = strdup(line);
        count=count+1;
    }
    fclose(fp);
    struct file_contents results;
    results.lines = lines;
    results.line_count = count;
    return results;
}