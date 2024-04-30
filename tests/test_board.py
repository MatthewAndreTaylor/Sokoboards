import pytest
from sokoban_board import SokobanBoard


# Test cases for SokobanBoard initialization
def test_board_initialization_valid():
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
    sokoban_board = SokobanBoard(board)
    assert sokoban_board is not None
    assert "\n".join(board) == str(sokoban_board)
    assert sokoban_board.is_solved() == False
    assert sokoban_board.player_pos() == (3, 3)

    sokoban_board.move(1)

    assert sokoban_board.player_pos() == (3, 4)


def test_board_initialization_invalid_list_length():
    board = [
        "#########",
        "#     #  #",
        "#  $     #",
        "#  @     #",
        "#        #",
        "#        #",
        "#     *  #",
        "#  $     #",
        "#      * #",
    ]
    with pytest.raises(ValueError):
        SokobanBoard(board)


def test_board_initialization_invalid_argument_type():
    with pytest.raises(TypeError):
        SokobanBoard("invalid_argument")
