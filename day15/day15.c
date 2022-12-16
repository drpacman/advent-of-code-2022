#include "../util/util.h"
#include <stdio.h>
#include <stdlib.h>

struct range {
    int start;
    int end;
};

struct entry {
    int sx;
    int sy;
    int bx;
    int by;
    int dist;
};

int compare(const void* l, const void* r){
    struct range* range1 = *(struct range **)l;
    struct range* range2 = *(struct range **)r;
    if (range1->start < range2->start){
        return -1;
    } else if (range1->start > range2->start){
        return 1;
    } else if (range1->end < range2->end){
        return -1;
    } else if (range1->end > range2->end){
        return 1;        
    }
    return 0;
}

void merge_ranges(struct range** ranges, int n, struct range** merged_ranges, int* len){
    // merge ranges
    merged_ranges[0] = ranges[0];
    int index = 0;
    for (int i=1;i<n;i++){
        if (ranges[i]->start >= ranges[i-1]->start && ranges[i]->end > ranges[i-1]->end ){
            merged_ranges[index]->end = ranges[i]->end;
        } else if (ranges[i]->start > ranges[i-1]->end){
            merged_ranges[index++] = ranges[i];
        }
    } 
    *len = index; 
}

int main(){
    int max_pos = 4000000;
    int target_row = 2000000;
    char* filename = "input";

    // int max_pos = 20;
    // int target_row = 10;
    // char* filename = "input_test";

    struct file_contents fc = read_lines(filename);
    int entry_count = fc.line_count;
    struct entry entries[entry_count];
    for (int i=0;i <entry_count;i++){
        struct entry e;
        sscanf(fc.lines[i], "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &e.sx, &e.sy, &e.bx, &e.by);
        e.dist = abs(e.sx-e.bx) + abs(e.sy-e.by);
        entries[i] = e;
    }

    // Part 1    
    int n = 0;
    struct range** ranges = malloc(fc.line_count * sizeof(struct range));
    for (int i=i;i<entry_count;i++){
        struct entry e = entries[i];
        if (abs(e.sy - target_row) < e.dist) {
            int len = e.dist - abs(e.sy - target_row);
            int xmin = e.sx - len;
            int xmax = e.sx + len;
            struct range* r = malloc(sizeof(struct range));
            r->start = xmin;
            r->end = xmax;
            ranges[n++] = r;            
        }
    }
    qsort(ranges, n, sizeof(struct range), compare);    
    struct range** merged_ranges = malloc(n * sizeof(struct range **));
    int merged_size;
    merge_ranges(ranges, n, merged_ranges, &merged_size);
    int total = 0;
    for (int i=0;i<=merged_size;i++){
        total = total + abs(merged_ranges[i]->end - merged_ranges[i]->start);
        // if the range crosses zero, add 1 to the count as zero is a valid position
        if (merged_ranges[i]->start < 0 && merged_ranges[i]->end > 0){
            total = total+1;
        }
    }
    // remove any beacons themselves
    total = total - 1;
    printf("Part 1 - %i\n", total);

    // Part 2
    // walk each entry
    // check if the bounary is outside all the other entries
    for (int i=0;i<entry_count;i++){
        // check all the positions on the boundary of the entry
        for (int j=0; j<entries[i].dist; j++){
            int positions[4][2]={ 
                { entries[i].sx - (entries[i].dist + 1 - j), entries[i].sy - j },
                { entries[i].sx + (entries[i].dist + 1 - j), entries[i].sy - j },
                { entries[i].sx - (entries[i].dist + 1 - j), entries[i].sy + j },
                { entries[i].sx + (entries[i].dist + 1 - j), entries[i].sy + j },
            };
            for (int pos = 0; pos < 4; pos++){
                if ( positions[pos][0] < 0 || positions[pos][0] > max_pos) continue;
                if ( positions[pos][1] < 0 || positions[pos][1] > max_pos) continue;
                int found = 1;
                for (int n=0;n<entry_count && found;n++){
                    if (i == n) continue;            
                    // check if entry contains pos
                    // it does if the manhattan distance is less then the distance to the pos.
                    int pos_dist = abs(entries[n].sx - positions[pos][0]) + abs(entries[n].sy - positions[pos][1]);
                    if (pos_dist <= entries[n].dist){
                        found = 0;
                    }
                }
                if (found){
                    u_int64_t tuning_freq = (u_int64_t)(positions[pos][0] * 4000000) + positions[pos][1];
                    printf("Part 2 - (%i,%i) - %llu\n", positions[pos][0], positions[pos][1], tuning_freq);
                }
            }
        }
    }
}