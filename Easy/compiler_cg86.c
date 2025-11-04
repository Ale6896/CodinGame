/*

-- Goal --

CG86 is a minimal arithmetic compiler targeting a fictional assembly language. It supports only addition and subtraction, using a single register cgx, initialized to zero.

To reduce the instruction count, repeated operations can be compressed using the REPEAT instruction.

Instruction Set:

ADD cgx X: Adds X to cgx
SUB cgx X: Subtracts X from cgx
REPEAT N: Repeats the next instruction N times
EXIT: Terminates the program

The REPEAT instruction applies only to identical operations — that is, instructions of the same type and operand (e.g., multiple instances of ADD cgx 5).

The REPEAT instruction must come before the instruction it repeats.

Identical operations must be compressed using the REPEAT instruction, even if they do not appear consecutively in the original expression.

Instructions must preserve the original order of computation and may only be optimized using the REPEAT instruction. Reordering or merging of non-identical operations is not allowed.

The program must end with an EXIT instruction.

-- Input --
A single line containing a space-separated arithmetic expression, alternating between integers and + or - operators.

-- Output --
A sequence of CG86 instructions, with one instruction per line, following the specified order and optimization rules.

-- Constraints --
The length of the input expression is less than 1024 characters.
All numbers in the input expression are integers between 1 and 9 (inclusive).
The expression is well-formed and contains at least two numbers and one operator.

-- Example --

Input:

    1 + 2 + 2 + 2 + 2 - 3

Output:

    ADD cgx 1
    REPEAT 4
    ADD cgx 2
    SUB cgx 3
    EXIT


*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../../C/Unity/src/unity.h"

struct Number {
    char sign;
    char magnitude;
    struct Number *next;
};

struct Number* createNumber(char sign, char magnitude) {

    struct Number* new_number = (struct Number*)malloc(sizeof(struct Number));

    new_number->sign = sign;
    new_number->magnitude = magnitude;
    new_number->next = NULL;

    return new_number;
}

void addToQueue(struct Number** list, struct Number* new) {
    if (*list == NULL) {
        *list = new;
        return;
    }

    struct Number* Aux = *list;
    while (Aux->next != NULL) {
        if (Aux->magnitude == new->magnitude && Aux->sign == new->sign) {
            struct Number* Temp = Aux->next;
            Aux->next = new;
            new->next = Temp;
            return;
        }
        Aux = Aux->next;
    }

    Aux->next = new;
}

char* removeSpaces(const char *expression) {
    char *new_expression = malloc(strlen(expression) + 2); // +1 for '\0', +1 for optional '+'
    int j = 0;
    if (isdigit(expression[0])) new_expression[j++] = '+';

    for (int i = 0; expression[i]; i++) {
        if (expression[i] != ' ') new_expression[j++] = expression[i];
    }
    new_expression[j] = '\0';
    return new_expression;  // caller must free()
}

void destroyQueue(struct Number** list) {
    struct Number* Aux = *list;

    while (Aux != NULL) {
        *list = (*list)->next;
        free(Aux);
        Aux = *list;
    }

    *list = NULL;  // now the caller’s list pointer is safely cleared
}


void transform_expression(const char *input, char *output) {
    int count_2 = 0;
    int value;
    char op;
    const char *ptr = input;
    char buffer[256] = "";

    sscanf(ptr, "%d", &value);
    sprintf(buffer + strlen(buffer), "ADD cgx %d\n", value);
    ptr = strchr(ptr, ' ') + 1;

    while (sscanf(ptr, "%c %d", &op, &value) == 2) {
        if (op == '+' && value == 2) {
            count_2++;
        } else {
            if (count_2 > 0) {
                sprintf(buffer + strlen(buffer), "REPEAT %d\nADD cgx 2\n", count_2);
                count_2 = 0;
            }
            if (op == '+') {
                sprintf(buffer + strlen(buffer), "ADD cgx %d\n", value);
            } else if (op == '-') {
                sprintf(buffer + strlen(buffer), "SUB cgx %d\n", value);
            }
        }
        ptr = strchr(ptr, ' ') + 1;
    }

    if (count_2 > 0) {
        sprintf(buffer + strlen(buffer), "REPEAT %d\nADD cgx 2\n", count_2);
    }

    strcat(buffer, "EXIT\n");
    strcpy(output, buffer);
}

void test(void) {
    const char *input = "1 + 2 + 2 + 2 + 2 - 3";
    char output[256];
    const char *expected =
        "ADD cgx 1\n"
        "REPEAT 4\n"
        "ADD cgx 2\n"
        "SUB cgx 3\n"
        "EXIT\n";

    transform_expression(input, output);
    TEST_ASSERT_EQUAL_STRING(expected, output);
}

int main()
{
    char expression[1025] = "";
    /* If LOCAL is set use the hardcoded test string; otherwise read from stdin (CodinGame) */
    if (getenv("LOCAL")) {
        strcpy(expression, "1 + 2 + 2 + 2 + 2 - 3");
        /* or read from a local file:
           FILE *f = fopen("input.txt","r"); ... */
    } else {
        scanf("%[^\n]", expression);
    }

    // Write an answer using printf(). DON'T FORGET THE TRAILING \n
    // To debug: fprintf(stderr, "Debug messages...\n");

    char *new_expression = removeSpaces(expression);

    struct Number* list = NULL;

    for (int i = 0; i < strlen(new_expression); i += 2) {
        struct Number* number = createNumber(new_expression[i], new_expression[i+1]);
        addToQueue(&list, number);
    }

    int n = 1;
    struct Number* Aux = list;

    while (Aux != NULL) {
        if (Aux->next != NULL && Aux->sign == Aux->next->sign && Aux->magnitude == Aux->next->magnitude) {
            ++n;
            Aux = Aux->next;
            continue;
        }

        if (n > 1) {
            printf("REPEAT %d\n", n);
            n = 1;
        }

        if (Aux->sign == '+') {
            printf("ADD cgx %c\n", Aux->magnitude);
        } else if (Aux->sign == '-') {
            printf("SUB cgx %c\n", Aux->magnitude);
        }

        Aux = Aux->next;
    }

    printf("EXIT\n");

    free(new_expression);
    destroyQueue(&list);
    return 0;
}
