#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/* ================================================================================================================= */
/* ===================================================== BOARD ===================================================== */
/* ================================================================================================================= */

typedef struct {
    uint8_t first[6]; // The cases of the first player
    uint8_t second[6]; // The cases of the second player
    bool turn; // The turn of the player
    uint8_t score[2]; // The score of the players
    uint8_t playable[6]; // The playable cases
} Board_t, *Board_p;


/* ================================================================================================================= */

void playable(Board_p board) {
    uint8_t tab_playable[6];
    if (!board->turn) {
        for (int i = 0; i < 6; i++) {
            tab_playable[i] = board->first[i] > 0;
        }
    } else {
        for (int i = 0; i < 6; i++) {
            tab_playable[i] = board->second[i] > 0;
        }
    }
    for (int i = 0; i < 6; i++) {
        board->playable[i] = tab_playable[i];
    }
}

/* ================================================================================================================= */
Board_t load_board(const uint8_t board_table[14]) {
    Board_t board;
    for (int i = 0; i < 6; i++) {
        board.first[i] = board_table[i];
        board.second[i] = board_table[i + 6];
    }
    board.turn = 0;
    board.score[0] = board_table[12];
    board.score[1] = board_table[13];
    playable(&board);
    return board;
}

/* ================================================================================================================= */

uint8_t count_playable(const uint8_t tab_playable[6]) {
    uint8_t count = 0;
    for (int i = 0; i < 6; i++) {
        count += tab_playable[i];
    }
    return count;
}

/* ================================================================================================================= */

void play(Board_p board, uint8_t index) {
    uint8_t seeds;

    if (board->turn) {
        index += 6;
        seeds = board->first[index];
        board->first[index] = 0;
    } else {
        seeds = board->second[index];
        board->second[index] = 0;
    }

    uint8_t played = index;

    while (seeds) {
        index = (index + 1) % 12;
        if (index == played) {
            index = (index + 1) % 12;
        }

        if (index < 6) {
            board->first[index]++;
        } else {
            board->second[index - 6]++;
        }
        seeds--;
    }
    // Check if the last seed was in the player's side and if there is 2 or 3 seeds in the case
    // Remove the seeds and add them to the player's score

    if (board->turn && index < 6) {
        while (board->first[index] == 2 || board->first[index] == 3) {
            board->score[0] += board->first[index];
            board->first[index] = 0;

            if (index == 0) break;
            index = (index - 1) % 6;
        }
    } else if (!board->turn && index >= 6) {
        while (board->second[index - 6] == 2 || board->second[index - 6] == 3) {
            board->score[1] += board->second[index - 6];
            board->second[index - 6] = 0;

            if (index == 6) break;
            index = (index - 1) % 6 + 6;
        }
    }

    board->turn = !board->turn;
    playable(board);
}

/* ================================================================================================================= */

bool is_finished(Board_p board) {
    return board->score[0] >= 25 || board->score[1] >= 25;
}

/* ================================================================================================================= */

void printb(Board_p board) {
    printf("Board_t: turn : %d \n_________________\n", board->turn);
    printf("Playable : ");
    for (int i = 0; i < 6; i++) {
        printf("%d ", board->playable[i]);
    }
    printf("\n");
    for (int i = 0; i < 6; i++) {
        printf("%02d ", board->second[5 - i]);
    }
    printf("\n");
    for (int i = 0; i < 6; i++) {
        printf("%02d ", board->first[i]);
    }
    printf("\n Player 1 : %d\n", board->score[0]);
    printf(" Player 2 : %d\n", board->score[1]);

    int sum_seeds = 0;
    for (int i = 0; i < 6; i++) {
        sum_seeds += board->first[i];
        sum_seeds += board->second[i];
    }
    printf("Sum seeds : %d\n", sum_seeds + board->score[0] + board->score[1]);

    printf("_________________\n");
}

/* ================================================================================================================= */
/* ===================================================== SOLVER ==================================================== */
/* ================================================================================================================= */

typedef struct {
    long int depth;
    int8_t result; // -1 : loss, 0 : draw, 1 : win
    uint8_t moves[1000];
} story;

story solve_recursive(Board_t *board, story result) {
    Board_t *last_board; // The board before the move played this turn
    uint8_t index; // The index (between the possible moves) played this turn
    uint8_t last_played; // The real index played this turn (between 0 and 5)
    uint8_t count; // The number of possible moves this turn


    if (result.depth >= 500) {
        exit(10);
    }

    if (is_finished(board)) {
        // If the game is finished, return the result
        if (board->score[0] > board->score[1]) {
            result.result = 1;
        } else if (board->score[0] < board->score[1]) {
            result.result = -1;
        } else {
            result.result = 0;
        }
        return result;
    }

    count = count_playable(board->playable); // Set the number of possible moves this turn
    if (count == 0) {
        // If there is no possible moves, the game is finished
        if (board->turn == 0) board->score[1] = 48 - board->score[0];
        else board->score[0] = 48 - board->score[1];
        result.result = board->score[0] > board->score[1] ? 1 : board->score[0] < board->score[1] ? -1 : 0;
        return result;
    }

    index = rand() % count; // Choose a random move between the possible ones
    for (int i = 0; i < 6; i++) {
        if (board->playable[i]) {
            if (index == 0) {
                last_board = board; // Save the board before the move played this turn
                last_played = i; // Save the real index played this turn
                play(board, i); // Play the move
                break;
            }
            index--;
        }
    }

    if (count_playable(board->playable) == 0) {
        // Our move may have caused a starvation

        last_board->turn = !last_board->turn;
        playable(last_board);
        if (count_playable(last_board->playable) ==
            0) { // The opponent already had no seeds to play before our move
            result.depth++;
            result.moves[result.depth] = last_played;
            return solve_recursive(last_board, result); // This is not a starvation, the player can't play anymore
        }
        last_board->turn = !last_board->turn;
        playable(last_board);

        last_board->playable[last_played] = 0; // Starvation
        return solve_recursive(last_board, result);
    }

    result.depth++;
    result.moves[result.depth] = last_played;
    return solve_recursive(board, result);
}


/* ================================================================================================================= */
/* ===================================================== MAIN ====================================================== */
/* ================================================================================================================= */

unsigned long mix(unsigned long a, unsigned long b, unsigned long c) {
    a = a - b;
    a = a - c;
    a = a ^ (c >> 13);
    b = b - c;
    b = b - a;
    b = b ^ (a << 8);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 13);
    a = a - b;
    a = a - c;
    a = a ^ (c >> 12);
    b = b - c;
    b = b - a;
    b = b ^ (a << 16);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 5);
    a = a - b;
    a = a - c;
    a = a ^ (c >> 3);
    b = b - c;
    b = b - a;
    b = b ^ (a << 10);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 15);
    return c;
}

int main(int argc, char *argv[]) {
    if (argc != 15) {
        return -1;
    }
    // Build the board
    uint8_t board_table[14];
    for (int i = 0; i < 12; i++) {
        board_table[i] = atoi(argv[i + 1]);
    }
    Board_t board1 = load_board(board_table);


    return 0;
}
