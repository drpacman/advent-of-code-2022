#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct file_contents {
    char** lines;
    int line_count;
};

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

int compare(const void * item1, const void * item2) {
   int f = *((char*)item1);
   int s = *((char*)item2);
   if (f > s) return  1;
   if (f < s) return -1;
   return 0;
}

int priority(const char c){
    if ('a' <= c && c <= 'z') {
        return 1 + c - 'a';
    } 
    return 27 + c - 'A';
}

int check_groups(char* group[], int group_len) {
    char match;
    int* pos = calloc(group_len, sizeof(int));        
    for (;;) {
        // check for a match
        int found = 1;
        for (int k=1;k<group_len && found;k++){
            if (group[0][pos[0]] != group[k][pos[k]]) {
                found = 0;
            }
        }
        
        if (found) {
            char match = group[0][pos[0]];
            // printf("Match - %c\n", match);
            free(pos);
            return priority(match);
        }

        // find the smallest entry
        char min = group[0][pos[0]];
        for (int k=1;k<group_len;k++){
            if (group[k][pos[k]] < min){
                min = group[k][pos[k]];
            }
        }
        // advance the minimium entries
        for (int k=0;k<group_len;k++){
            if (group[k][pos[k]] == min){
                pos[k] += 1;
            }
        }
    } 
}

int calculate_total(char** group, int group_len, int batch_size) {
    int total=0;
    for (int i=0;i<group_len;i=i+batch_size){
        total += check_groups(group + i, batch_size);
    }
    for (int i=0;i<group_len;i++){
        free(group[i]);
    }
    free(group);
    return total;
}

int part1(char * filename) {
    int total = 0;
    struct file_contents fc = read_lines(filename); 
    int count = 0;
    // split lines in half
    char **group = malloc((fc.line_count*2) * sizeof(char *));    
    for (int i=0;i<fc.line_count;i++){
        int carriage_size = strnlen(fc.lines[i], 200)/2;
        group[count] = strndup(fc.lines[i], carriage_size);
        qsort (group[count], carriage_size, sizeof(char), compare);         
        group[count+1] = strndup(fc.lines[i] + (carriage_size*sizeof(char)), carriage_size);
        qsort (group[count+1], carriage_size, sizeof(char), compare);        
        count = count + 2;
    }
    return calculate_total(group, count, 2);
}

int part2(char * filename) {
    struct file_contents fc = read_lines(filename);     
    for (int i=0;i<fc.line_count;i++){
        qsort (fc.lines[i], strnlen(fc.lines[i], 100), sizeof(char), compare);   
    }
    return calculate_total(fc.lines, fc.line_count, 3);
}

int main() {
    printf("Part 1 - %d\n", part1("input"));
    printf("Part 2 - %d\n", part2("input"));
}