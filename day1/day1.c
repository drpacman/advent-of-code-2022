#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int compare(const void * item1, const void * item2) {
   int f = *((long*)item1);
   int s = *((long*)item2);
   if (f > s) return  -1;
   if (f < s) return 1;
   return 0;
}

int main() {
   FILE *fp;
   
   fp = fopen("input", "r");
   long array_size=10;
   long * elves = malloc(array_size*sizeof(long)); 
   long count = 0;
   long total = 0;
   char* line = NULL;
   size_t len;
   ssize_t read; 
   char* p;
   while ((read = getline(&line, &len, fp)) != -1) {
      // strip newline - find it in string and replace it with null terminator
      if ((p = strchr(line, '\n')) != NULL) {
         *p = '\0';
      }
      if (strlen(line) == 0) {
         if (count == array_size){
            array_size *=2;
            elves=realloc(elves, array_size*sizeof(long));
         }
         elves[count] = total;
         total = 0;
         count += 1;
      } else {
         errno = 0;
         char* end;
         const long i = strtol(line, &end, 10);   
         if (i > 0) {
            total = total + i;
         } else {
            printf("Invalid non-numerical entry of %s\n", line);
         }
      }
   }
   elves[count] = total;
         
   qsort (elves, array_size, sizeof(long), compare);
   printf("Part 1 - %ld\n", elves[0]);
   printf("Part 2 - %ld\n", elves[0] + elves[1] + elves[2]);
   free(elves);
   fclose(fp);
}