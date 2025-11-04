#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int goLeft(int H, int W, int x, int y, char matrix[H][W]) {

    while (matrix[y][x] == '-') {
      char current = matrix[y][x];
        --x;
    }

    return x;
}

int goRight(int H, int W, int x, int y, char matrix[H][W]) {

    while (matrix[y][x] == '-') {
      char current = matrix[y][x];
        ++x;
    }

    return x;
}

int goDown(int y) {
    return ++y;
}

char play(int H, int W, int index, char matrix[H][W]) {
    
    int x = index;
    int y = 1;

    while (y < H-1) {
      char current = matrix[y][x];

        if (matrix[y][x+1] == '-') {
            x = goRight(H,W,x+1,y,matrix);
        } else if (matrix[y][x-1] == '-') {
            x = goLeft(H, W, x-1, y, matrix);
        }
        y = goDown(y);
    }

    return matrix[y][x];
}

int main() {

    // Simulate input redirection from a file
    if (getenv("LOCAL")) {
      freopen("input.txt", "r", stdin);
    }

    int W;
    int H;
    scanf("%d%d", &W, &H); fgetc(stdin);

    char matrix[H][W];

    for (int i = 0; i < H; i++) {
        char line[1025] = "";
        scanf("%[^\n]", line); fgetc(stdin);
        strcpy(matrix[i], line);
    }

    for (int i = 0; i < W; ++i) {
        char c = matrix[0][i];
        if (c != ' ' && c != '-') {
            char n = play(H, W, i, matrix);
            printf("%c%c\n", c,n);
        } 
    }

    return 0;
    }