from blessed import Terminal


class Sokoban:
    def __init__(self):
        self.term = Terminal()
        self.player_pos = (1, 1)
        self.box_positions = [
            list("          "),
            list("          "),
            list("          "),
            list("      #   "),
            list("          "),
            list("          "),
            list("          "),
            list("   #      "),
            list("          "),
            list("          "),
        ]
        self.goal_positions = [
            list("          "),
            list("          "),
            list("          "),
            list("          "),
            list("          "),
            list(" #        "),
            list("          "),
            list("       #  "),
            list("          "),
            list("          "),
        ]
        self.level = [
            list("##########"),
            list("#        #"),
            list("#        #"),
            list("#        #"),
            list("#        #"),
            list("#        #"),
            list("#        #"),
            list("#        #"),
            list("#        #"),
            list("##########"),
        ]

    def draw(self):
        with self.term.cbreak(), self.term.hidden_cursor():
            for y, row in enumerate(self.level):
                for x, char in enumerate(row):
                    if (x, y) == self.player_pos:
                        print(self.term.move(y, x) + "@", end="")
                    elif self.box_positions[y][x] != " ":
                        print(self.term.move(y, x) + "$", end="")
                    elif self.goal_positions[y][x] != " ":
                        print(self.term.move(y, x) + "*", end="")
                    else:
                        print(self.term.move(y, x) + char, end="")

    def move(self, direction):
        x, y = self.player_pos
        dx, dy = direction

        new_x, new_y = x + dx, y + dy
        next_char = self.level[new_y][new_x]

        if self.box_positions[new_y][new_x] != " ":
            box_x, box_y = new_x + dx, new_y + dy
            next_char = self.level[box_y][box_x]
            if next_char == " ":
                self.box_positions[new_y][new_x] = " "
                self.player_pos = new_x, new_y
                self.box_positions[box_y][box_x] = "#"
        elif next_char == " ":
            self.player_pos = (new_x, new_y)

        if sokoban.is_solved():
            sokoban.draw()
            print("Congratulations! You solved the puzzle!")
            exit(0)

    def is_solved(self):
        return self.box_positions == self.goal_positions


if __name__ == "__main__":
    sokoban = Sokoban()
    print(sokoban.term.clear())

    with sokoban.term.cbreak():
        while True:
            sokoban.draw()
            key = sokoban.term.inkey()

            if key.code == sokoban.term.KEY_UP:
                sokoban.move((0, -1))
            elif key.code == sokoban.term.KEY_DOWN:
                sokoban.move((0, 1))
            elif key.code == sokoban.term.KEY_LEFT:
                sokoban.move((-1, 0))
            elif key.code == sokoban.term.KEY_RIGHT:
                sokoban.move((1, 0))
