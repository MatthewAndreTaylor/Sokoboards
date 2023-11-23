#include <Python.h>
#include <stdio.h>

#define BOARD_SIZE 10

typedef unsigned int BoardRow;

BoardRow level[BOARD_SIZE] = {0};
BoardRow box_positions[BOARD_SIZE] = {0};
BoardRow goal_positions[BOARD_SIZE] = {0};
int player_x = 1;
int player_y = 1;

void printBoard(BoardRow board[BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ", (board[i] >> j) & 1);
        }
        printf("\n");
    }
}

static PyObject* string(PyObject *self) {
    PyObject* boardStr = PyUnicode_New(BOARD_SIZE * (BOARD_SIZE + 1), 127);
    int index = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int shift = (1 << j);
            if (i == player_y && j == player_x) {
                PyUnicode_WriteChar(boardStr, index++, '@');
            }
            else if (box_positions[i] & shift) {
                PyUnicode_WriteChar(boardStr, index++, '$');
            }
            else if (goal_positions[i] & shift) {
                PyUnicode_WriteChar(boardStr, index++, '*');
            }
            else if (level[i] & shift) {
                PyUnicode_WriteChar(boardStr, index++, '#');
            }
            else {
                PyUnicode_WriteChar(boardStr, index++, ' ');
            }
        }

        if (i < BOARD_SIZE - 1) {
            PyUnicode_WriteChar(boardStr, index++, '\n');
        }
    }

    PyUnicode_WriteChar(boardStr, index, '\0');
    return boardStr;
}

static PyObject* is_solved(PyObject *self) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (box_positions[i] != goal_positions[i]) {
            Py_RETURN_FALSE;
        }
    }
    Py_RETURN_TRUE;
}

static PyObject *setup(PyObject *self, PyObject *args) {
    PyObject *input;
    if (!PyArg_ParseTuple(args, "O", &input)) {
        return NULL;
    }
    if (PyUnicode_Check(input)) {
        const char *input_str = PyUnicode_AsUTF8(input);
        int len = strlen(input_str);

        if (len != BOARD_SIZE * BOARD_SIZE) {
            PyErr_SetString(PyExc_ValueError, "Invalid string length");
            return NULL;
        }

        int index = 0;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                char c = input_str[index++];
                if (c == '@') {
                    player_x = j;
                    player_y = i;
                } else if (c == '$') {
                    box_positions[i] |= (1 << j);
                } else if (c == '*') {
                    goal_positions[i] |= (1 << j);
                } else if (c == '#') {
                    level[i] |= (1 << j);
                }
            }
        }
    } else if (PyList_Check(input)) {
        int len = PyList_Size(input);
        if (len != BOARD_SIZE) {
            PyErr_SetString(PyExc_ValueError, "Invalid list length");
            return NULL;
        }

        for (int i = 0; i < BOARD_SIZE; i++) {
            PyObject *row_str = PyList_GetItem(input, i);
            const char *row = PyUnicode_AsUTF8(row_str);

            if (strlen(row) != BOARD_SIZE) {
                PyErr_SetString(PyExc_ValueError, "Invalid string length");
                return NULL;
            }

            for (int j = 0; j < BOARD_SIZE; j++) {
                char c = row[j];
                if (c == '@') {
                    player_x = j;
                    player_y = i;
                } else if (c == '$') {
                    box_positions[i] |= (1 << j);
                } else if (c == '*') {
                    goal_positions[i] |= (1 << j);
                } else if (c == '#') {
                    level[i] |= (1 << j);
                }
            }
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid argument type");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *move(PyObject *self, PyObject *args) {
    int direction;
    if (!PyArg_ParseTuple(args, "i", &direction)) {
        return NULL;
    }

    int dx, dy;
    switch (direction) {
        case 0:  // Up
            dx = 0; dy = -1;
            break;
        case 1:  // Down
            dx = 0; dy = 1;
            break;
        case 2:  // Left
            dx = -1; dy = 0;
            break;
        case 3:  // Right
            dx = 1; dy = 0;
            break;
        default:
            PyErr_SetString(PyExc_ValueError, "Invalid direction");
            return NULL;
    }

    int x = player_x;
    int y = player_y;
    int new_x = x + dx;
    int new_y = y + dy;
    int next_char = level[new_y] & (1 << new_x);

    if ((box_positions[new_y] & (1 << new_x)) != 0) {
        int box_x = new_x + dx;
        int box_y = new_y + dy;
        next_char = level[box_y] & (1 << box_x);

        if (next_char == 0 && (box_positions[box_y] & (1 << box_x)) == 0) {
            box_positions[new_y] &= ~(1 << new_x);
            player_x = new_x;
            player_y = new_y;
            box_positions[box_y] |= (1 << box_x);
        }
    } else if (next_char == 0) {
        player_x = new_x;
        player_y = new_y;
    }

    Py_RETURN_NONE;
}

static PyMethodDef BoardMethods[] = {
    {"setup", (PyCFunction)setup, METH_VARARGS, NULL},
    {"move", (PyCFunction)move, METH_VARARGS, NULL},
    {"is_solved", (PyCFunction)is_solved, METH_NOARGS, NULL},
    {"string", (PyCFunction)string, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef sokobanmodule = {PyModuleDef_HEAD_INIT, "sokoban_board",
                                             NULL, -1, BoardMethods};

PyMODINIT_FUNC PyInit_sokoban_board() { return PyModule_Create(&sokobanmodule); }
