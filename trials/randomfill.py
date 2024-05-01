import random
import sokoban_problem

initial_board = [
    "##########",
    "#@       #",
    "#     $  #",
    "#        #",
    "#        #",
    "#        #",
    "#     *  #",
    "#  $     #",
    "#      * #",
    "##########",
]

board = initial_board
next_board = board

for i in range(20000):
    # Select a random location
    x, y = random.randint(1, 8), random.randint(1, 8)
    if board[y][x] == " ":
        next_board = (
            board[:y] + [board[y][:x] + "#" + board[y][x + 1 :]] + board[y + 1 :]
        )

    if sokoban_problem.num_expanded(next_board) > 0:
        board = next_board


print("\n".join(board))
