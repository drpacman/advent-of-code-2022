#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Round {
   int first;
   int second;
};

int score_part1(struct Round round) {
   int score[3][3] = { {3,6,0}, {0,3,6}, {6,0,3}};
   return 1 + round.second + score[round.first][round.second];
}

int score_part2(struct Round round) {
   int score[3][3] = { {3,1,2}, {4,5,6}, {8,9,7}};
   return score[round.second][round.first];
}
int main() {
   FILE *fp;
   
   fp = fopen("input", "r");
   long array_size=10;
   struct Round * games = malloc(array_size*sizeof(struct Round)); 
   long count = 0;
   for (;;){
      char first;
      char second;
      int ret = fscanf(fp, "%c %c\n", &first, &second);
      if (ret <= 0) {
         break;
      }
      struct Round round;
      round.first = ((int)first) - (int)'A';
      round.second = ((int)second) - (int)'X';
      if (count >= array_size) {
         array_size = array_size*2;
         games = realloc(games, array_size*sizeof(struct Round));
      }
      games[count] = round;
      count++;  
   }
   long total = 0;
   for (int i=0;i<count;i++){
      total = total + score_part1(games[i]);
   }
   printf("Part1 - %li\n", total);    
   total = 0;
   for (int i=0;i<count;i++){
      total = total + score_part2(games[i]);
   }
   printf("Part 2 - %li\n", total);    
   free(games);
   fclose(fp);
}