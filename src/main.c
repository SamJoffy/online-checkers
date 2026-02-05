#include "board.c"

int main() {
    char *board = create_board();

    char move[5];
    move[0] = 'O';
    int allowedChains;
    print_board(board);
    int x, y;

    while (move[0] != 'E') {
        scanf("%s", move);
        if (isValidMove(board, move)) {
            allowedChains = movePiece(board, move);

            printf("AllowedChains: %d\n", allowedChains);

            x = move[2] - '0';
            y = move[1] - '0';
            x = getMoveX(x, move[3]);
            y = getMoveY(y, move[3]);

            while(move[0] != 'N' && allowedChains) {
                print_board(board);
                printf("Can chain move\n");
                scanf("%s", move);
                if (move[2] == x + '0' && move[1] == y + '0' && isValidChain(allowedChains, move[3])) {
                    allowedChains = movePiece(board, move);
                    x = getMoveX(x, move[3]);
                    y = getMoveY(y, move[3]);
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