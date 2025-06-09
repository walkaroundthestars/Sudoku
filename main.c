#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//sudoku state with board and number of conflicts
typedef struct {
    int **board;
    int conflicts;
} SudokuState;

//function drawing horizontal line separating board :)
void drawLine(int n) {
    for (int i = 1; i <= n; i++) {
        printf("-");
    }
}

//function to randomize numbers in rows
void shuffle(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

//function for choosing board size
int chooseSize() {
    int size = 9;

    printf("Choose size of board:\n1) 4x4\n2) 9x9\n3) 16x16\n");
    int answer;
    scanf("%d", &answer);

    while (answer != 1 && answer != 2 && answer != 3) {
        printf("Please enter correct value.\n");
        scanf("%d", &answer);
    }

    if (answer == 1) {
        size = 4;
    } else if (answer == 2) {
        size = 9;
    } else if (answer == 3) {
        size = 16;
    }
    return size;
}

//checking if move/number placement is correct
int isValidMove(int **board, int size, int row, int col, int number) {
    //checking if number is the only one in this row and column
    for (int i = 0; i < size; i++) {
        if (board[i][col] == number || board[row][i] == number) {
            return 0;
        }
    }

    int blockSize = (int) sqrt(size);

    //checking if number is the only one in the neighborhood
    int startRow = (row / blockSize) * blockSize;
    int startCol = (col / blockSize) * blockSize;

    for (int i = startRow; i < startRow + blockSize; i++) {
        for (int j = startCol; j < startCol + blockSize; j++) {
            if (board[i][j] == number)
                return 0;
        }
    }

    return 1;
}

//function for counting number of conflicts
int countConflicts(int **board, int size) {
    int conflicts = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int value = board[i][j];
            if (value != 0) {
                // setting to zero to avoid conflicts with itself
                board[i][j] = 0;
                if (!isValidMove(board, size, i, j, value)) {
                    conflicts++;
                }
                board[i][j] = value;
            }
        }
    }

    return conflicts;
}

//function to fill board with numbers
int fillBoard(int **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                int numbers[size];

                for (int k = 0; k < size; k++) {
                    numbers[k] = k + 1;
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

//function to fill board with random numbers (function for solver)
int fillBoardRandom(int **board, int **originalBoard, int size) {
    int blockSize = (int) sqrt(size);
    int numbers[size];

    for (int row = 0; row < blockSize; row++) {
        for (int column = 0; column < blockSize; column++) {

            for (int i = 0; i < size; i++) {
                numbers[i] = i + 1;
            }
            shuffle(numbers, size);

            //todo: mozna zrobić funkcje do sprawdzania wolnych komórek bo sie chyba powtarza
            //checking which cells are not 0
            int used[size + 1];
            for (int i = 0; i <= size; i++) {
                used[i] = 0;
            }

            for (int i = 0; i < blockSize; i++) {
                for (int j = 0; j < blockSize; j++) {
                    int r = row * blockSize + i;
                    int c = column * blockSize + j;
                    int value = originalBoard[r][c];

                    if (value != 0) {
                        used[value] = 1;
                    }
                }
            }

            //fill empty cells
            int numIndex = 0;
            for (int i = 0; i < blockSize; i++) {
                for (int j = 0; j < blockSize; j++) {
                    int r = row * blockSize + i;
                    int c = column * blockSize + j;

                    if (originalBoard[r][c] == 0) {
                        while (numIndex < size && used[numbers[numIndex]]) {
                            numIndex++;
                        }
                        if (numIndex < size) {
                            board[r][c] = numbers[numIndex];
                            used[numbers[numIndex]] = 1;
                            numIndex++;
                        }
                    } else {
                        board[r][c] = originalBoard[r][c];
                    }
                }
            }
        }
    }
}

//function to create clear board
void createBoard(int **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = 0;
        }
    }
    fillBoard(board, size);
}

//function to display board in nice way
void printBoard(int **board, int size, int **originalBoard) {
    int blockSize = (int) sqrt(size);

    for (int i = 0; i < size; i++) {
        printf("\n");
        if (i % blockSize == 0 && i != 0) {
            //drawing horizontal line
            drawLine(size * 5);
            printf("\n");
        }
        for (int j = 0; j < size; j++) {
            if (j % blockSize == 0 && j != 0) {
                //drawing vertical line
                printf("|");
            }
            if (board[i][j] != 0 && board[i][j] == originalBoard[i][j]) {
                //showing already given numbers in green
                printf("\033[0;32m");
                printf("%3d  ", board[i][j]);
                printf("\033[0m");
            } else {
                //showing other numbers and zeros in white
                printf("%3d  ", board[i][j]);
            }
        }
    }
}

