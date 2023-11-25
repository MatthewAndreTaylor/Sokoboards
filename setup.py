from setuptools import setup, Extension

setup(
    ext_modules=[
        Extension("sokoban_board", ["sokoboard.c"]),
        Extension("sokoban_problem", ["sokoban_problem.cpp"]),
    ]
)
