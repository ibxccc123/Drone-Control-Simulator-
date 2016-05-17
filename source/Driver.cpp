/*Danny Peng 50031597
 *Driver file that initiates the grid and drone threads.
 */

#include "Drone.h"

extern int* droneList;
extern int** grid;
extern int size;
extern int numRunways;
extern int numDrones;
extern bool Free;

int main(int argc, char** argv) {
	if(argc != 4){
		cout << "Usage: You must provide three arguments (size of grid, number of runways, number of drones) to execute.\n";
		return 1;
	}	
	//Set arguments of program into variables.
	size = atoi(argv[1]);
	numRunways = atoi(argv[2]);
	numDrones = atoi(argv[3]);
	droneList = new int[numDrones];	
	Free = false;
	for(int i=0;i<numDrones;i++){
		droneList[i] = 0;
	}
	//Initialize grid; 0 is empty space.
	grid = new int*[size];
	for(int i = 0; i < size; i++){
		grid[i] = new int[size];
	}
	//Initialize Drone Control Center/Home Base
	int RDecoy,CDecoy;
	srand(time(NULL));
	RDecoy = rand() % size;
	CDecoy = rand() % size;
	grid[RDecoy][CDecoy] = 3; //3 is location of drone control center
	//Convert size into string for cout.
	string Size;
	ostringstream convert1;
	convert1 << size;
	Size = convert1.str();
	//Creating runways.
	createRunway();
	//Convert numRunways into string for cout.	
	string Runways;
	ostringstream convert2;
	convert2 << numRunways;
	Runways = convert2.str();
	cout << "Grid after " + Runways + " runways have been placed\n";
	cout << "1: Drone" << endl;
	cout << "2: Runway" << endl;
	cout << "3: Drone Control Center / Home Base" << endl;
	cout << "4: Drone in Runway / Home Base Airspace" << endl;
	printGrid();
	sleep(5);
	//Creating drones.
	pthread_t threads[numDrones];
	int rc;
	for(int i=0; i<numDrones; i++){
		rc = pthread_create(&threads[i], NULL, createDrone, (void *)i);
	}
	pthread_exit(NULL);
	return (0);
}
