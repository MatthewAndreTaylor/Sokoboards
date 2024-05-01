import pytest
import sokoban_problem

direction_map = {0: "Up", 1: "Down", 2: "Left", 3: "Right"}


def test_simple_problem():
    initial_board = [
        "##########",
        "#@       #",
        "#$       #",
        "#*       #",
        "#  ###   #",
        "#        #",
        "#        #",
        "#        #",
        "#        #",
        "##########",
    ]
    num_expanded = sokoban_problem.num_expanded(initial_board)
    assert num_expanded == 1

    path = sokoban_problem.path(initial_board)
    dirs = list(map(direction_map.get, path))
    assert dirs == ["Down"]

    initial_board = [
        "##########",
        "#  @     #",
        "#  $     #",
        "#   *    #",
        "#  ###   #",
        "#        #",
        "#        #",
        "#        #",
        "#        #",
        "##########",
    ]
    num_expanded = sokoban_problem.num_expanded(initial_board)
    assert num_expanded == 28

    path = sokoban_problem.path(initial_board)
    dirs = list(map(direction_map.get, path))
    assert dirs == ["Down", "Left", "Down", "Right"]


def test_unsolvable_problem():
    initial_board = [
        "##########",
        "#@       #",
        "# $      #",
        "#        #",
        "##########",
        "# *      #",
        "#        #",
        "#        #",
        "#        #",
        "##########",
    ]
    num_expanded = sokoban_problem.num_expanded(initial_board)
    assert num_expanded == 0

    path = sokoban_problem.path(initial_board)
    assert len(path) == 0

    initial_board = [
        "##########",
        "#@       #",
        "#$       #",
        "# *      #",
        "#  ###   #",
        "#        #",
        "#        #",
        "#        #",
        "#        #",
        "##########",
    ]
    num_expanded = sokoban_problem.num_expanded(initial_board)
    assert num_expanded == 0
    
    path = sokoban_problem.path(initial_board)
    assert len(path) == 0
