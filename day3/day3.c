#include <stdio.h>
#include <string.h>

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

int part1(char * filename) {
    FILE* fp = fopen(filename, "r");
    char* line = NULL;
    size_t len;
    ssize_t read;
    int total = 0;
    while((read=getline(&line, &len, fp)) != -1){
        // split line in half
        int carriage_size = read/2;
        int first = 0;
        int second = carriage_size;  
        // sort each line half
        qsort (line, carriage_size, sizeof(char), compare); 
        qsort (line + (carriage_size*sizeof(char)), carriage_size, sizeof(char), compare); 
        char match;
        while (first < carriage_size && second < read) {
           if (line[first] == line[second]) {
               // we have one answer
               match = line[first];
               break;
           } 
           if (line[first] < line[second]){
               first++;
           } else {
               second++;
           }
        }  
        total += priority(match);
    }
    fclose(fp);
    return total;
}

int part2(char * filename) {
    FILE* fp = fopen(filename, "r");
    size_t len;
    ssize_t read;
    int total = 0;
    int i = 0;
    char* line = NULL;
    char* group[3];
    int pos[3] = {0,0,0};
    while((read=getline(&line, &len, fp)) != -1){
        char* p;
        // strip newline - find it in string and replace it with null terminator
        if ((p = strchr(line, '\n')) != NULL) {
          *p = '\0';
          read=read-1;
        }
        // sort the line
        qsort (line, read, sizeof(char), compare);   
        group[i] = strdup(line);
        if (i == 2){
            char match;
            for (;;) {
                // check for a match
                if (group[0][pos[0]] == group[1][pos[1]] &&
                    group[1][pos[1]] == group[2][pos[2]]) {
                        match = group[0][pos[0]];
                        break;
                }
                
                // find the smallest entry
                char min = group[0][pos[0]];
                for (int k=1;k<3;k++){
                    if (group[k][pos[k]] < min){
                        min = group[k][pos[k]];
                    }
                }
                // advance the minimium entries
                for (int k=0;k<3;k++){
                    if (group[k][pos[k]] == min){
                        pos[k] += 1;
                    }
                }
            } 
            total += priority(match);
            pos[0]=0;
            pos[1]=0;
            pos[2]=0;
            i=0;
        } else {
            i++;              
        }
    }
    fclose(fp);
    return total;
}

int main() {
    printf("Part 1 - %d\n", part1("input"));
    printf("Part 2 - %d\n", part2("input"));
}