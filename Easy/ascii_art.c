/**
-- The Goal --
In stations and airports you often see this type of screen:
Have you ever asked yourself how it might be possible to simulate this display on a good old terminal? We have: with ASCII art!

-- Rules --

ASCII art allows you to represent forms by using characters. To be precise, in our case, these forms are words. For example, the word "MANHATTAN" could be displayed as follows in ASCII art:
 
# #  #  ### # #  #  ### ###  #  ###
### # # # # # # # #  #   #  # # # #
### ### # # ### ###  #   #  ### # #
# # # # # # # # # #  #   #  # # # #
# # # # # # # # # #  #   #  # # # #
 

​Your mission is to write a program that can display a line of text in ASCII art in a style you are given as input.

-- Input --

Line 1: the width L of a letter represented in ASCII art. All letters are the same width.

Line 2: the height H of a letter represented in ASCII art. All letters are the same height.

Line 3: The line of text T, composed of N ASCII characters.

Following lines: the string of characters ABCDEFGHIJKLMNOPQRSTUVWXYZ? Represented in ASCII art.

-- Output --
The text T in ASCII art.
The characters a to z are shown in ASCII art by their equivalent in upper case.
The characters that are not in the intervals [a-z] or [A-Z] will be shown as a question mark in ASCII art.

-- Constraints --
0 < L < 30
0 < H < 30
0 < N < 200

-- Example 1 --

Input

4
5 
E
 #  ##   ## ##  ### ###  ## # # ###  ## # # #   # # ###  #  ##   #  ##   ## ### # # # # # # # # # # ### ### 
# # # # #   # # #   #   #   # #  #    # # # #   ### # # # # # # # # # # #    #  # # # # # # # # # #   #   # 
### ##  #   # # ##  ##  # # ###  #    # ##  #   ### # # # # ##  # # ##   #   #  # # # # ###  #   #   #   ## 
# # # # #   # # #   #   # # # #  #  # # # # #   # # # # # # #    ## # #   #  #  # # # # ### # #  #  #       
# # ##   ## ##  ### #    ## # # ###  #  # # ### # # # #  #  #     # # # ##   #  ###  #  # # # #  #  ###  #  

Output

### 
#   
##  
#   
### 

-- Example 2 --

Input

4
5
MANHATTAN
 #  ##   ## ##  ### ###  ## # # ###  ## # # #   # # ###  #  ##   #  ##   ## ### # # # # # # # # # # ### ### 
# # # # #   # # #   #   #   # #  #    # # # #   ### # # # # # # # # # # #    #  # # # # # # # # # #   #   # 
### ##  #   # # ##  ##  # # ###  #    # ##  #   ### # # # # ##  # # ##   #   #  # # # # ###  #   #   #   ## 
# # # # #   # # #   #   # # # #  #  # # # # #   # # # # # # #    ## # #   #  #  # # # # ### # #  #  #       
# # ##   ## ##  ### #    ## # # ###  #  # # ### # # # #  #  #     # # # ##   #  ###  #  # # # #  #  ###  #  

Output

# #  #  ### # #  #  ### ###  #  ###  
### # # # # # # # #  #   #  # # # #  
### ### # # ### ###  #   #  ### # #  
# # # # # # # # # #  #   #  # # # #  
# # # # # # # # # #  #   #  # # # # 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef LOCAL
#include <assert.h>
#endif

typedef struct {
    char pattern[30][30];  // Size array for the pattern (max size from constraints)
} Letter;

char *input1(void);
char *output1(void);
char *input2(void);
char *output2(void);
//char *solve_ascii_art(); // Add your solution function declaration

// Convert letter to index (0-25 for A-Z, 26 for non-letters)
int get_letter_index(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    return 26; // Return 26 for non-letter characters (?)
}

char *appendLetterPattern(char *dest, int H, int L, Letter letter) {
    for (int i = 0; i < H; i++) {
        strncat(dest + i * (L + 1), letter.pattern[i], (size_t) L);
        dest[i * (L + 1) + L] = '\n'; // Add newline after each row
    }
    return dest;
}

void addLetter2AsciiArt(int index, int text_length, Letter letter, Letter ascii_art[text_length]) {
    ascii_art[index] = letter;
}

void printPattern(FILE *out, int H, int L, int text_length, Letter ascii_art[text_length]) {
        for (int h = 0; h < H; h++) {
            for (int t = 0; t < text_length; t++) {
                for (int l = 0; l < L; l++) {
                    fprintf(out, "%c", ascii_art[t].pattern[h][l]);
                }
            }
            fprintf(out, "\n");
        }
    
}

/*
#ifdef LOCAL
// Test function for sample input 2
void test_input2(void) {
    TEST_ASSERT_EQUAL_STRING(output2(), solve_ascii_art());
}
#endif
*/

int TEST_ASSERT_EQUAL_STRING(const char *expected, const char *actual) {
#ifdef LOCAL
  if (!actual || !expected) {
    fprintf(stderr, "Test setup error: null pointer\n");
    return 1;
  }
  if (strcmp(expected, actual) != 0) {
      fprintf(stderr, "Assertion failed:\nExpected:\n%s\nGot:\n%s\n", expected, actual);
      return 1;
  }

  fprintf(stderr,"\nTest passed!\n");
#endif
  free((void *)actual);
  return 0;
}

