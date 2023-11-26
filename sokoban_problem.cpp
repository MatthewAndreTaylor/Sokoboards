#include <Python.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>

#define BOARD_SIZE 10

typedef unsigned int BoardRow;

typedef struct {
    BoardRow level[BOARD_SIZE];
    BoardRow goal_positions[BOARD_SIZE];
} BoardConfig;

typedef struct {
    BoardRow box_positions[BOARD_SIZE];;
    int player_x;
    int player_y;
} BoardMoves;

bool operator==(const BoardMoves& lhs, const BoardMoves& rhs) {
    return lhs.player_x == rhs.player_x && 
           lhs.player_y == rhs.player_y &&
           std::equal(std::begin(lhs.box_positions), std::end(lhs.box_positions), std::begin(rhs.box_positions));
}

bool operator<(const BoardMoves& lhs, const BoardMoves& rhs) {
    if (lhs.player_x != rhs.player_x) {
        return lhs.player_x < rhs.player_x;
    }
    if (lhs.player_y != rhs.player_y) {
        return lhs.player_y < rhs.player_y;
    }
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (lhs.box_positions[i] != rhs.box_positions[i]) {
            return lhs.box_positions[i] < rhs.box_positions[i];
        }
    }
    return false;
}

class SokobanProblem {
public:
    BoardMoves start_state;
    BoardConfig problem_board;

    std::vector<BoardMoves> successors(const BoardMoves& currentState) {
        std::vector<BoardMoves> succ;
        int dxList[] = {0, 0, -1, 1};
        int dyList[] = {-1, 1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int dx = dxList[i];
            int dy = dyList[i];

            int x = currentState.player_x;
            int y = currentState.player_y;
            int new_x = x + dx;
            int new_y = y + dy;
            int next_char = problem_board.level[new_y] & (1 << new_x);

            if ((currentState.box_positions[new_y] & (1 << new_x)) != 0) {
                int box_x = new_x + dx;
                int box_y = new_y + dy;
                next_char = problem_board.level[box_y] & (1 << box_x);

                if (next_char == 0 && (currentState.box_positions[box_y] & (1 << box_x)) == 0) {
                    BoardMoves newState = currentState;
                    newState.box_positions[new_y] &= ~(1 << new_x);
                    newState.player_x = new_x;
                    newState.player_y = new_y;
                    newState.box_positions[box_y] |= (1 << box_x);
                    succ.push_back(newState);
                }
            } else if (next_char == 0) {
                BoardMoves newState = currentState;
                newState.player_x = new_x;
                newState.player_y = new_y;
                succ.push_back(newState);
            }
        }

        return succ;
    }

    bool is_goal(const BoardMoves& currentState) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (currentState.box_positions[i] != problem_board.goal_positions[i]) {
                return false;
            }
        }
        return true;
    }
};

size_t breadthFirstSearch(SokobanProblem& problem) {
    std::queue<BoardMoves> queue;
    std::set<BoardMoves> visited;
    size_t nodes_expanded = 0;
    queue.push(problem.start_state);

    for (; !queue.empty(); queue.pop()) {
        BoardMoves state = queue.front();

        if (problem.is_goal(state)) {
            return nodes_expanded;
        }

        auto it = visited.find(state);
        if (it == visited.end()) {
            std::vector<BoardMoves> successors = problem.successors(state);
            nodes_expanded += 1;
            visited.insert(state);

            for (const auto& next_state : successors) {
                queue.push(next_state);
            }
        }
    }

    return 0;
}

SokobanProblem problem;

static PyObject *fitness(PyObject *self, PyObject *args) {
    PyObject *input;
    if (!PyArg_ParseTuple(args, "O", &input)) {
        return NULL;
    }

    if (PyList_Check(input)) {
        int len = PyList_Size(input);
        if (len != BOARD_SIZE) {
            PyErr_SetString(PyExc_ValueError, "Invalid list length");
            return NULL;
        }

        for (int i = 0; i < BOARD_SIZE; i++) {
            problem.problem_board.level[i] = 0;
            problem.start_state.box_positions[i] = 0;
            problem.problem_board.goal_positions[i] = 0;
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
                    problem.start_state.player_x = j;
                    problem.start_state.player_y = i;
                } else if (c == '$') {
                    problem.start_state.box_positions[i] |= (1 << j);
                } else if (c == '*') {
                    problem.problem_board.goal_positions[i] |= (1 << j);
                } else if (c == '#') {
                    problem.problem_board.level[i] |= (1 << j);
                }
            }
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid argument type");
        return NULL;
    }

    return PyLong_FromSize_t(breadthFirstSearch(problem));
}

static PyMethodDef ProblemMethods[] = {
    {"fitness", (PyCFunction)fitness, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef sokoproblemmodule = {PyModuleDef_HEAD_INIT, "sokoban_problem",
                                             NULL, -1, ProblemMethods};

PyMODINIT_FUNC PyInit_sokoban_problem() { return PyModule_Create(&sokoproblemmodule); }