//function for saving current game
void saveToFile(int **board, int size, int **originalBoard, int mistakes) {
    char fileName[50];
    sprintf(fileName, "sudoku%dx%d.txt", size, size);
    FILE *file = fopen(fileName, "w");

    if (file == NULL) {
        printf("Couldn't save the game.\n");
        return;
    }

    //saving number of mistakes
    fprintf(file, "%d\n", mistakes);

    //saving current game state
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%d ", board[i][j]);
        }
        fprintf(file, "\n");
    }

    //saving original board
    fprintf(file, "\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%d ", originalBoard[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game successfully saved.\n");
}

//function for loading saved games
int readFromFile(int **board, int size, int **originalBoard, int mistakes) {
    char fileName[50];
    sprintf(fileName, "sudoku%dx%d.txt", size, size);
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        printf("Couldn't find a game in this size.\n");
        return 0;
    }

    //loading saved number of mistakes
    fscanf(file, "%d\n", &mistakes);

    //loading saved game state
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(file, "%d", &board[i][j]);
        }
    }

    //loading original board pattern
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(file, "%d", &originalBoard[i][j]);
        }
    }

    fclose(file);
    return 1;
}

//checking if game is over
int isBoardComplete(int **board, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0)
                return 0;
    return 1;
}

//function for copying board (to prevent from making changes in original board)
void copyBoard(int **board, int **originalBoard, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            originalBoard[i][j] = board[i][j];
        }
    }
}

//removing given number of items from board
void removeNumbers(int **board, int size, int quantityToDelete) {
    int removed = 0;
    while (removed < quantityToDelete) {
        int row = rand() % size;
        int col = rand() % size;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            removed++;
        }
    }
}

//loop of the game
void game(int **board, int size, int **originalBoard, int mistakes) {
    while (1) {
        printBoard(board, size, originalBoard);

        if (isBoardComplete(board, size) == 1) {
            printf("\nCongratulations! You won with %d mistakes :)\n", mistakes);
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
            while (getchar() != '\n');
            printf("Do you want to save the game? y/n\n");
            char ans;
            ans = getchar();
            if (ans == 'n') {
                break;
            }
            if (ans == 'y') {
                saveToFile(board, size, originalBoard, mistakes);
                break;
            }
        }
        if (scanf("%d %d", &col, &value) != 2) {
            break;
        }

        row--;
        col--;

        if (row >= size || row < 0 || col >= size || col < 0) {
            printf("Invalid input\n");
        } else if (originalBoard[row][col] != 0) {
            printf("You can't change the numbers that are already given.\n");
        } else if (isValidMove(board, size, row, col, value) == 0) {
            printf("\033[0;31m");
            printf("Invalid move.\n");
            printf("\033[0m");
            mistakes++;
        } else {
            board[row][col] = value;
        }
    }
}

//creating new game
void newGame() {
    int mistakes = 0;
    int size = chooseSize();

    int **board = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        board[i] = malloc(size * sizeof(int));
    }

    //choose of difficulty level
    printf("Choose level: \n1) Easy\n2) Medium\n3) Hard\n4) Super hard\n");
    int level;
    scanf("%d", &level);

    while (level != 1 && level != 2 && level != 3 && level != 4) {
        printf("Please choose correct option.\n");
        scanf("%d", &level);
    }

    createBoard(board, size);


    if (level == 1) {
        removeNumbers(board, size, (int) (size * size * 0.4));
    } else if (level == 2) {
        removeNumbers(board, size, (int) (size * size * 0.6));
    } else if (level == 3) {
        removeNumbers(board, size, (int) (size * size * 0.8));
    } else if (level == 4) {
        removeNumbers(board, size, (int) (size * size * 0.95));
    }

    int **originalBoard = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        originalBoard[i] = malloc(size * sizeof(int));
    }
    copyBoard(board, originalBoard, size);

    game(board, size, originalBoard, mistakes);

    for (int i = 0; i < size; i++) {
        free(board[i]);
        free(originalBoard[i]);
    }
    free(board);
    free(originalBoard);
}

