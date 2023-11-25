#include <Python.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>

#define BOARD_SIZE 10

typedef unsigned int BoardRow;

typedef struct {
    BoardRow level[BOARD_SIZE] = {0};
    BoardRow goal_positions[BOARD_SIZE] = {0};
} BoardConfig;

typedef struct {
    BoardRow box_positions[BOARD_SIZE] = {0};
    int player_x = 1;
    int player_y = 1;
} BoardMoves;

bool operator==(const BoardMoves& lhs, const BoardMoves& rhs) {
    return std::equal(std::begin(lhs.box_positions), std::end(lhs.box_positions), std::begin(rhs.box_positions))
        && lhs.player_x == rhs.player_x
        && lhs.player_y == rhs.player_y;
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

bool breadthFirstSearch(SokobanProblem& problem) {
    std::queue<BoardMoves> queue;
    std::vector<BoardMoves> visited;
    queue.push(problem.start_state);

    for (; !queue.empty(); queue.pop()) {
        BoardMoves state = queue.front();

        if (problem.is_goal(state)) {
            return true;
        }

        auto it = std::find(visited.begin(), visited.end(), state);
        if (it == visited.end()) {
            std::vector<BoardMoves> successors = problem.successors(state);
            visited.push_back(state);

            for (const auto& next_state : successors) {
                auto next_it = std::find(visited.begin(), visited.end(), next_state);
                if (next_it == visited.end()) {
                    queue.push(next_state);
                }
            }
        }
    }

    return false;
}

static PyMethodDef ProblemMethods[] = {
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef sokoproblemmodule = {PyModuleDef_HEAD_INIT, "sokoban_problem",
                                             NULL, -1, ProblemMethods};

PyMODINIT_FUNC PyInit_sokoban_problem() { return PyModule_Create(&sokoproblemmodule); }
