#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

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
void printb(Board_p board);

void play(Board_p board, uint8_t index) {
    uint8_t seeds;

    if (board->turn) {
        seeds = board->second[index];
        board->second[index] = 0;
        index += 6;
    } else {
        seeds = board->first[index];
        board->first[index] = 0;
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
    uint8_t first_move;
} Story;

Story solve_recursive(Board_t *board, Story result) {
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
            return solve_recursive(last_board, result); // This is not a starvation, the player can't play anymore
        }
        last_board->turn = !last_board->turn;
        playable(last_board);

        last_board->playable[last_played] = 0; // Starvation
        return solve_recursive(last_board, result);
    }

    result.depth++;
    if (result.depth == 1) {
        result.first_move = last_played;
    }
    return solve_recursive(board, result);
}


void evaluate(Board_t *board, int number_of_batches) {
    double probability_table[6] = {0, 0, 0, 0, 0, 0};
    int victories[6] = {0, 0, 0, 0, 0, 0};
    int loses[6] = {0, 0, 0, 0, 0, 0};
    int null[6] = {0, 0, 0, 0, 0, 0};
    double depth[6] = {0, 0, 0, 0, 0, 0};
    int MinMax[6][2] = {{0, 0},
                        {0, 0},
                        {0, 0},
                        {0, 0},
                        {0, 0},
                        {0, 0}};
    for (int i = 0; i < number_of_batches; i++) {
        Story current_story = {0, 0, 0};
        Board_t board_loop = *board;
        current_story = solve_recursive(&board_loop, current_story);

        if (current_story.result == 1) {
            victories[current_story.first_move]++;
        } else if (current_story.result == -1) {
            loses[current_story.first_move]++;
        } else {
            null[current_story.first_move]++;
        }

        depth[current_story.first_move] += current_story.depth;

        if (current_story.depth > MinMax[current_story.first_move][1]) {
            MinMax[current_story.first_move][1] = current_story.depth;
        }

        if (current_story.depth < MinMax[current_story.first_move][0] || MinMax[current_story.first_move][0] == 0) {
            MinMax[current_story.first_move][0] = current_story.depth;
        }
    }


    for (int i = 0; i < 6; ++i) {
        depth[i] = depth[i] / (double) (victories[i] + null[i] + loses[i]);
        probability_table[i] = (double) victories[i] / (double) (victories[i] + null[i] + loses[i]);
        printf("%d -- v: %d ; n: %d ; d: %d ; txV : %.10lf ; depthMoy : %.10lf ; depthMin : %d ; depthMax %d\n", i, victories[i], null[i], loses[i], probability_table[i], depth[i] , MinMax[i][0], MinMax[i][1]);
    }

    // Choose the best move


}



/* ================================================================================================================= */
/* ===================================================== TEST ====================================================== */
/* ================================================================================================================= */


