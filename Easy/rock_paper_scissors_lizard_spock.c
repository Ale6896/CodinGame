/*

-- Goal --
An international Rock Paper Scissors Lizard Spock tournament is organized, all players receive a number 
when they register.

Each player chooses a sign that he will keep throughout the tournament among:
Rock (R)
Paper (P)
sCissors (C)
Lizard (L)
Spock (S)

Scissors cuts Paper
Paper covers Rock
Rock crushes Lizard
Lizard poisons Spock
Spock smashes Scissors
Scissors decapitates Lizard
Lizard eats Paper
Paper disproves Spock
Spock vaporizes Rock
Rock crushes Scissors
and in case of a tie, the player with the lowest number wins (it's scandalous but it's the rule).

-- Example --

4 R \
      1 P \
1 P /      \
             1 P
8 P \      /     \
      8 P /       \
3 R /              \
                     2 L
7 C \              /
      5 S \       /
5 S /      \     /
             2 L
6 L \      /
      2 L /
2 L /

The winner of the tournament is player 2. Before winning, he faced player 6, then player 5 and finally player 1.

-- Input --
Line 1: an integer N representing the number of participants in the competition
Lines 2 to N+1: an integer NUMPLAYER indicating the player number (players have distinct numbers 
between 1 and N) followed by a letter 'R', 'P', 'C', 'L' or 'S' indicating the chosen sign SIGNPLAYER

-- Output --
Line 1: the number of the winner
Line 2: the list of its opponents separated by spaces

-- Constraints --
N is a 2^k value (2, 4, 8, 16, ..., 1024)
2 ≤ N ≤ 1024

-- Example --
Input
    8
    4 R
    1 P
    8 P
    3 R
    7 C
    5 S
    6 L
    2 L

Output
    2
    6 5 1

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// --- Structs and Typedefs ---

typedef struct Player {
    int number;                  // Unique player number
    char sign;                   // One of R, P, C, L, S
    struct Player* defeated;     // Head of defeated players list
    struct Player* next;         // Next player in current round
    struct Player* previous;     // Optional back pointer (not used in logic)
} *PtrPlayer;


// Registry of Player pointers
// This is used to free all players
typedef struct {
    PtrPlayer* items;  // array of Player*
    int count;
} PlayerRegistry;

// --- Player Creation and Tournament Setup ---

// Creates a new player node
PtrPlayer createPlayer(int number, char sign, PlayerRegistry* registry) {
    PtrPlayer new_player = (PtrPlayer)malloc(sizeof(struct Player));
    if (!new_player) exit(EXIT_FAILURE);

    new_player->number = number;
    new_player->sign = sign;
    new_player->defeated = NULL;
    new_player->next = NULL;
    new_player->previous = NULL;

    registry->items = realloc(registry->items, sizeof(PtrPlayer) * (registry->count + 1));
    registry->items[registry->count++] = new_player;

    return new_player;
}

// Adds a player to the end of the current round's linked list
void addToTournament(PtrPlayer* contest, PtrPlayer new_player) {
    if (*contest == NULL) {
        *contest = new_player;
        return;
    }

    PtrPlayer aux = *contest;
    while (aux->next != NULL) {
        aux = aux->next;
    }

    aux->next = new_player;
    new_player->previous = aux;
}


// --- Game Logic ---

// Returns a 0-based index for the sign, used for the fight matrix
int getKeyValue(char key) {
    switch (key) {
        case 'R': return 0; // Rock
        case 'P': return 1; // Paper
        case 'C': return 2; // Scissors
        case 'L': return 3; // Lizard
        case 'S': return 4; // Spock
        default: return -1;
    }
}

// Returns 1 if p1 wins, -1 if p2 wins, 0 for tie
int fight(PtrPlayer p1, PtrPlayer p2) {

    int p1_sign = getKeyValue(p1->sign);
    int p2_sign = getKeyValue(p2->sign);

    if (p1_sign == -1 || p2_sign == -1) {
        fprintf(stderr,"Invalid sign(s): p1=%c, p2=%c\n", p1->sign, p2->sign);
        exit(EXIT_FAILURE);  // or handle as needed
    }

    // Matrix of outcomes:
    int outcome[5][5] = {
        // R   P   C   L   S
        {  0, -1,  1,  1, -1 }, // R
        {  1,  0, -1, -1,  1 }, // P
        { -1,  1,  0,  1, -1 }, // C
        { -1,  1, -1,  0,  1 }, // L
        {  1, -1,  1, -1,  0 }  // S
    };

    return outcome[p1_sign][p2_sign];
}

// Adds the loser to the winner's defeated list
void setWinner(PtrPlayer temp, PtrPlayer winner, PtrPlayer loser) {
    loser->next = NULL;
    loser->previous = NULL;

    winner->previous = temp->previous;
    winner->next = temp->next;

    if (winner->defeated == NULL) {
        winner->defeated = loser;
    } else {
        PtrPlayer aux = winner->defeated;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = loser;
        loser->previous = aux;
    }
}

void disconectPlayers(PtrPlayer p1, PtrPlayer p2) {
    p1->next = NULL;
    p1->previous = NULL;
    p2->next = NULL;
    p2->previous = NULL;
}

// Pairs up players, determines winners, and builds next round
void matchPlayers(PtrPlayer* players) {
    PtrPlayer new_round = NULL; // new_round is used for the next contest among winners of current round
    PtrPlayer current = *players;

    while (current != NULL && current->next != NULL) {
        PtrPlayer p1 = current;
        PtrPlayer p2 = current->next;
        current = p2->next;

        // Create a temporary player to hold  place for the winner
        PtrPlayer temp = p1;
        temp->previous = p1->previous;
        temp->next = p2->next;

        disconectPlayers(p1, p2);

        int result = fight(p1, p2);
        PtrPlayer winner, loser;

        if (result == 0) {
            winner = (p1->number < p2->number) ? p1 : p2;
            loser  = (winner == p1) ? p2 : p1;
        } else if (result == 1) {
            winner = p1;
            loser  = p2;
        } else {
            winner = p2;
            loser  = p1;
        }

        setWinner(temp, winner, loser);
        addToTournament(&new_round, winner); // Adds winner to next round
    }

    *players = new_round;
}

// --- Memory Management ---

// Free all Players from the registry
void freeAllPlayers(PlayerRegistry* registry) {
    for (int i = 0; i < registry->count; i++) {
        free(registry->items[i]);
    }
    free(registry->items);
    registry->items = NULL;
    registry->count = 0;
}

// --- Main Tournament Driver ---

int main()
{
    int N;
    scanf("%d", &N);

    PtrPlayer contest = NULL;
    PlayerRegistry registry = {NULL, 0};

    for (int i = 0; i < N; i++) {
        int NUMPLAYER;
        char SIGNPLAYER[2] = "";
        scanf("%d%s", &NUMPLAYER, SIGNPLAYER);

        PtrPlayer new_player = createPlayer(NUMPLAYER, SIGNPLAYER[0], &registry);

        addToTournament(&contest, new_player);

    }

    // Run rounds until only one player remains
    while (contest->next != NULL && contest->next != NULL) {
        matchPlayers(&contest);
    }

    // Final winner
    PtrPlayer winner = contest;
    printf("%d\n", winner->number);

    // Print defeated players
    PtrPlayer aux = winner->defeated;
    while (aux != NULL) {
        printf("%d", aux->number);
        aux = aux->next;
        if (aux != NULL) printf(" ");
    }
    printf("\n");

    // Safe cleanup
    freeAllPlayers(&registry);

    return 0;
}
