#include <Python.h>
#include <stdio.h>
#include <string>

#define BOARD_SIZE 10

typedef unsigned int BoardRow;

typedef struct {
    PyObject_HEAD
    BoardRow level[BOARD_SIZE];
    BoardRow goal_positions[BOARD_SIZE];
    BoardRow box_positions[BOARD_SIZE];
    int player_x;
    int player_y;
} SokobanBoardObject;

static int SokobanBoard_init(SokobanBoardObject *self, PyObject *args, PyObject *kwds) {
    PyObject *input;
    if (!PyArg_ParseTuple(args, "O", &input)) {
        return -1;
    }
    if (PyList_Check(input)) {
        Py_ssize_t len = PyList_Size(input);
        if (len != BOARD_SIZE) {
            PyErr_SetString(PyExc_ValueError, "Invalid list length");
            return -1;
        }

        for (int i = 0; i < BOARD_SIZE; i++) {
            PyObject *row_str = PyList_GetItem(input, i);
            // Check if the row is a string
            if (!PyUnicode_Check(row_str)) {
                PyErr_SetString(PyExc_TypeError, "Invalid row type");
                return -1;
            }

            const char *row = PyUnicode_AsUTF8(row_str);

            if (strlen(row) != BOARD_SIZE) {
                PyErr_SetString(PyExc_ValueError, "Invalid string length");
                return -1;
            }

            for (int j = 0; j < BOARD_SIZE; j++) {
                char c = row[j];
                if (c == '@') {
                    self->player_x = j;
                    self->player_y = i;
                } else if (c == '$') {
                    self->box_positions[i] |= (1 << j);
                } else if (c == '*') {
                    self->goal_positions[i] |= (1 << j);
                } else if (c == '#') {
                    self->level[i] |= (1 << j);
                }
            }
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid argument type");
        return -1;
    }
    return 0;
}

static PyObject* SokobanBoard_string(SokobanBoardObject *self) {
    std::string boardstr= "";

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int shift = (1 << j);
            if (i == self->player_y && j == self->player_x) {
                boardstr += '@';
            }
            else if (self->box_positions[i] & shift) {
                boardstr += '$';
            }
            else if (self->goal_positions[i] & shift) {
                boardstr += '*';
            }
            else if (self->level[i] & shift) {
                boardstr += '#';
            }
            else {
                boardstr += ' ';
            }
        }

        if (i < BOARD_SIZE - 1) {
            boardstr += '\n';
        }
    }

    PyObject *boardStr = PyUnicode_FromString(boardstr.c_str());
    return boardStr;
}

static PyObject* SokobanBoard_is_solved(SokobanBoardObject *self) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (self->box_positions[i] != self->goal_positions[i]) {
            Py_RETURN_FALSE;
        }
    }
    Py_RETURN_TRUE;
}

static PyObject* SokobanBoard_move(SokobanBoardObject *self, PyObject *args) {
    int direction;
    if (!PyArg_ParseTuple(args, "i", &direction)) {
        PyErr_SetString(PyExc_TypeError, "Invalid argument type");
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

    int x = self->player_x;
    int y = self->player_y;
    int new_x = x + dx;
    int new_y = y + dy;
    int next_char = self->level[new_y] & (1 << new_x);

    if ((self->box_positions[new_y] & (1 << new_x)) != 0) {
        int box_x = new_x + dx;
        int box_y = new_y + dy;
        next_char = self->level[box_y] & (1 << box_x);

        if (next_char == 0 && (self->box_positions[box_y] & (1 << box_x)) == 0) {
            self->box_positions[new_y] &= ~(1 << new_x);
            self->player_x = new_x;
            self->player_y = new_y;
            self->box_positions[box_y] |= (1 << box_x);
        }
    } else if (next_char == 0) {
        self->player_x = new_x;
        self->player_y = new_y;
    }

    Py_RETURN_NONE;
}

static PyObject* SokobanBoard_player_pos(SokobanBoardObject *self) {
    return Py_BuildValue("(ii)", self->player_x, self->player_y);
}

static PyMethodDef SokobanBoard_methods[] = {
    {"__str__", (PyCFunction)SokobanBoard_string, METH_NOARGS, "Return a string representation of the board."},
    {"is_solved", (PyCFunction)SokobanBoard_is_solved, METH_NOARGS, "Check if the board is solved."},
    {"move", (PyCFunction)SokobanBoard_move, METH_VARARGS, "Move the player in the specified direction."},
    {"player_pos", (PyCFunction)SokobanBoard_player_pos, METH_NOARGS, "Return the player's position."},
    {NULL}
};

static PyTypeObject SokobanBoardType = {
    PyVarObject_HEAD_INIT(NULL, 0) "SokobanBoard", /* tp_name */
    sizeof(SokobanBoardObject),              /* tp_basicsize */
    0,                                       /* tp_itemsize */
    0,                                       /* tp_dealloc */
    0,                                       /* tp_print */
    0,                                       /* tp_getattr */
    0,                                       /* tp_setattr */
    0,                                       /* tp_reserved */
    0,                                       /* tp_repr */
    0,                                       /* tp_as_number */
    0,                                       /* tp_as_sequence */
    0,                                       /* tp_as_mapping */
    PyObject_HashNotImplemented,             /* tp_hash */
    0,                                       /* tp_call */
    (reprfunc)SokobanBoard_string,           /* tp_str */
    0,                                       /* tp_getattro */
    0,                                       /* tp_setattro */
    0,                                       /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    0,                                       /* tp_doc */
    0,                                       /* tp_traverse */
    0,                                       /* tp_clear */
    0,                                       /* tp_richcompare */
    0,                                       /* tp_weaklistoffset */
    0,                                       /* tp_iter */
    0,                                       /* tp_iternext */
    SokobanBoard_methods,                    /* tp_methods */
    0,                                       /* tp_members */
    0,                                       /* tp_getset */
    0,                                       /* tp_base */
    0,                                       /* tp_dict */
    0,                                       /* tp_descr_get */
    0,                                       /* tp_descr_set */
    0,                                       /* tp_dictoffset */
    (initproc)SokobanBoard_init,             /* tp_init */
    PyType_GenericAlloc,                     /* tp_alloc */
    PyType_GenericNew,                       /* tp_new */
    PyObject_Free,                           /* tp_free */
};

static PyModuleDef BoardModuleDef = {PyModuleDef_HEAD_INIT,
                                     "sokoban_board",
                                     NULL,
                                     -1,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL};

PyMODINIT_FUNC PyInit_sokoban_board(void) {
    PyObject* module;
    if (PyType_Ready(&SokobanBoardType) < 0) {
        return NULL;
    }

    module = PyModule_Create(&BoardModuleDef);
    if (module == NULL) {
        return NULL;
    }
    PyModule_AddObject(module, "SokobanBoard", (PyObject*)&SokobanBoardType);
    return module;
}