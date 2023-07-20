//
//  main.c
//  project1
//
//  Created by Courtney Lee on 8/25/22.
//

#include <stdio.h>
#include <math.h>

int main() {
    int numArray[10];
    int size = 10;
    double sum = 0.0;
    double avg = 0.0;
    
    printf("Enter 10 numbers");
    //add elements to numArray
    int i = 0;
    for (i = 0; i < size; ++i) {
        scanf("%d",&numArray[i]);
    }
    //square root each element and add to sum
    int j = 0;
    for (j = 0; j < size; ++j) {
        sum += sqrt (numArray[j]);
    }
    //divide sum by 10 to get the average
    avg = sum / 10.0;
    
    printf("The average of the square roots is %lf",avg);
    printf("\n");
    return 0;
    
}
