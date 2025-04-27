#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//function drawing horizontal line separating board :)
void drawLine(int n) {
    for (int i = 1; i <= n; i++) {
        printf("-");
    }
}

//function to randomize numbers in rows
void shuffle(int *array, int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

//checking if move/number placement is correct
int isValidMove(int** board, int size, int row, int col, int number) {

    //checking if number is the only one in this row and column
    for (int i = 0; i < size; i++) {
        if (board[i][col] == number || board[row][i] == number) {
            return 0;
        }
    }

    int blockSize = (int)sqrt(size);

    //checking if number is the only one in the neighborhood
    int startRow = (row / blockSize) * blockSize;
    int startCol = (col / blockSize) * blockSize;

    for (int i = startRow; i < startRow + blockSize; i++)
    {
        for (int j = startCol; j < startCol + blockSize; j++)
        {
            if (board[i][j] == number)
                return 0;
        }
    }

    return 1;
}

//function to fill board with numbers
int fillBoard(int** board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {

                int numbers[size];

                for (int k = 0; k < size; k++) {
                    numbers[k] = k+1;
                }

                shuffle(numbers, size);

                for (int k = 0; k < size; k++) {
                    if (isValidMove(board, size, i, j, numbers[k])) {
                        board[i][j] = numbers[k];
                        if (fillBoard(board, size) == 1)
                            return 1;
                        board[i][j] = 0;
                    }
                }

                return 0;
            }
        }
    }
    return 1;
}

//function to create clear board
void createBoard(int** board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = 0;
        }
    }
    fillBoard(board, size);
}

//function to display board in nice way
void printBoard(int** board, int size, int** originalBoard)
{
    int blockSize = (int)sqrt(size);

    for (int i = 0; i < size; i++) {
        printf("\n");
        if (i % blockSize == 0 && i != 0)
        {
            drawLine(size*5);
            printf("\n" );
        }
        for (int j = 0; j < size; j++)
        {
            if (j % blockSize == 0 && j != 0)
            {
                printf("|");
            }
            if (board[i][j] != 0 && board[i][j] == originalBoard[i][j]) {
                printf("\033[0;32m");
                printf("%3d  ", board[i][j]);
                printf("\033[0m");
            }
            else {
                printf("%3d  ", board[i][j]);
            }
        }
    }
}

//checking if game is over
int isBoardComplete(int** board, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0)
                return 0;
    return 1;
}

//function for copying board (to prevent making changes in original board)
void copyBoard(int** board, int** originalBoard, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            originalBoard[i][j] = board[i][j];
        }
    }
}

//removing given number of items from board
void removeNumbers(int** board, int size, int quantityToDelete) {
    int removed = 0;
    while (removed < quantityToDelete)
    {
        int row = rand() % size;
        int col = rand() % size;
        if (board[row][col] != 0)
        {
            board[row][col] = 0;
            removed++;
        }
    }
}

int main(void) {
    int mistakes = 0;
    int size;

    printf("Choose size of board:\n1) 4x4\n2) 9x9\n3) 16x16\n4) 25x25\n");
    int answer;
    scanf("%d", &answer);

    while (answer != 1 && answer != 2 && answer != 3 && answer != 4) {
        printf("Please enter correct value.\n");
        scanf("%d", &answer);
    }

    if (answer == 1) {
        size = 4;
    }
    else if (answer == 2) {
        size = 9;
    }
    else if (answer == 3) {
        size = 16;
    }
    else if (answer == 4) {
        size = 25;
    }


    int **board = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        board[i] = malloc(size * sizeof(int));
    }

    printf("Choose level: 1, 2 or 3:\n");
    int level;
    scanf("%d", &level);

    while (level != 1 && level != 2 && level != 3) {
        printf("Please choose correct option.\n");
        scanf("%d", &level);
    }

    createBoard(board, size);


    if (level == 1) {
        removeNumbers(board, size, (int)(size * size *  0.4));
    }
    else if (level == 2) {
        removeNumbers(board, size, (int)(size * size *  0.6));
    }
    else if (level == 3) {
        removeNumbers(board, size, (int)(size * size *  0.8));
    }

    int **originalBoard = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        originalBoard[i] = malloc(size * sizeof(int));
    }
    copyBoard(board, originalBoard, size);

    while (1) {
        printBoard(board, size, originalBoard);
        if (isBoardComplete(board, size) == 1) {
            printf("\nCongratulations! You won with %d mistakes :)", mistakes);
            break;
        }

        printf("\nPlease enter your move (row column value) or -1 to finish:\n");
        int row;
        int col;
        int value;

        if (scanf("%d", &row) != 1) {
            break;
        }
        if (row == -1) {
            break;
        }
        if (scanf("%d %d", &col, &value) != 2) {
            break;
        }

        row--;
        col--;

        if (row >= size  || row < 0 || col >= size || col < 0) {
            printf("Invalid input\n");
        }
        else if (originalBoard[row][col] != 0) {
            printf("You can't change the numbers that are already given.\n");
        }
        else if (isValidMove(board, size, row, col, value) == 0) {
            printf("Invalid move.\n");
            mistakes++;
        }
        else {
            board[row][col] = value;
        }
    }

    for (int i = 0; i < size; i++) {
        free(board[i]);
        free(originalBoard[i]);
    }
    free(board);
    free(originalBoard);
    return 0;
}