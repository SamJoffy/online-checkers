#include <stdio.h>
#include <stdlib.h>

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 10;
int *piece_count; // First is red, second is black

/** Ends the game and frees memory */
void endGame(char *board) {
    free(board);
    free(piece_count);
}

/** Checks whether the two pieces are the same color */
int isSameColor(char c, char d) {
    return c == d || ((c == 'B' || d == 'B') && (c == 'K' || d == 'K')) || ((c == 'R' || d == 'R') && (c == 'Q' || d == 'Q'));
}

/** Returns the count of the given piece */
int getPieceCount(char c) {
    return piece_count[c == 'B'];
}

/** Returns the character at position x, y */
char getCharAt(char *board, int x, int y) {
    return board[y * BOARD_WIDTH + x];
}

/** Sets the character at the given board coordinates to c */
void setCharAt(char *board, int x, int y, char c) {
    board[y * BOARD_WIDTH + x] = c;
}

/** Prints the current state of the board in plaintext */
void print_board(char *board) {
    printf("\n");
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            printf(" %c ", getCharAt(board, j, i));
        }
        printf("\n");
    }
    printf("\n");
}

/** Returns a new instance of a board */
char *create_board() {
    piece_count = malloc(sizeof(int) * 2);
    piece_count[0] = 20;
    piece_count[1] = 20;

    char *board = malloc(sizeof(char) * BOARD_WIDTH * BOARD_HEIGHT);

    for (int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i ++) {
        board[i] = ' ';
    }

    for (int i = 0; i < BOARD_WIDTH * 4; i += 2) {
        if (i == BOARD_WIDTH || i == BOARD_WIDTH * 3) {
            i += BOARD_WIDTH;
        }
        board[i] = 'B';
    }

    for (int i = BOARD_WIDTH + 1; i < BOARD_WIDTH * 4; i += 2) {
        if (i == BOARD_WIDTH * 2 + 1) {
            i += BOARD_WIDTH;
        }
        board[i] = 'B';
    }

    for (int i = BOARD_WIDTH * 4; i < BOARD_WIDTH * 5; i += 2) {
        board[i] = 'O';
    }

    for (int i = BOARD_WIDTH * 5 + 1; i < BOARD_WIDTH * 6; i += 2) {
        board[i] = 'O';
    }

    for (int i = BOARD_WIDTH * 6; i < BOARD_WIDTH * BOARD_HEIGHT; i += 2) {
        if (i == BOARD_WIDTH * 7 || i == BOARD_WIDTH * 9) {
            i += BOARD_WIDTH;
        }
        board[i] = 'R';
    }

    for (int i = BOARD_WIDTH * 7 + 1; i < BOARD_WIDTH * BOARD_HEIGHT; i += 2) {
        if (i == BOARD_WIDTH * 8 + 1) {
            i += BOARD_WIDTH;
        }
        board[i] = 'R';
    }

    return board;
}

/** Checks if the given jump is valid. Assumes move has already been multiplied by two. */
int isJumpMoveValid(char *board, int x, int y, int xMove, int yMove, char color) {
    if (x + xMove < 0 || x + xMove >= BOARD_WIDTH || y + yMove < 0 || y + yMove > BOARD_HEIGHT) {
        return 0;
    }

    if (getCharAt(board, x + xMove, y + yMove) != 'O' || isSameColor(getCharAt(board, x + xMove/2, y + yMove/2), color) || getCharAt(board, x + xMove/2, y + yMove/2) == 'O') {
        return 0;
    }

    return 1;
}

