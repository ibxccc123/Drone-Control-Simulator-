# Drone-Control-Simulator-
A grid-based simulator in C++ which simulates delivery aircrafts’ pathfinding and object avoidance.

NOTE: Makefile is within Source directory.

The executable will output a key, followed by a grid.  The key for the grid is:

0 - Empty air (can be occupied by a drone)

1 - Drone (cannot have another drone occupy the same airspace)

2 - Runway (destination where drones drop off their packages)

3 - Drone Control Center / Home Base (origin where drones pick up packages and return to)

4 - Runway / Home Base occupied by a drone (cannot have another drone occupy the same airspace)


Initially, all drones begin in the home base and have packages ready to deliver.

The grid is randomly generated with random placements of the runways and the drone control center.
The driver file takes in three arguments: the size of the grid, the number of runways, and number of drones.
The default arguments are: 10, 5, 5.  This will generate a 10x10 grid, with 5 runways and 5 drones at the DCC.

Each turn, drones can move one position in a cardinal direction on the grid.  Text output will be accompanied
by the visual grid output.  

NOTE: While 10 drones have worked during testing, runtime will be slightly slow at the beginning 
(approximately 20 seconds for a single turn to pass) due to having sleep() function calls in order 
to have a text output speed which is readable. 
