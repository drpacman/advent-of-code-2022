#include "../util/util.h"
#include <stdio.h>
#include <stdlib.h>

struct range {
    int start;
    int end;
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
    struct file_contents fc = read_lines("input");
    int sx,sy,bx,by;
    int target_row = 2000000;
    int n = 0;
    struct range** ranges = malloc(fc.line_count * sizeof(struct range));
    for (int i=0;i < fc.line_count;i++){
        sscanf(fc.lines[i], "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy, &bx, &by);
        int dist = abs(sx-bx) + abs(sy-by);
        if (abs(sy - target_row) < dist) {
            int len = dist - abs(sy - target_row);
            int xmin = sx - len;
            int xmax = sx + len;
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

    
}