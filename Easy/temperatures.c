/*

-- Goal --
In this exercise, you have to analyze records of temperature to find the closest to zero.

-- Rules --
Write a program that prints the temperature closest to 0 among input data. If two numbers are equally close to zero, positive integer has to be considered closest to zero (for instance, if the temperatures are -5 and 5, then display 5).

-- Game Input --
Your program must read the data from the standard input and write the result on the standard output.
Input

-- Input --
Line 1: N, the number of temperatures to analyze

Line 2: A string with the N temperatures expressed as integers ranging from -273 to 5526

-- Output --
Display 0 (zero) if no temperatures are provided. Otherwise, display the temperature closest to 0.

-- Constraints --
0 ≤ N < 10000

-- Example --

Input:
5
1 -2 -8 4 5

Output:
1

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>  // for INT_MAX

int main()
{
    // the number of temperatures to analyse
    int n;
    scanf("%d", &n);
    int num = INT_MAX;
    for (int i = 0; i < n; i++) {
        // a temperature expressed as an integer ranging from -273 to 5526
        int t;
        scanf("%d", &t);
        
        // If |t| <= |num|
        if (abs(t) <= abs(num)) {
            /* 
            ** if |t| < |num| then num = t 
            ** else if t > 0 then num = t
            **/
            num = (abs(t) < abs(num)) ? t : ((t > num) ? t : num);
        }

    }

    printf("%d\n", (n > 0) ? num : 0);

    return 0;
}
