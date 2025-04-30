# Sudoku📱

* Dear Gentle Reader, *

This project was created as assignment at the university. 
It enables to play Sudoku (if you don't know what it is, you can find instruction in game menu :)) in 3 board sizes at different difficulty levels.

## How does it work?🚀

First, user has to choose one option from the menu (as in: start new game, play old one or read instruction).
- if the choice is new game, the user is asked to choose size of board (here 4x4, 9x9 or 16x16) and difficulty level, then the program creates filled board and removes amount of numbers based on difficulty level (the higher level, the more numbers are removed). It also creates copy of the board to prevent changes in numbers given by the program.
![image](https://github.com/user-attachments/assets/ba7ee4af-4f1a-4f6d-bdac-ff32fb8ca193)

- if user chooses old game, he/she will be asked about the size. If game in this size was played before and saved, the user will be able continue the game. Otherwise, player will be send back to menu. The file with old game also contains information about number of mistakes and board with numbers which were given at the beginning.
![image](https://github.com/user-attachments/assets/529faad6-d760-4d7a-8642-2d2217c1ecda)

To play, user has to type in number of row and column where the number should be placed, and value of this number. Before entering value into the board, program checks if such a move is allowed. If not, the information will be displayed and value will not be placed.
![image](https://github.com/user-attachments/assets/205c9855-de84-4f05-a3e4-e8198ac79743)

When user want to stop playing and board is not complitely filled, he/she can choose whether to save game or not.
![image](https://github.com/user-attachments/assets/9b65efaf-8f92-430e-ae6a-cbff4b8318aa)


## Problems:
- changes in files with saved games might cause problems with loading old games.

## Meme for teacher:
![The history of Germany by Jeremy Clarkson](https://github.com/user-attachments/assets/9c99fec7-a40a-463d-ac7f-8afdc7539e71)
