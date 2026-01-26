#include "board.c"

int main() {
    char *board = create_board();

    char move[5];
    move[0] = 'O';
    print_board(board);

    while (move[0] != 'E') {
        scanf("%s", move);
        if (isValidMove(board, move)) {
            movePiece(board, move);
            print_board(board);
        }
        else {
            printf("Invalid Move: %s\n", move);
        }
    }
    printf("\nFinished");

    // endGame(board);

    return 0;
}