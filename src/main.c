#include "board.c"

int main() {
    char *board = create_board();

    char move[5];
    move[0] = 'O';
    int allowedChains;
    print_board(board);

    while (move[0] != 'E') {
        scanf("%s", move);
        if (isValidMove(board, move)) {
            allowedChains = movePiece(board, move);

            printf("AllowedChains: %d\n", allowedChains);

            while(move[0] != 'N' && allowedChains) {
                print_board(board);
                printf("Can chain move\n");
                scanf("%s", move);
                if (isValidChain(allowedChains, move[3])) {
                    allowedChains = movePiece(board, move);
                    printf("AllowedChains: %d\n", allowedChains);
                }
                else {
                    printf("Invalid Move: %s\n", move);
                }
            }
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