void copy_pattern_to_result(int H, int L, int text_length, Letter ascii_art[text_length], char **result) {
    size_t total_size = (size_t) (H * (L * text_length + 1) + 1); // +1 for null terminator
    *result = (char *)malloc(total_size);
    if (*result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    memset(*result, 0, total_size); // Initialize the result buffer

    for (int h = 0; h < H; h++) {
        for (int t = 0; t < text_length; t++) {
            for (int l = 0; l < L; l++) {
                size_t pos = (size_t) (h * (L * text_length + 1) + t * L + l);
                (*result)[pos] = ascii_art[t].pattern[h][l];
            }
        }
        size_t newline_pos = (size_t) (h * (L * text_length + 1) + L * text_length);
        (*result)[newline_pos] = '\n';
    }
    (*result)[total_size - 1] = '\0'; // Null-terminate the string
}

char *solve_ascii_art(char *input) {
  // This is a debug function to help you test your code locally.
#ifdef LOCAL
      //freopen("input.txt", "r", stdin);
      //char *input = input2();
      FILE *temp = tmpfile();       // create temporary file
      fputs(input, temp);           // write input to it
      rewind(temp);                 // reset file pointer to beginning

      // redirect stdin
      stdin = temp;
#endif

    int L;
    scanf("%d", &L);
    int H;
    scanf("%d", &H); fgetc(stdin);
    char T[257] = "";
    scanf("%[^\n]", T); fgetc(stdin);

    Letter letters[27]; // 26 letters + '?'
    for (int i = 0; i < H; i++) {
        char ROW[1025] = "";
        scanf("%[^\n]", ROW); fgetc(stdin);

        // Store each letter's pattern
        for (int j = 0; j < 27; j++) {
            for (int k = 0; k < L; k++) {
                // Fill the pattern for letter j at row i, column k
                letters[j].pattern[i][k] = ROW[j * L + k];
            }
        }
    }

    int text_length = (int) strlen(T);

    char *result; // Buffer for the final result
    //memset(result, 0, sizeof(result)); // Initialize the result buffer

    Letter ascii_art[text_length];

    // Print each character's ASCII art
    for (int i = 0; i < text_length; i++) {
        int letter_index = get_letter_index(T[i]);
        // Now letter_index is 0-25 for A-Z, or 26 for non-letters
        // Use letters[letter_index] to get the pattern
        addLetter2AsciiArt(i, text_length, letters[letter_index], ascii_art);
        
    }

    printPattern(stdout, H, L, text_length, ascii_art);

    copy_pattern_to_result(H, L, text_length, ascii_art, &result);

    return result;
}

int main(void) {

  char *result = solve_ascii_art(input2());

  return TEST_ASSERT_EQUAL_STRING(output2(), result);
}


char *input1() {
    return "4\n"
           "5\n"
           "E\n"
           " #  ##   ## ##  ### ###  ## # # ###  ## # # #   # # ###  #  ##   #  ##   ## ### # # # # # # # # # # ### ### \n"
           "# # # # #   # # #   #   #   # #  #    # # # #   ### # # # # # # # # # # #    #  # # # # # # # # # #   #   # \n"
           "### ##  #   # # ##  ##  # # ###  #    # ##  #   ### # # # # ##  # # ##   #   #  # # # # ###  #   #   #   ## \n"
           "# # # # #   # # #   #   # # # #  #  # # # # #   # # # # # # #    ## # #   #  #  # # # # ### # #  #  #       \n"
           "# # ##   ## ##  ### #    ## # # ###  #  # # ### # # # #  #  #     # # # ##   #  ###  #  # # # #  #  ###  #  \n";
}

char *output1() {
  return  "### \n"
          "#   \n" 
          "##  \n"
          "#   \n"
          "### \n";
}

 

// Sample input 2
char *input2() {
    return "4\n"
           "5\n"
           "MANHATTAN\n"
           " #  ##   ## ##  ### ###  ## # # ###  ## # # #   # # ###  #  ##   #  ##   ## ### # # # # # # # # # # ### ### \n"
           "# # # # #   # # #   #   #   # #  #    # # # #   ### # # # # # # # # # # #    #  # # # # # # # # # #   #   # \n"
           "### ##  #   # # ##  ##  # # ###  #    # ##  #   ### # # # # ##  # # ##   #   #  # # # # ###  #   #   #   ## \n"
           "# # # # #   # # #   #   # # # #  #  # # # # #   # # # # # # #    ## # #   #  #  # # # # ### # #  #  #       \n"
           "# # ##   ## ##  ### #    ## # # ###  #  # # ### # # # #  #  #     # # # ##   #  ###  #  # # # #  #  ###  #  \n";
}

// Expected output 2
char *output2() {
  return "# #  #  ### # #  #  ### ###  #  ### \n"
         "### # # # # # # # #  #   #  # # # # \n"
         "### ### # # ### ###  #   #  ### # # \n"
         "# # # # # # # # # #  #   #  # # # # \n" 
         "# # # # # # # # # #  #   #  # # # # \n";
}

