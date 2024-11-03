# Evolutionary TSP

Evolutionary algorithm that solves the Travelling Salesperson Problem.

Random European cities are taken as input. Full list of possible cities can be found in `res/european_cities.csv`. Map in the background addapted from [Wikimedia Commons](https://commons.m.wikimedia.org/wiki/File:Blank_Map_of_The_World_Equirectangular_Projection.png).

You can start and pause the execution using the spacebar button.

# Algorithm details

Numerical parameters listed in the table can be modified in `configuration.hpp` file.

(Table template below taken from A.E.Eiben, J.E.Smith, _Introduction to Evolutionary Computing_)

|    |     |
|-------------|-------------|
| **Representation**         | Permutation         |
| **Recombination** | Partially Mapped Crossover (PMX) |
| **Recombination probability** | 100% |
| **Mutation** | Inversion Mutation |
| **Mutation probability** | 80% |
| **Parent selection** | Tournament with size $k=5$ and a single winner |
| **Survival selection** | Generational, full replacement |
| **Population size** | 50 |
| **Number of offspring** | 50 |
| **Initialization** | Completely random |
| **Termination condition** | Fixed no. of 1000 generations |

# Demos

Simulation for 32 cities, population of 50 individuals and delay of 40 miliseconds between each generation. The result is definitely not optimal (edges Varna-Sofia and Bucharest-Athens cross) but it looks "good enough".

https://github.com/user-attachments/assets/beb3e77b-6dbe-4537-bda3-a5d2139daf36

Larger simulation with 48 cities, population of 200 individuals and a delay of 10 miliseconds. Plausible-looking result achieved in less than 200 generations.

https://github.com/user-attachments/assets/1d609ca8-8749-4f0f-b749-f7ec66acea44
