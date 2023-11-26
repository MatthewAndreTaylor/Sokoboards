import random
import sokoban_problem

def fitness(board):
    return sokoban_problem.fitness(board)

def generate_boards(config):
    new_board = [list(row) for row in config]
    for i in range(len(config)):
        for j in range(len(config[i])):
            if config[i][j] == ' ' and random.random() < 0.15:
                new_board[i][j] = '#'
            elif config[i][j] == '#' and random.random() > 0.1 and 0 < i < 9 and 0 < j < 9:
                new_board[i][j] = ' '
    
    return [''.join(row) for row in new_board]

def mutation(config):
    new_board = [list(row) for row in config]
    attempts = 0
    if random.random() > 0.5:
        while attempts < 10:
            x, y = random.randrange(1,9), random.randrange(1,9)
            if config[x][y] == ' ':
                new_board[x][y] = '#'
                break
            attempts+=1
    else:
        while attempts < 10:
            x, y = random.randrange(1,9), random.randrange(1,9)
            if config[x][y] == '#':
                new_board[x][y] = ' '
                break
            attempts+=1
    
    return [''.join(row) for row in new_board]
    
                
                
def crossover(parent1, parent2):
    crossover_point = random.randint(0, len(parent1) - 1)
    child = parent1[:crossover_point] + parent2[crossover_point:]
    return child

def genetic_algorithm(initial_board, population_size, generations):
    population = [initial_board] + [generate_boards(initial_board) for _ in range(population_size)]

    for generation in range(generations):
        fitness_scores = [(board, fitness(board)) for board in population]
        fitness_scores.sort(key=lambda x: x[1], reverse=True)
        parents = [board for board, score in fitness_scores[:population_size // 2] if score > 0]

        # Generate offspring through crossover and mutation
        offspring = []
        while len(offspring) < population_size - 1:
            parent1 = random.choice(parents)
            parent2 = random.choice(parents)
            child0 = mutation(crossover(parent1, parent2))
            child1 = mutation(parent1)
            child2 = mutation(parent2)
            offspring.append(child0)
            offspring.append(child1)
            offspring.append(child2)

        # Replace the old population with the new generation
        population = offspring + parents

        # Print the best fitness score in each generation
        best_fitness = fitness_scores[0][1]
        print(f"Generation {generation + 1}: Best Fitness = {best_fitness}")

    # Return the best board from the final generation
    return fitness_scores[0][0]


initial_board = [
    '##########',
    "#@       #",
    "#     $  #",
    "#        #",
    "#        #",
    "#        #",
    "#     *  #",
    "#  $     #",
    "#      * #",
    "##########"
]

print(f"Generation {0}: Best Fitness = {fitness(initial_board)}")
for line in genetic_algorithm(initial_board, 1000, 10):
    print(f"\'{line}\',")