//loading and preparing old game
void oldGame() {
    int size = chooseSize();

    int **board = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        board[i] = malloc(size * sizeof(int));
    }

    int **originalBoard = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        originalBoard[i] = malloc(size * sizeof(int));
    }

    //load from file
    int mistakes = 0;
    int isSaved = readFromFile(board, size, originalBoard, mistakes);

    if (isSaved == 1) {
        game(board, size, originalBoard, mistakes);
    }

    for (int i = 0; i < size; i++) {
        free(board[i]);
        free(originalBoard[i]);
    }
    free(board);
    free(originalBoard);
}

void generateNeighbor(SudokuState *neighborState, SudokuState *currentState, int **originalBoard, int size) {
    int blockSize = (int)sqrt(size);

    //copying current state to neighbor
    for (int i = 0; i < size; i++) {
        memcpy(neighborState->board[i], currentState->board[i], size * sizeof(int));
    }

    //choosing random cell for replacement
    int blockRow = rand() % blockSize;
    int blockCol = rand() % blockSize;

    //find cells which aren't blocked
    int freeCells[size*size][2];
    int count = 0;
    for (int i = 0; i < blockSize; i++) {
        for (int j = 0; j < blockSize; j++) {
            int row = blockRow * blockSize + i;
            int col = blockCol * blockSize + j;
            if (!originalBoard[row][col]) {
                freeCells[count][0] = row;
                freeCells[count][1] = col;
                count++;
            }
        }
    }

    if (count >= 2) {
        //choosing 2 random cells to exchange
        int a = rand() % count;
        int b;
        do {
            b = rand() % count;
        } while (b == a);

        int row1 = freeCells[a][0];
        int col1 = freeCells[a][1];
        int row2 = freeCells[b][0];
        int col2 = freeCells[b][1];

        //exchange their values
        int temp = neighborState->board[row1][col1];
        neighborState->board[row1][col1] = neighborState->board[row2][col2];
        neighborState->board[row2][col2] = temp;
    }

    //check how many conflicts does the neighbor have
    neighborState->conflicts = countConflicts(neighborState->board, size);
}

//simulated annealing solver
void saSolver() {
    int size = 9;// chooseSize();

    int **board = malloc(size * sizeof(int *));
    int **originalBoard = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        board[i] = malloc(size * sizeof(int));
        originalBoard[i] = malloc(size * sizeof(int));
    }

    createBoard(board, size);

    //trying on level 2
    removeNumbers(board, size, (int) (size * size * 0.6));

    //copying original board
    copyBoard(board, originalBoard, size);

    //filling board with random values
    fillBoardRandom(board, originalBoard, size);

    srand(time(NULL));

    int **current = malloc(size * sizeof(int *));
    int **neighbor = malloc(size * sizeof(int *));
    int **best = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        current[i] = malloc(size * sizeof(int));
        neighbor[i] = malloc(size * sizeof(int));
        best[i] = malloc(size * sizeof(int));
        memcpy(current[i], board[i], size * sizeof(int));
        memcpy(best[i], board[i], size * sizeof(int));
    }

    SudokuState currentState, neighborState;
    currentState.board = current;
    currentState.conflicts = countConflicts(current, size);

    neighborState.board = neighbor;
    neighborState.conflicts = currentState.conflicts;

    int bestConflicts = countConflicts(best, size);
    int currentConflicts = bestConflicts;

    double T = 5.0;
    double endT = 0.01;
    double alpha = 0.995;
    int maxIterations = 100000;

    for (int iter = 0; iter < maxIterations && T > endT; iter++) {
        generateNeighbor(&neighborState, &currentState, originalBoard, size);
        int neighborConflicts = neighborState.conflicts;
        int delta = neighborConflicts - currentConflicts;

        if (delta < 0 || (rand() / (double)RAND_MAX) < exp(-delta / T)) {
            for (int i = 0; i < size; i++) {
                memcpy(current[i], neighbor[i], size * sizeof(int));
            }
            currentConflicts = neighborConflicts;

            if (currentConflicts < bestConflicts) {
                for (int i = 0; i < size; i++) {
                    memcpy(best[i], current[i], size * sizeof(int));
                }
                bestConflicts = currentConflicts;
            }
        }

        if (iter % 1000 == 0 || bestConflicts == 0) {
            printf("Iter: %d, T: %.3f, Conflicts: %d\n", iter, T, bestConflicts);
        }

        if (bestConflicts == 0) break;
        T *= alpha;
    }

    printf("\nFinal solution (conflicts = %d):\n", bestConflicts);
    printBoard(best, size, originalBoard);

    for (int i = 0; i < size; i++) {
        free(board[i]);
        free(originalBoard[i]);
        free(current[i]);
        free(neighbor[i]);
        free(best[i]);
    }
    free(board);
    free(originalBoard);
    free(current);
    free(neighbor);
    free(best);
}

