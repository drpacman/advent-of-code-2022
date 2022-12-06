#include <stdio.h>
#include <string.h>

int find_unique_seq(char * line, int count) {
    int len = strlen(line);
    for (int i=0;i<len-count-1;i++){
        int found = 1;
        for (int j=0;j<count;j++) {                
            for (int k=0;k<count;k++){
                if (j == k) continue;
                if (line[i+j] == line[i+k]){
                    found = 0;
                    break;
                }
            }
        }
        if (found == 1){
            return i+count;
        }
    }
    return -1;
}

int main(){
    FILE* fp = fopen("input","r");
    char* buff[4];
    size_t len;
    char* line = NULL;    
    int array_size=1000;
    getline(&line, &len, fp);
    printf("Part 1 - %i\n", find_unique_seq(line, 4));    
    printf("Part 2 - %i\n", find_unique_seq(line, 14));    
    fclose(fp);
}