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

path_len = sokoban_problem.path_len(initial_board)

print(f"Took {path_len} steps") # Took 7409 steps
```

6. Checkout some of the work I did to randomly generate levels in  `/trials`