//creating one single genotype
void createGenotype(SudokuState *ind, int **originalBoard, int size) {
    ind->board = malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) {
        ind->board[i] = malloc(size * sizeof(int));
    }

    fillBoardRandom(ind->board, originalBoard, size);
    ind->conflicts = countConflicts(ind->board, size);
}

//crossing genotypes by rows
void rowCrossover(SudokuState *child, SudokuState *mother, SudokuState *father, int size, int **originalBoard) {
    // for (int i = 0; i < size; i++) {
    //     int **childBoard = child->board;
    //     if (rand() % 2 == 0) {
    //         memcpy(childBoard[i], mother->board[i], size * sizeof(int));
    //     } else {
    //         memcpy(childBoard[i], father->board[i], size * sizeof(int));
    //     }
    // }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (originalBoard[i][j] != 0) {
                child->board[i][j] = originalBoard[i][j]; // nie zmieniamy danych oryginalnych
            } else {
                if (rand() % 2 == 0) {
                    child->board[i][j] = mother->board[i][j];
                } else {
                    child->board[i][j] = father->board[i][j];
                }
            }
        }
    }
}

//crossing genotypes by blocks
void blockCrossover(SudokuState *child, SudokuState *mother, SudokuState *father, int size, int **originalBoard) {
    int blockSize = (int)sqrt(size);

    for (int i = 0; i < blockSize; i++) {
        for (int j = 0; j < blockSize; j++) {
            int useMother = rand() % 2;

            for (int k = 0; k < blockSize; k++) {
                for (int l = 0; l < blockSize; l++) {
                    int row = i * blockSize + k;
                    int col = j * blockSize + l;

                    //child->board[row][col] = useMother ? mother->board[row][col] : father->board[row][col];
                    if (originalBoard[row][col] != 0) {
                        child->board[row][col] = originalBoard[row][col];
                    } else {
                        if (useMother)
                            child->board[row][col] = mother->board[row][col];
                        else
                            child->board[row][col] = father->board[row][col];
                    }
                }
            }
        }
    }
}

void mutate(SudokuState *ind, int **originalBoard, int size) {
    int blockSize = (int)sqrt(size);
    int blockRow = rand() % blockSize;
    int blockCol = rand() % blockSize;

    int freeCells[size][2];
    int count = 0;

    for (int i = 0; i < blockSize; i++) {
        for (int j = 0; j < blockSize; j++) {
            int row = blockRow * blockSize + i;
            int column = blockCol * blockSize + j;
            if (originalBoard[row][column] == 0) {
                freeCells[count][0] = row;
                freeCells[count][1] = column;
                count++;
            }
        }
    }

    if (count >= 2) {
        int a = rand() % count;
        int b;
        do {
            b = rand() % count;
        } while (a == b);

        int row1 = freeCells[a][0];
        int column1 = freeCells[a][1];
        int row2 = freeCells[b][0];
        int column2 = freeCells[b][1];

        int temp = ind->board[row1][column1];
        ind->board[row1][column1] = ind->board[row2][column2];
        ind->board[row2][column2] = temp;
    }

    ind->conflicts = countConflicts(ind->board, size);
}

//selection of child genotypes
SudokuState *selection(SudokuState **pop, int popSize, int tournamentSize) {
    SudokuState *best = pop[rand() % popSize];
    for (int i = 1; i < tournamentSize; i++) {
        SudokuState *competitor = pop[rand() % popSize];
        if (competitor->conflicts < best->conflicts)
            best = competitor;
    }
    return best;
}

//function for copying the best state of sudoku
SudokuState *copyState(SudokuState *bestState, int size) {
    SudokuState *copy = malloc(sizeof(SudokuState));
    copy->board = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        copy->board[i] = malloc(size * sizeof(int));
        memcpy(copy->board[i], bestState->board[i], size * sizeof(int));
    }
    copy->conflicts = bestState->conflicts;
    return copy;
}

