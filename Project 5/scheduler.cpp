/*
* COMP 3500
* Courtney Lee
* Project 5
*
*************************************************
* Compile and Run instructions (Also in README) *
*************************************************
* To compile: "make"
*
* To run with FCFS policy
* ./scheduler task.list FCFS
*
* To run with RR policy
* ./scheduler task.list RR (time quantum)
*
* To run with SRTF
* ./scheduler task.list SRTF 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue> 
#include <vector>

#include "scheduler.h"
#include "open.h"
#include "read.h"
#include "stats.h"

/*
* Simulate First Come First Serve Policy
*/
float fcfs_policy(task_t task_array[], u_int count) {
    int time = 0;
    int i = 0;
    int waste = 0;

    while (i < count) {

        if (task_array[i].arrival_time <= time) {
        printf("<time %i> process %i is running\n", time, task_array[i].pid);
            //record the start time
            if (task_array[i].start_time == -1) {
                task_array[i].start_time = time;
            }
            
            task_array[i].remaining_time--;
            time++;

        
             if (task_array[i].remaining_time == 0) {
                printf("<time %i> process %i is finsihed...\n", time, task_array[i].pid);
                task_array[i].finish_time = time; 
                i++;
        }
        }

        else {
            printf("<time %i> no process is running right now\n", time);
            time++;
            waste++;
        }
    }
    //processes finish
    printf("<time %i> All processes finish ......\n", time);
    return (time != 0) ? (float)(time - waste) / (float) time : 0;
}

/*
* Simulate Round Robin Policy
*/
float rr_policy(task_t task_array[], u_int count, u_int quantum) {
    int time = 0;
     int i = 0;
     int j = 0;
     int finish = 0; 
     int waste = 0;

     std::queue<task_t *> process_queue;
    /*
    *https://en.cppreference.com/w/cpp/container/queue
    *references for C++ queue syntax and usees.
    */
     
     while (finish < count) {
        while (i < count && task_array[i].arrival_time <= time) {
            process_queue.push(&task_array[i]);
            i++;
        }

        if (!process_queue.empty()) {
            task_t *current_process = process_queue.front();
            //finish process
            if (current_process -> remaining_time == 0) {
                printf("<time %i> process %i is finished...\n", time, current_process->pid);

                current_process -> finish_time = time;
                process_queue.pop();
                finish++;
                j = 0;
            }

            else {
                if (j < quantum) {
                    printf("<time %i> process %i is running\n", time, current_process->pid);
                    //record start time
                    if (current_process -> start_time == -1) {
                        current_process -> start_time = time;
                    }
                    current_process -> remaining_time--;
                    time ++;
                    j++;

                }
                //next process
                else{
                    j = 0;
                     process_queue.pop();
                     process_queue.push(current_process);
                }
            }
        }

        else {
             printf ("<time %i> no process is running right now\n", time);
            time++;
            waste++;
        }
    }

     printf ("<time %i> All processes finish ......\n", time);
    return (time != 0) ? (float)(time - waste) / (float)time : 0;

}

/*
* Simulate Shortest Remaining Time First policy
*/
float srtf_policy(task_t task_array[], u_int count) {
    int time = 0;
    int i = 0;
    int finish = 0;
    int waste = 0;

    while (finish < count) {
        int shortest = -1;
        int shortest_index = -1;

        // find the shortest task
        for (i = 0; i < count; i++) {
            if (task_array[i].arrival_time <= time && task_array[i].remaining_time > 0) {

                //which has the shortest remaining time of the processes
                if (shortest == -1 || task_array[i].remaining_time < shortest) {
                    shortest = task_array[i].remaining_time;
                    shortest_index = i;
                }
            }
        }
        // if there is a shortest task
        if (shortest_index != -1) {
            // run the task
        if (task_array[shortest_index].start_time == -1) {
            task_array[shortest_index].start_time = time;
        }
            printf("<time %d> process %d is running\n", time, task_array[shortest_index].pid);
            task_array[shortest_index].remaining_time--;
            time++;

            // if the task is finished
            if (task_array[shortest_index].remaining_time == 0) {
                printf("<time %d> process %d is finished...\n", time, task_array[shortest_index].pid);
        task_array[shortest_index].finish_time = time;
                finish++;
            }
        } else {
            // if there is no shortest task, waste time
            printf("<time %d> No process is running...\n", time);
            time++;
            waste++;
        }
    }
    printf("<time %i> All processes finish ......\n", time);
    return (time != 0) ? (float)(time - waste) / (float) time : 0;

}

/*
* Main Function to simulate the different scheduling policies
*/
int main(int argc, char *argv[]) {
 char *fileName;
 FILE *fp;
 task_t task_array[MAX_TASK_NUM];
 int errorCode;
 u_int i;
 u_int count;
 float cpu_usage;

 // Error Handling

 errorCode = open_file(argv[1], &fp);
 if (errorCode == 1) {
    return EXIT_FAILURE;
 }

if (argc == 1 || argc > 4) {
    printf("Usage: command fileName [FCFS|RR|SRFT] [time_quantum]\n");
    return EXIT_FAILURE;

}

if (argc < 2) {
    printf("Usage: input <fileName>\n");
    return EXIT_FAILURE;
}

if (strcmp(argv[2], "RR") == 0) {
    if (argc <4) {
        printf("Please enter time_quantum for RR policy\n");
        return EXIT_FAILURE;
    }
}


 printf("Scheduling Policy: %s\n", argv[2]);

 read_file(fp, task_array, &count);

 printf("There are %d tasks loaded from \"%s\". Press enter to continue...", count, argv[1]);
 getchar();
 printf("==================================================================\n");

 //call the prompted policy

 if (strcmp(argv[2], "FCFS") == 0) {
    cpu_usage = fcfs_policy(task_array, count);
 }
 
 else if (strcmp(argv[2], "RR") == 0) {
    cpu_usage = rr_policy(task_array, count, atoi(argv[3]));
 }

    else if(strcmp(argv[2], "SRTF") == 0) {
    cpu_usage = srtf_policy(task_array, count);
 }

 else {
    printf("%s is not a valid option\n" , argv[2]);
    return EXIT_FAILURE;
 }
 printf("==================================================================\n");

 calculate_stats(task_array, count, cpu_usage);

 printf("==================================================================\n");

 fclose(fp);
 return EXIT_SUCCESS;
}
