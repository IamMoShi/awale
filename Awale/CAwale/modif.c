//
//Story solve_recursive(Board_t *board, Story result) {
//    Board_t last_board; // The board before the move played this turn
//    uint8_t index; // The index (between the possible moves) played this turn
//    uint8_t last_played; // The real index played this turn (between 0 and 5)
//    uint8_t count; // The number of possible moves this turn
//
//    result.absolute_depth++;
//
//    if (result.depth >= 2000) {
//        exit(10);
//    }
//
//    if (is_finished(board)) {
//        // If the game is finished, return the result
//        if (board->score[0] > board->score[1]) {
//            result.result = 1;
//        } else if (board->score[0] < board->score[1]) {
//            result.result = -1;
//        } else {
//            result.result = 0;
//        }
//        return result;
//    }
//
//    count = count_playable(board->playable); // Set the number of possible moves this turn
//
//    if (count == 0) {
//        // If there is no possible moves, the game is finished
//        if (board->turn == 0) board->score[1] = 48 - board->score[0];
//        else board->score[0] = 48 - board->score[1];
//        result.result = board->score[0] > board->score[1] ? 1 : board->score[0] < board->score[1] ? -1 : 0;
//        return result;
//    }
//
//    index = rand() % count; // Choose a random move between the possible ones
//    for (int i = 0; i < 6; i++) {
//        if (board->playable[i]) {
//            if (index == 0) {
//                last_board = *board; // Save the board before the move played this turn
//                last_played = i; // Save the real index played this turn
//                play(board, i); // Play the move
//                break;
//            }
//            index--;
//        }
//    }
//
//    if (count_playable(board->playable) == 0) {
//        // Our move may have caused a starvation
//
//        last_board.turn = !last_board.turn;
//        playable(&last_board);
//        if (count_playable(last_board.playable) ==
//            0) { // The opponent already had no seeds to play before our move
//            result.depth++;
//            return solve_recursive(&last_board, result); // This is not a starvation, the player can't play anymore
//        }
//        last_board.turn = !last_board.turn;
//        playable(&last_board);
//
//        last_board.playable[last_played] = 0; // Starvation
//        board = &last_board;
//        return solve_recursive(board, result);
//    }
//
//    result.depth++;
//    if (result.depth == 1) {
//        result.first_move = last_played;
//    }
//    return solve_recursive(board, result);
//}