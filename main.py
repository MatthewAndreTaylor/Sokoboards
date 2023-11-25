from blessed import Terminal
import sokoban_board
class Sokoban:
    def __init__(self):
        self.term = Terminal()

    def draw(self):
        with self.term.cbreak(), self.term.hidden_cursor():
            print(self.term.home + sokoban_board.string())

    def move(self, direction):
        sokoban_board.move(direction)

        if sokoban_board.is_solved():
            sokoban.draw()
            print("Congratulations! You solved the puzzle!")
            exit(0)


if __name__ == "__main__":
    sokoban = Sokoban()
    board = [
        "##########",
        "#     #  #",
        "#  $     #",
        "#  @     #",
        "#        #",
        "#        #",
        "#     *  #",
        "#  $     #",
        "#      * #",
        "##########",
    ]
    sokoban_board.setup(board)
    print(sokoban.term.clear())

    with sokoban.term.cbreak():
        while True:
            sokoban.draw()
            key = sokoban.term.inkey()

            if key.code == sokoban.term.KEY_UP:
                sokoban.move(0)
            elif key.code == sokoban.term.KEY_DOWN:
                sokoban.move(1)
            elif key.code == sokoban.term.KEY_LEFT:
                sokoban.move(2)
            elif key.code == sokoban.term.KEY_RIGHT:
                sokoban.move(3)



import random
def generate_random_board(size=10):
    board = (['#' * size] +
             ['#' + ''.join(random.choices(['#', ' '], [30, 70]) for _ in range(size - 2)) + '#' for _ in range(size - 2)] +
             ['#' * size])

    for _ in range(2):
        y, x = random.randrange(1, size - 1), random.randrange(1, size - 1)
        board[y] = board[y][:x] + '*' + board[y][x+1:]

    for _ in range(2):
        while True:
            y, x = random.randrange(1, size - 1), random.randrange(1, size - 1)
            if sum(1 for dy, dx in [(-1, 0), (1, 0), (0, -1), (0, 1)] if board[y + dy][x + dx] == '#') <= 1:
                board[y] = board[y][:x] + '$' + board[y][x+1:]
                break

    return board