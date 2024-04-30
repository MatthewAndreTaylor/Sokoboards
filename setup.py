from setuptools import setup, Extension

setup(
    ext_modules=[
        Extension("sokoban_board", ["sokoboard.cpp"]),
        Extension("sokoban_problem", ["sokoban_problem.cpp"]),
    ]
)
