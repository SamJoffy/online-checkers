#include "board.c"

/** Tests that validation of the correct color being moved works */
int testMoveColorCheck(char *board) {
    return isValidMove(board, "R60D") && !isValidMove(board, "B60D");
}

/** Tests that validation of the pieces' move direction works */
int testMoveDirectionCheck(char *board) {
    return !isValidMove(board, "R60S") && isValidMove(board, "B39A");
}

/** Tests that moving pieces around the board works */
int testMovePiece(char *board) {
    movePiece(board, "B39A");
    movePiece(board, "R60D");
    movePiece(board, "R51D");
    printf("\n");
    print_board(board);
    return getCharAt(board, 8, 4) == 'B' && getCharAt(board, 2, 4) == 'R';
}

/** Tests that validation of the pieces' jump moves are correct */
int testJumpMoveCheck(char *board) {
    return isValidMove(board, "B31S") && !isValidMove(board, "R43D");
}

/** Tests that jumping pieces works */
int testJumpMove(char *board) {
    movePiece(board, "B31S");
    return getCharAt(board, 2, 4) == 'O' && getCharAt(board, 3, 5) == 'B' && getPieceCount('R') == 19;
}

int main() {
    char *board = create_board();
    print_board(board);
    if (!testMoveColorCheck(board)) {
        printf("TEST MOVE COLOR CHECK FAILED");
        return 0;
    }

    if (!testMoveDirectionCheck(board)) {
        printf("TEST MOVE DIRECTION CHECK FAILED");
        return 0;
    }

    if (!testMovePiece(board)) {
        printf("TEST MOVE PIECE FAILED");
        return 0;
    }

    if (!testJumpMoveCheck(board)) {
        printf("TEST JUMP MOVE CHECK FAILED");
        return 0;
    }

    if (!testJumpMove(board)) {
        printf("TEST JUMP MOVE FAILED");
        return 0;
    }

    print_board(board);
    printf("\nALL TESTS PASSED");
    // free(board);
    return 0;
}