void test_load_board() {
    uint8_t test_table[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    Board_t test_board = load_board(test_table);

    assert(test_board.first[0] == 1);
    assert(test_board.first[1] == 2);
    assert(test_board.first[2] == 3);
    assert(test_board.first[3] == 4);
    assert(test_board.first[4] == 5);
    assert(test_board.first[5] == 6);
    assert(test_board.second[0] == 7);
    assert(test_board.second[1] == 8);
    assert(test_board.second[2] == 9);
    assert(test_board.second[3] == 10);
    assert(test_board.second[4] == 11);
    assert(test_board.second[5] == 12);
    assert(test_board.turn == 0);
    assert(test_board.score[0] == 13);
    assert(test_board.score[1] == 14);
    printf("Test load Board passed\n");
}

void test_playable() {

    //Everything is possible
    uint8_t test_table_1[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    Board_t test_board = load_board(test_table_1);


    assert(test_board.playable[0] == 1);
    assert(test_board.playable[1] == 1);
    assert(test_board.playable[2] == 1);
    assert(test_board.playable[3] == 1);
    assert(test_board.playable[4] == 1);
    assert(test_board.playable[5] == 1);

    //Everything is possible
    uint8_t test_table_2[14] = {0, 1, 0, 0, 0, 0, 7, 8, 9, 10, 11, 12, 13, 14};
    Board_t test_board_2 = load_board(test_table_2);

    assert(test_board_2.playable[0] == 0);
    assert(test_board_2.playable[1] == 1);
    assert(test_board_2.playable[2] == 0);
    assert(test_board_2.playable[3] == 0);
    assert(test_board_2.playable[4] == 0);
    assert(test_board_2.playable[5] == 0);

    //Everything is possible
    uint8_t test_table_3[14] = {0, 1, 0, 0, 0, 0, 1, 0, 1, 12, 0, 0, 13, 14};
    Board_t test_board_3 = load_board(test_table_3);
    test_board_3.turn = 1;
    playable(&test_board_3);


    assert(test_board_3.playable[0] == 1);
    assert(test_board_3.playable[1] == 0);
    assert(test_board_3.playable[2] == 1);
    assert(test_board_3.playable[3] == 1);
    assert(test_board_3.playable[4] == 0);
    assert(test_board_3.playable[5] == 0);

    printf("Test playable passed\n");


}

void test_count() {

    uint8_t test_table_1[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    Board_t test_board_1 = load_board(test_table_1);

    uint8_t test_count_1 = count_playable(test_board_1.playable);
    assert(test_count_1 == 6);


    uint8_t test_table_2[14] = {0, 1, 0, 0, 0, 0, 7, 8, 9, 10, 11, 12, 13, 14};
    Board_t test_board_2 = load_board(test_table_2);

    uint8_t test_count_2 = count_playable(test_board_2.playable);
    assert(test_count_2 == 1);

    uint8_t test_table_3[14] = {0, 1, 0, 0, 0, 0, 1, 0, 1, 12, 0, 0, 13, 14};
    Board_t test_board_3 = load_board(test_table_3);
    test_board_3.turn = 1;
    playable(&test_board_3);

    uint8_t test_count_3 = count_playable(test_board_3.playable);
    assert(test_count_3 == 3);
}

void test_is_finished() {

    uint8_t test_table_1[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    Board_t test_board_1 = load_board(test_table_1);

    assert(!is_finished(&test_board_1));


    uint8_t test_table_2[14] = {0, 1, 0, 0, 0, 0, 7, 8, 9, 10, 11, 12, 20, 25};
    Board_t test_board_2 = load_board(test_table_2);


    assert(is_finished(&test_board_2));
}

void test_play() {
    uint8_t test_table_1[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    Board_t test_board_1 = load_board(test_table_1);

    play(&test_board_1, 0);

    assert(test_board_1.first[0] == 0);
    assert(test_board_1.first[1] == 3);
    assert(test_board_1.first[2] == 3);
    assert(test_board_1.first[3] == 4);
    assert(test_board_1.first[4] == 5);
    assert(test_board_1.first[5] == 6);
    assert(test_board_1.second[0] == 7);
    assert(test_board_1.second[1] == 8);
    assert(test_board_1.second[2] == 9);
    assert(test_board_1.second[3] == 10);
    assert(test_board_1.second[4] == 11);
    assert(test_board_1.second[5] == 12);


    uint8_t test_table_2[14] = {1, 2, 3, 4, 5, 4, 7, 8, 9, 10, 11, 12, 13, 14};
    Board_t test_board_2 = load_board(test_table_2);

    play(&test_board_2, 5);


    assert(test_board_2.first[0] == 1);
    assert(test_board_2.first[1] == 2);
    assert(test_board_2.first[2] == 3);
    assert(test_board_2.first[3] == 4);
    assert(test_board_2.first[4] == 5);
    assert(test_board_2.first[5] == 0);
    assert(test_board_2.second[0] == 8);
    assert(test_board_2.second[1] == 9);
    assert(test_board_2.second[2] == 10);
    assert(test_board_2.second[3] == 11);
    assert(test_board_2.second[4] == 11);
    assert(test_board_2.second[5] == 12);

    uint8_t test_table_3[14] = {0, 1, 0, 0, 0, 0, 1, 0, 1, 12, 0, 0, 13, 14};
    Board_t test_board_3 = load_board(test_table_3);

    play(&test_board_3, 0);

    assert(test_board_3.first[0] == 0);
    assert(test_board_3.first[1] == 1);
    assert(test_board_3.first[2] == 0);
    assert(test_board_3.first[3] == 0);
    assert(test_board_3.first[4] == 0);
    assert(test_board_3.first[5] == 0);
    assert(test_board_3.second[0] == 1);
    assert(test_board_3.second[1] == 0);
    assert(test_board_3.second[2] == 1);
    assert(test_board_3.second[3] == 12);
    assert(test_board_3.second[4] == 0);
    assert(test_board_3.second[5] == 0);


    uint8_t test_table_4[14] = {0, 1, 0, 0, 0, 0, 1, 0, 1, 12, 0, 0, 13, 14};
    Board_t test_board_4 = load_board(test_table_4);
    test_board_4.turn = 1;

    play(&test_board_4, 0);


    assert(test_board_4.first[0] == 0);
    assert(test_board_4.first[2] == 0);
    assert(test_board_4.first[3] == 0);
    assert(test_board_4.first[4] == 0);
    assert(test_board_4.first[5] == 0);
    assert(test_board_4.second[0] == 0);
    assert(test_board_4.second[1] == 1);
    assert(test_board_4.second[2] == 1);
    assert(test_board_4.second[3] == 12);
    assert(test_board_4.second[4] == 0);
    assert(test_board_4.second[5] == 0);

    printf("Test passed\n");
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
    test_load_board();
    test_playable();
    test_count();
    test_is_finished();
    test_play();

    if (argc != 15) {
        return -1;
    }
    // Build the board
    srand(time(NULL));

    uint8_t board_table[14];
    for (int i = 0; i < 12; i++) {
        board_table[i] = atoi(argv[i + 1]);
    }
    Board_t board1 = load_board(board_table);

    evaluate(&board1, 100000);

    return 0;
}
