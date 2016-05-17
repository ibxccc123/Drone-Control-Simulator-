/*Danny Peng 50031597
 *Header file that contains function declarations for printing the grid and controlling the drones.
 */

#ifndef DRONE_H
#define DRONE_H

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <cstdlib>

using namespace std;


void printGrid();
void createRunway();
void *createDrone(void *threadid);

#endif
