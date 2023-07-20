#include <stdio.h>
#include "scheduler.h"

/*
 * Print the stats 
 */
void calculate_stats(task_t task_list[], int size, float cpu_usage) {

    float waitingTime = 0;
    float responseTime = 0; 
    float turnaroundTime = 0;


//calculate the  times using the recorded times from each policy method
    for (int i = 0; i < size; i++) {

        waitingTime += (float)(task_list[i].finish_time - task_list[i].arrival_time - task_list[i].burst_time);
        turnaroundTime += (float)(task_list[i].finish_time - task_list[i].arrival_time);
        responseTime += (float)(task_list[i].start_time - task_list[i].arrival_time);

    }
    
    //calculate averages 
    waitingTime = (size != 0) ? waitingTime/(float)size : 0;
    turnaroundTime = (size != 0) ? turnaroundTime / (float)size : 0;
    responseTime = (size != 0) ? responseTime / (float)size : 0;


    printf("Average waiting time: %.2f\n", waitingTime);
    printf("Average response time: %.2f\n", responseTime);
    printf("Average turnaround time: %.2f\n", turnaroundTime);
    printf("Overall CPU usage: %5.2f%%\n", cpu_usage * 100);

}

