#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

const int size = 9;
int board[9][9];
int originalBoard[9][9];

//function drawing horizontal line separating board :)
void drawLine(int n) {
    for (int i = 1; i <= n; i++) {
        printf("-");
    }
}

//function to randomize numbers in rows
void shuffle(int *array)
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
int isValidMove(int row, int col, int number) {

    //checking if number is the only one in this row and column
    for (int i = 0; i < size; i++) {
        if (board[i][col] == number || board[row][i] == number) {
            return 0;
        }
    }

    //checking if number is the only one in the neighborhood
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;

    for (int i = startRow; i < startRow + 3; i++)
    {
        for (int j = startCol; j < startCol + 3; j++)
        {
            if (board[i][j] == number)
                return 0;
        }
    }

    return 1;
}

//function to fill board with numbers
int fillBoard() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {

                int numbers[size];

                for (int k = 0; k < size; k++) {
                    numbers[k] = k+1;
                }

                shuffle(numbers);

                for (int k = 0; k < size; k++) {
                    if (isValidMove(i, j, numbers[i])) {
                        board[i][j] = numbers[i];
                        if (fillBoard())
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
void createBoard() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = 0;
        }
    }
    fillBoard();
}

//function to display board in nice way
void printBoard()
{
    //printf("Sudoku board");
    for (int i = 0; i < size; i++) {
        printf("\n");
        if (i % (int)sqrt(size) == 0 && i != 0)
        {
            drawLine(size*5);
            printf("\n" );
        }
        for (int j = 0; j < size; j++)
        {
            if (j % (int)sqrt(size) == 0 && j != 0)
            {
                printf("|");
            }
            printf("  %d  ", board[i][j]);
        }
    }
}

//checking if game is over
int isBoardComplete() {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0)
                return 0;
    return 1;
}

//function for copying board (to prevent making changes in original board)
void copyBoard() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            originalBoard[i][j] = board[i][j];
        }
    }
}

//removing given number of items from board
void removeNumbers(int quantityToDelete) {
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
    createBoard();
    removeNumbers(50);
    copyBoard();

    while (1) {
        //system("cls");
        printBoard();
        if (isBoardComplete()) {
            printf("Congratulations! You won :)");
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

        if (row > 9 || row < 1 || col > 9 || col < 1) {
            printf("Invalid input\n");
        }
        else if (originalBoard[row][col] != 0) {
            printf("You can't change the numbers that are already given.\n");
        }
        else if (!isValidMove(row, col, value)) {
            printf("Invalid move.\n");
        }
        else {
            board[row][col] = value;
        }
    }
    return 0;
}