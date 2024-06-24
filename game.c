#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BOARD_SIZE 7

// Function to initialize the game board with empty squares
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' '; // Use ' ' to represent empty squares
        }
    }
}

// Function to randomly place pieces on the board
void placePiecesRandomly(char board[BOARD_SIZE][BOARD_SIZE], int numPiecesPerPlayer) {
    // Seed the random number generator
    srand(time(NULL));

    // Place Player 1's pieces
    for (int i = 0; i < numPiecesPerPlayer; i++) {
        int row, col;
        do {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
        } while (board[row][col] != ' '); // Check if the square is already occupied
        board[row][col] = 'X';
    }

    // Place Player 2's pieces
    for (int i = 0; i < numPiecesPerPlayer; i++) {
        int row, col;
        do {
            row = rand() % BOARD_SIZE;
            col = rand() % BOARD_SIZE;
        } while (board[row][col] != ' ');
        board[row][col] = 'O';
    }
}

// Function to print the current state of the board
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("   1 2 3 4 5 6 7\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c  ", 'a' + i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Function to check if a move is valid
int isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int fromRow, int fromCol, int toRow, int toCol) {
    if (fromRow < 0 || fromRow >= BOARD_SIZE || fromCol < 0 || fromCol >= BOARD_SIZE ||
        toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE) {
        return 0; // Coordinates are out of bounds
    }
    
    if (board[fromRow][fromCol] == ' ' || board[toRow][toCol] != ' ') {
        return 0; // Invalid piece or the target square is already occupied
    }

    int dx = abs(toRow - fromRow);
    int dy = abs(toCol - fromCol);
    
    if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
        return 1; // Valid move (horizontal or vertical)
    }

    return 0; // Invalid move
}

// Heuristic function that calculates a score for a given move (example)
int calculateHeuristic(char board[BOARD_SIZE][BOARD_SIZE], int fromRow, int fromCol, int toRow, int toCol) {
    int centerRow = BOARD_SIZE / 2;
    int centerCol = BOARD_SIZE / 2;
    int distanceToCenterBefore = abs(fromRow - centerRow) + abs(fromCol - centerCol);
    int distanceToCenterAfter = abs(toRow - centerRow) + abs(toCol - centerCol);

    // Return a higher score for moves that get closer to the center
    return distanceToCenterBefore - distanceToCenterAfter;
}

// Function to check if a player can make any valid move
int canPlayerMakeMove(char board[BOARD_SIZE][BOARD_SIZE], int player, int numPiecesPerPlayer) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == (player == 1 ? 'X' : 'O')) {
                // Check if this piece can move to any neighboring square
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int newRow = i + dx;
                        int newCol = j + dy;
                        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                            if (board[newRow][newCol] == ' ') {
                                return 1; // The player has at least one valid move
                            }
                        }
                    }
                }
            }
        }
    }
    return 0; // The player cannot make any valid move
}

int main() {
    char board[BOARD_SIZE][BOARD_SIZE];
    int numPiecesPerPlayer;
    char userChoice;
    int turnLimit;

    // Prompt the user to input the number of pieces per player and the turn limit
    printf("Enter the number of pieces for each player: ");
    scanf("%d", &numPiecesPerPlayer);
    printf("Enter the turn limit: ");
    scanf("%d", &turnLimit);

    // Initialize the game board and place pieces
    initializeBoard(board);
    placePiecesRandomly(board, numPiecesPerPlayer);

    // Prompt the user to choose their role as Player 1 or Player 2
    printf("Do you want to be Player 1 (X) or Player 2 (O)? (1/2): ");
    scanf(" %c", &userChoice);

    // Determine which player makes the first move
    int currentPlayer = (userChoice == '1') ? 1 : 2;

    int turn = 0;
    while (turn < turnLimit) {
        // Print the current state of the board
        printf("Turn %d:\n", turn + 1);
        printBoard(board);

        // Determine the current player
        char currentPlayerIndicator = (currentPlayer == 1) ? 'X' : 'O';

        if (currentPlayer == 1) {
            // Player 1's turn
            printf("Player 1 (X), enter your move (e.g., c4 d4): ");
            int fromRow, fromCol, toRow, toCol;
            scanf(" %d %d %d %d", &fromRow, &fromCol, &toRow, &toCol);

            if (isValidMove(board, fromRow, fromCol, toRow, toCol)) {
                // Apply the valid move to the board
                if (board[fromRow][fromCol] == 'X') {
                    board[toRow][toCol] = 'X';
                    board[fromRow][fromCol] = ' ';
                } else {
                    printf("Invalid piece. It must belong to Player 1.\n");
                }

                // Switch to the next player
                currentPlayer = 2;
                turn++;
            } else {
                printf("Invalid move. Try again.\n");
            }
        } else {
            // Player 2's turn or computer's turn (AI) - Implement your logic here
            int bestFromRow, bestFromCol, bestToRow, bestToCol;
            int bestScore = -1;

            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == 'O') {
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                int newRow = i + dx;
                                int newCol = j + dy;
                                if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                                    if (board[newRow][newCol] == ' ') {
                                        int score = calculateHeuristic(board, i, j, newRow, newCol);
                                        if (score > bestScore) {
                                            bestScore = score;
                                            bestFromRow = i;
                                            bestFromCol = j;
                                            bestToRow = newRow;
                                            bestToCol = newCol;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Apply the best move found by the AI
            board[bestToRow][bestToCol] = 'O';
            board[bestFromRow][bestFromCol] = ' ';

            // Switch to the next player
            currentPlayer = 1;
            turn++;
        }
    }

    // Check if the game ended due to the turn limit
    if (turn == turnLimit) {
        // Determine the winner based on the number of valid moveable spaces
        int player1MoveableSpaceCount = 0;
        int player2MoveableSpaceCount = 0;

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == 'X') {
                    player1MoveableSpaceCount++;
                } else if (board[i][j] == 'O') {
                    player2MoveableSpaceCount++;
                }
            }
        }

        if (player1MoveableSpaceCount > player2MoveableSpaceCount) {
            printf("Player 1 (X) wins!\n");
        } else if (player2MoveableSpaceCount > player1MoveableSpaceCount) {
            printf("Player 2 (O) wins!\n");
        } else {
            printf("It's a draw!\n");
        }
    }

    return 0;
}