//genetic algorythm solver
void gaSolver() {
    int size = 9; //chooseSize();

    int **board = malloc(size * sizeof(int *));
    int **originalBoard = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        board[i] = malloc(size * sizeof(int));
        originalBoard[i] = malloc(size * sizeof(int));
    }

    createBoard(board, size);

    //trying on level 2
    removeNumbers(board, size, (int) (size * size * 0.6));

    //copying original board
    copyBoard(board, originalBoard, size);

    int populationSize = 200;
    int generations = 1000;
    double mutationRate = 0.1;
    int tournamentSize = 5;

    SudokuState **population = malloc(populationSize * sizeof(SudokuState *));
    for (int i = 0; i < populationSize; i++) {
        population[i] = malloc(sizeof(SudokuState));
        population[i]->board = malloc(size * sizeof(int *));
        for (int j = 0; j < size; j++) {
            population[i]->board[j] = malloc(size * sizeof(int));
        }
        createGenotype(population[i], originalBoard, size);
    }

    SudokuState * best = population[0];

    for (int generation = 0; generation < generations; generation++) {
        SudokuState **newPopulation = malloc(populationSize * sizeof(SudokuState *));
        for (int i = 0; i < populationSize; i++) {
            newPopulation[i] = malloc(sizeof(SudokuState));
            newPopulation[i]->board = malloc(size * sizeof(int *));
            for (int j = 0; j < size; j++) {
                newPopulation[i]->board[j] = malloc(size * sizeof(int));
            }

            SudokuState *mother = selection(population, populationSize, tournamentSize);
            SudokuState *father = selection(population, populationSize, tournamentSize);

            //crossover
            blockCrossover(newPopulation[i], mother, father, size, originalBoard);
            //rowCrossover(newPopulation[i], mother, father, size, originalBoard);

            // Mutation
            if ((rand() / (double)RAND_MAX) < mutationRate) {
                mutate(newPopulation[i], originalBoard, size);
            }

            newPopulation[i]->conflicts = countConflicts(newPopulation[i]->board, size);

            //if current genotype is better, set it as new best
            if (newPopulation[i]->conflicts < best->conflicts) {
                best = newPopulation[i];
            }
        }

        //deleting old population
        for (int i = 0; i < populationSize; i++) {
            for (int j = 0; j < size; j++) {
                free(population[i]->board[j]);
            }
            free(population[i]->board);
            free(population[i]);
        }
        free(population);

        population = newPopulation;

        if (generation % 10 == 0 || best->conflicts == 0) {
            printf("Generation: %d, number of conflicts: %d\n", generation, best->conflicts);
        }

        if (best->conflicts == 0) break;
    }

    printf("\nBest solution:\n");
    printBoard(best->board, size, originalBoard);
    printf("Conflicts: %d\n", best->conflicts);


    for (int i = 0; i < populationSize; i++) {
        for (int j = 0; j < size; j++) {
            free(population[i]->board[j]);
        }
        free(population[i]->board);
        free(population[i]);
    }
    free(population);
}

//printing instruction how to play
void printInstruction() {
    printf("\n*** How to Play Sudoku ***\n");
    printf("1. Choose the board size and difficulty level.\n");
    printf("2. Fill every empty cell so that:\n");
    printf("   - Each number appears once in every row,\n");
    printf("   - Each number appears once in every column,\n");
    printf("   - Each number appears once in every box.\n");
    printf("3. To make a move, enter three numbers:\n");
    printf("   (row) (column) (value)\n");
    printf("   Example: 2 3 5 (puts 5 in row 2, column 3)\n");
    printf("4. You cannot change the given numbers.\n");
    printf("5. Wrong moves will be counted as mistakes.\n");
    printf("6. Enter -1 as the row number to quit the game.\n\n");
    printf("Press Enter to come back to menu.\n");
    getchar();
}

//main menu
int main(void) {
    int menu;
    do {
        printf("Choose option from menu:\n1) New game\n2) Old game\n3) How to play\n4) Solve with SA\n5) Solve with GA\n6) Exit\n");
        scanf("%d", &menu);

        while (menu != 1 && menu != 2 && menu != 3 && menu != 4 && menu != 5 && menu != 6) {
            printf("Please enter correct value.\n");
            scanf("%d", &menu);
        }

        if (menu == 1) {
            newGame();
        } else if (menu == 2) {
            oldGame();
        } else if (menu == 3) {
            printInstruction();
        } else if (menu == 4) {
            saSolver();
        } else if (menu == 5) {
            gaSolver();
        } else if (menu == 6) {
            printf("Thank you for playing!");
        }

        while (getchar() != '\n');
    } while (menu != 6);
    return 0;
}
