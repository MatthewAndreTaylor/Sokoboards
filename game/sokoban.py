from blessed import Terminal
from sokoban_board import SokobanBoard


class Sokoban:
    def __init__(self, board):
        self.term = Terminal()
        self.board = SokobanBoard(board)

    def draw(self):
        with self.term.cbreak(), self.term.hidden_cursor():
            print(self.term.home + str(self.board))

    def move(self, direction):
        self.board.move(direction)

        if self.board.is_solved():
            sokoban.draw()
            print("Congratulations! You solved the puzzle!")
            exit(0)


if __name__ == "__main__":
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
    sokoban = Sokoban(board)
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
