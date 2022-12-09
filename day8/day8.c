#include "../util/util.h"
#include <string.h>
#include <stdio.h>

int main(){
    struct file_contents fc = read_lines("input");
    int width = strlen(fc.lines[0]);
    int height = fc.line_count;
    int count = (2*width) + (2*(height - 2));
    for (int y=1;y<height-1;y++){
        for (int x=1;x<width-1;x++){
            // check 4 directions from this item
            int visible = 4;
            for(int up=y-1; up>=0;up--){
                if (fc.lines[y][x] <= fc.lines[up][x]){
                    visible--;
                    break;
                }
            }
            for(int down=y+1; down<height;down++){
                if (fc.lines[y][x] <= fc.lines[down][x]){
                    visible--;
                    break;
                }
            }
            for(int left=x-1; left>=0;left--){
                if (fc.lines[y][x] <= fc.lines[y][left]){
                    visible--;
                    break;
                }
            }
            for(int right=x+1; right<width;right++){
                if (fc.lines[y][x] <= fc.lines[y][right]){
                    visible--;
                    break;
                }
            }
            if (visible > 0){
                count += 1;
            }
        }
    }
    printf("Part1 - %i\n", count);
    int max_scenic_score=0;
    for (int y=0;y<height-1;y++){
        for (int x=0;x<width-1;x++){
            // check 4 directions from this item
            int scenic_score[4] = { 0,0 ,0, 0 };
            for(int up=y-1; up>=0;up--){
                scenic_score[0]+=1;
                if (fc.lines[y][x] <= fc.lines[up][x]){
                    break;
                }
            }
            for(int down=y+1; down<height;down++){
                scenic_score[1]+=1;
                if (fc.lines[y][x] <= fc.lines[down][x]){
                    break;
                }
            }
            for(int left=x-1; left>=0;left--){
                scenic_score[2]+=1;
                if (fc.lines[y][x] <= fc.lines[y][left]){
                    break;
                }
            }
            for(int right=x+1; right<width;right++){
                scenic_score[3]+=1;
                if (fc.lines[y][x] <= fc.lines[y][right]){
                    break;
                }
            }
            int total = scenic_score[0]*scenic_score[1]*scenic_score[2]*scenic_score[3];
            if (total > max_scenic_score){
                max_scenic_score = total;
            }
        }
    }
    printf("Part2 - %i\n", max_scenic_score);
}
    