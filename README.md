# Sokoboards 📦

Implementation of the Sokoban game where the player has to push boxes onto targets within a confined space. 

https://en.wikipedia.org/wiki/Sokoban

## Requirements

- Python 3.x
- blessed (install using `pip install blessed`)
- setuptools

## Usage

1. Clone this repository to your local machine.
2. Navigate to the project directory.
3. Install the extensions `python setup.py install`
3. Run the `game/sokoban.py` script using Python.

```bash
python game/sokoban.py
```

4. Alternatively run the lite version `game/sokoban_lite.py` (no c++ install required)

```bash
python game/sokoban_lite.py
```

5. Create your own levels check how long a computer would take to solve them

```py
import sokoban_problem

initial_board = [
    "##########",
    "#@       #",
    "#     $  #",
    "#        #",
    "#  ###   #",
    "#        #",
    "#     *  #",
    "#  $     #",
    "#      * #",
    "##########",
]

num_expanded = sokoban_problem.num_expanded(initial_board)

print(f"Took {num_expanded} steps") # Took 7409 steps

path = sokoban_problem.path(initial_board)
direction_map = {0: "Up", 1: "Down", 2: "Left", 3: "Right"}
solution = list(map(direction_map.get, path))
print(solution) # ['Right', 'Right', 'Right', 'Right', 'Right', 'Down', 'Down', 'Down', 'Down', 'Left', 'Down', 'Left', 'Left', 'Down', 'Left', 'Down', 'Right', 'Right', 'Right', 'Right']
```

6. Checkout some of the work I did to randomly generate levels in  `/trials`