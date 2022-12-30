#include "../util/util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void mix(long* moves, long** ring, int ring_len, int loops){
    // loop the whole ring
    for (int l=0;l<loops;l++){
        for (int n=0;n<ring_len;n++){
            long number = moves[n];
            int dist = abs((int)(number % (ring_len-1)));
            // find the current position of the move
            int index = 0;
            for (int i=0;i<ring_len;i++){
                if (ring[i][0] == n){
                    index = i;
                    break;
                }
            }
            // do the move
            if (number > 0){
                for (int i=0;i<dist;i++){
                    int pos = (index+i)%ring_len;
                    int other = (index+i+1)%ring_len;
                    long* tmp = ring[other];
                    ring[other] = ring[pos];
                    ring[pos] = tmp;
                }
            } else {
                for (int i=0;i<dist;i++){
                    int pos = (index+ring_len-i)%ring_len;
                    int other = (index+ring_len-i-1)%ring_len;
                    long* tmp = ring[other];
                    ring[other] = ring[pos];
                    ring[pos] = tmp;
                }
            } 
        }
    }
}

long result(long** ring, int ring_len){
    int zero_index = 0;
    for (int i=0;i<ring_len;i++){
        if (ring[i][1] == 0){
            zero_index = i;
            break;
        }
    }
    int indexes[3] = { 1000, 2000, 3000 };
    long result = 0;
    for (int i=0;i<3;i++){
        result += ring[(zero_index + indexes[i]) % ring_len][1];
    }
    return result;
}

int main(){
    struct file_contents fc = read_lines("input");
    int ring_len = fc.line_count;
    long** ring = malloc(ring_len*sizeof(long*));
    long* moves = malloc(ring_len*sizeof(long));
    for (int i=0;i<ring_len;i++){
        moves[i] = atol(fc.lines[i]);
        long* entry = malloc(2*sizeof(int));
        entry[0] = i;
        entry[1] = moves[i];
        ring[i] = entry;
    }

    mix(moves, ring, ring_len, 1);
    printf("Part1 - %li\n", result(ring, ring_len));

    int dec_key = 811589153;
    for (int i=0;i<ring_len;i++){
        moves[i] = dec_key*moves[i];
        ring[i][0] = i;
        ring[i][1] = moves[i];
    }
    mix(moves, ring, ring_len, 10);
    printf("Part2 - %li\n", result(ring, ring_len));
}