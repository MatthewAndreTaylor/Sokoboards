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
