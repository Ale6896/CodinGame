#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 * ---
 * Hint: You can use the debug stream to print initialTX and initialTY, if Thor seems not follow your orders.
 **/

int main()
{
    // the X position of the light of power
    int light_x;
    // the Y position of the light of power
    int light_y;
    // Thor's starting X position
    int initial_tx;
    // Thor's starting Y position
    int initial_ty;
    scanf("%d%d%d%d", &light_x, &light_y, &initial_tx, &initial_ty);

    // Thor's actual X position
    int actual_tx = initial_tx;
    // Thor's actual Y position
    int actual_ty = initial_ty;

    // game loop
    while (1) {
        // The remaining amount of turns Thor can move. Do not remove this line.
        int remaining_turns;
        scanf("%d", &remaining_turns);

        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "Debug messages...\n");

        if (actual_ty < light_y){
            printf("S");
            actual_ty += 1;
        }
        if (actual_ty > light_y) {
            printf("N");
            actual_ty -= 1;
        }
        if (actual_tx < light_x) {
            printf("E");
            actual_tx += 1;
        }
        if (actual_tx > light_x) {
            printf("W");
            actual_tx -= 1;
        }
        printf("\n");


        // A single line providing the move to be made: N NE E SE S SW W or NW
    }

    return 0;
}