/** Checks if the current move is valid */
int isValidMove(char *board, char *move) {
    /* Moves follow the format "color row_number column_number move_direction(s) (if jumping multiple pieces)" 
       and are always four characters long */
    
    char color = move[0];
    int x = move[2] - '0';
    int y = move[1] - '0';
    char direction = move[3];

    // Check if x and y are valid positions
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return 0;
    }

    // Check if the color given matches the coordinates
    if (!isSameColor(getCharAt(board, x, y), color)) {
        return 0;
    }

    // Make sure piece cannot move backwards (unless a queen)
    if (getCharAt(board, x, y) == 'B' && (direction == 'W' || direction == 'D')) {
        return 0;
    }
    if (getCharAt(board, x, y) == 'R' && (direction == 'S' || direction == 'A')) {
        return 0;
    }

    // Check if the direction is valid (use W for up left, D for up right, S for down right, A for down left)
    int xMove, yMove;
    switch (direction) {
    case 'W':
        xMove = -1;
        yMove = -1;
        break;
    
    case 'D':
        xMove = 1;
        yMove = -1;
        break;

    case 'S':
        xMove = 1;
        yMove = 1;
        break;

    case 'A':
        xMove = -1;
        yMove = 1;
        break;
    
    default:
        return 0;
    }

    if (x + xMove < 0 || y + yMove < 0 || x + xMove >= BOARD_WIDTH || y + yMove >= BOARD_HEIGHT) {
        return 0;
    }

    switch (getCharAt(board, x + xMove, y + yMove))
    {
    case 'R':
        if (isSameColor(color, 'R') || !isJumpMoveValid(board, x, y, xMove * 2, yMove * 2, color)) {
            return 0;
        }
        break;

    case 'B':
        if (isSameColor(color, 'B') || !isJumpMoveValid(board, x, y, xMove * 2, yMove * 2, color)) {
            return 0;
        }
        break;
    
    case 'Q':
        if (isSameColor(color, 'R') || !isJumpMoveValid(board, x, y, xMove * 2, yMove * 2, color)) {
            return 0;
        }
        break;

    case 'K':
        if (isSameColor(color, 'B') || !isJumpMoveValid(board, x, y, xMove * 2, yMove * 2, color)) {
            return 0;
        }
        break;

    default:
        break;
    }

    return 1;
}

/** Performs the given move on the board. Assumes the move is valid. Returns an integer 
 *  with a binary representation corresponding to which moves it can chain into.
 *  In the format SADW
*/
int movePiece(char *board, char *move) {
    int x = move[2] - '0';
    int y = move[1] - '0';
    char color = getCharAt(board, x, y);
    char direction = move[3];

    int xMove, yMove;
    switch (direction) {
    case 'W':
        xMove = -1;
        yMove = -1;
        break;
    
    case 'D':
        xMove = 1;
        yMove = -1;
        break;

    case 'S':
        xMove = 1;
        yMove = 1;
        break;

    case 'A':
        xMove = -1;
        yMove = 1;
        break;
    
    default:
        return 0;
    }

    int jumped = 0;

    if (getCharAt(board, x + xMove, y + yMove) == 'O') {
        setCharAt(board, x, y, 'O');
        setCharAt(board, x + xMove, y + yMove, color);
    }
    else {
        setCharAt(board, x, y, 'O');
        setCharAt(board, x + xMove, y + yMove, 'O');
        xMove *= 2;
        yMove *= 2;
        setCharAt(board, x + xMove, y + yMove, color);
        piece_count[isSameColor(color, 'R')]--;
        jumped = 1;
    }

    if (y + yMove == BOARD_HEIGHT - 1 && color == 'B') {
        setCharAt(board, x + xMove, y + yMove, 'K');
    }
    else if (y + yMove == 0 && color == 'R') {
        setCharAt(board, x + xMove, y + yMove, 'Q');
    }

    int chain = 0;

    if (jumped) {
        printf("Board while looking for chains:\n");
        print_board(board);

        int moves[] = {-2, 2};
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (isJumpMoveValid(board, x + xMove, y + yMove, moves[i], moves[j], color)) {
                    chain += 1 << (i + 2*j);
                }
            }
        }
    }

    return chain;
}

// todo: make sure is right piece
int isValidChain(int allowed, char move) {
    switch (move)
    {
    case 'S':
        return 8 & allowed;
        break;
    
    case 'A':
        return 4 & allowed;
        break;
    
    case 'D':
        return 2 & allowed;
        break;
    
    case 'W':
        return 1 & allowed;
        break;

    default:
        return 0;
        break;
    }
}

int getMoveX(int currentX, char move) {
    switch (move)
    {
    case 'S':
        currentX += 2;
        break;
    
    case 'A':
        currentX -= 2;
        break;
    
    case 'D':
        currentX += 2;
        break;
    
    case 'W':
        currentX -= 2;
        break;

    default:
        break;
    }

    return currentX;
}

int getMoveY(int currentY, char move) {
    switch (move)
    {
    case 'S':
        currentY += 2;
        break;
    
    case 'A':
        currentY += 2;
        break;
    
    case 'D':
        currentY -= 2;
        break;
    
    case 'W':
        currentY -= 2;
        break;

    default:
        break;
    }

    return currentY;
}