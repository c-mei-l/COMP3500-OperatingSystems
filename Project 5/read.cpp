#include <stdio.h>
#include "scheduler.h"

/*
 * read from the input file
 */
void read_file(FILE *file_descriptor, task_t task_list[], u_int *size) {
    int num = 0;

    while (fscanf(file_descriptor, "%u %u %u", &task_list[num].pid, 
        &task_list[num].arrival_time, &task_list[num].burst_time)!= EOF) {
            num++;
            }

    *size = num;

    for (int i = 0; i < num; i++) {
        task_list[i].remaining_time = task_list[i].burst_time;
        task_list[i].start_time = -1;
        task_list[i].finish_time = -1;
    }
}