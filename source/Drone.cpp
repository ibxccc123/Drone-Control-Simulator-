/*Danny Peng 50031597
 *Drone file containing function definitions for everything involved with drone control.
 */

#include "Drone.h"

int* droneList;
int** grid;
int size;
int numRunways;
int numDrones;
bool Free;

void printGrid(){
        for(int i=0; i<size; i++){
                for(int j=0; j<size; j++){
                        cout << grid[i][j] << " ";
                }
                cout << endl;
        }
                sleep(1);
}

void createRunway(){
        int R1,C1;
	int RDecoy,CDecoy;

        for(int i=0; i<size; i++){  //Locating Command Center
                for(int j=0; j<size; j++){
                	if(grid[i][j] == 3){
                        	RDecoy = i;
                        	CDecoy = j;
                      	}
              	}
        }

        for(int i=0; i<numRunways; i++){
                do{
                        srand(time(NULL));
                        R1 = rand() % size;
                        C1 = rand() % size;
                }while(grid[R1][C1] != 0 || R1 == RDecoy || C1 == CDecoy);
                grid[R1][C1] = 2; //2 is location of a runway (dropoff location)
        }
}


void *createDrone(void *threadid){
        long tid;
        tid = long(threadid);
        int RDecoy;
        int CDecoy;
        int RRunways[numRunways];
        int CRunways[numRunways];
        int counter = 0;
        for(int i=0; i<size; i++){  //Each drone knows where each runway and where the control center is
                for(int j=0; j<size; j++){
                        if(grid[i][j] != 0){
                                if(grid[i][j] == 3){
                                        RDecoy = i;
                                        CDecoy = j;
                                }
                                else if(grid[i][j] == 2){
                                        RRunways[counter] = i;
                                        CRunways[counter] = j;
                                        counter++;
                                }
                        }
                }
        }
	//Initial Lock
	lock1:
	while(true){
         if(tid == 0 && droneList[0] == 0){
	   goto start1;
	}
	 else if(tid > 0){
	   if(droneList[tid-1] == 1 && droneList[tid] == 0){
             goto start1;
	   }
	 }
        }
	//End lock
	
	//Normal lock
	lock2:
	while(true){
         if(tid == 0 && droneList[0] == 0){
	   Free = false;
	   goto start2;
	}
	 else if(tid > 0 && Free == true && droneList[tid-1] == 1 && droneList[tid] == 0){
	     Free = false;
             goto start2;
	}
	   sleep(1);
	}
	//End lock



		//Mutual Exclusion Zone
		start1:
		sleep(3);
                srand(time(NULL));
        	cout << "Drone " << tid << " is registered under DRONE" <<  endl;
                int destination;
		destination = rand () % counter;
                int RDestination;
		RDestination = RRunways[destination];  //If counter = 1 (only 1 runway), then output will always be 0
                int CDestination;
		CDestination = CRunways[destination];
                cout << "Drone " << tid << " is currently at the Drone Control Center and is heading to Runway " << destination << " at coordinates " << RDestination << " " << CDestination << endl;
                int proximity;
		proximity = 0;
                int RCurrent;
		RCurrent = RDecoy;
                int CCurrent; 
		CCurrent = CDecoy;
		if(tid == numDrones-1){
                	printGrid();
			for(int i=0;i < numDrones;i++){
				droneList[i] = 0;
			}
		}
		else{
			droneList[tid] = 1;
		}
		goto lock2;

		start2:
			sleep(1);
			while(true){
                        switch(proximity){
                        	case 0:
                                        if(RDecoy < RDestination){
                                                proximity = 1;
						break;
                                        }
                                        else if(RDecoy > RDestination){
                                                proximity = 2;
						break;
                                        }
                                        else{
                                                proximity = 3;
						break;
                                        }
                                case 1:
					if(grid[RCurrent + 1][CCurrent] == 1 || grid[RCurrent + 1][CCurrent] == 4){
					  cout << "Drone " << tid << " is currently waiting at current location at " << RCurrent << " " << CCurrent << " to prevent a collision with another drone" << endl;
						sleep(2);
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
					  goto lock2;
					} 
					else{
                                        if(grid[RCurrent][CCurrent] == 1){
                                                grid[RCurrent][CCurrent] = 0;
                                        }
					if(grid[RCurrent][CCurrent] == 4){
						grid[RCurrent][CCurrent] = 2;
					}
					if(RCurrent == RDecoy && CCurrent == CDecoy){
						grid[RCurrent][CCurrent] = 3;
					}
                                        RCurrent++;
					if(!(grid[RCurrent][CCurrent] == 2)){
						grid[RCurrent][CCurrent] = 1;
					}
					else{
						grid[RCurrent][CCurrent] = 4;
					}
                                        cout << "Drone " << tid << " is currently en-route to " <<  RDestination << " " << CDestination << " and has moved to " << RCurrent  << " " << CCurrent << endl;
                                        if(RCurrent < RDestination){
                                                proximity = 1;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
                                        else{
                                                proximity = 3;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
					}
                                case 2:
					if(grid[RCurrent - 1][CCurrent] == 1 || grid[RCurrent - 1][CCurrent] == 4){
					  cout << "Drone " << tid << " is currently waiting at current location at " << RCurrent << " " << CCurrent << " to prevent a collision with another drone" << endl;
						sleep(2);
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
					  goto lock2;
					} 
					else{
                                        if(grid[RCurrent][CCurrent] == 1){
                                                grid[RCurrent][CCurrent] = 0;
                                        }
					if(grid[RCurrent][CCurrent] == 4){
						grid[RCurrent][CCurrent] = 2;
					}
					if(RCurrent == RDecoy && CCurrent == CDecoy){
						grid[RCurrent][CCurrent] = 3;
					}
                                        RCurrent--;
					if(!(grid[RCurrent][CCurrent] == 2)){
						grid[RCurrent][CCurrent] = 1;
					}
					else{
						grid[RCurrent][CCurrent] = 4;
					}
                                        cout << "Drone " << tid << " is currently en-route to " << RDestination << " " << CDestination << " and has moved to " << RCurrent  << " " << CCurrent << endl;
                                        if(RCurrent > RDestination){
                                                proximity = 2;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
                                        else{
                                                proximity = 3;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
					}
                                case 3: 
					if(CDecoy < CDestination){
                                                proximity = 4;
                                                break;
                                        }
                                        else if(CDecoy > CDestination){
                                                proximity = 5;
                                                break;
                                        }
					else{
						proximity = 6;
						break;
					}					
				case 4:
					if(grid[RCurrent][CCurrent + 1] == 1 || grid[RCurrent][CCurrent + 1] == 4){
					  cout << "Drone " << tid << " is currently waiting at current location at " << RCurrent << " " << CCurrent << " to prevent a collision with another drone" << endl;
						sleep(2);
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
					  goto lock2;
					} 
					else{
					if(grid[RCurrent][CCurrent] == 1){
                                        	grid[RCurrent][CCurrent] = 0;
                                        }
					if(grid[RCurrent][CCurrent] == 4){
						grid[RCurrent][CCurrent] = 2;
					}
					if(RCurrent == RDecoy && CCurrent == CDecoy){
						grid[RCurrent][CCurrent] = 3;
					}
                                        CCurrent++;
					if(!(grid[RCurrent][CCurrent] == 2)){
						grid[RCurrent][CCurrent] = 1;
					}
					else{
						grid[RCurrent][CCurrent] = 4;
					}
                                        cout << "Drone " << tid << " is currently en-route to " << RDestination << " " << CDestination << " and has moved to " << RCurrent  << " " << CCurrent << endl;
                                        if(CCurrent < CDestination){
                                        	proximity = 4;
						if(tid == numDrones-1){
                                        	printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
                                        else{
                                                proximity = 6;
						if(tid == numDrones-1){
                                        	printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
					}
				case 5:
					if(grid[RCurrent][CCurrent - 1] == 1 || grid[RCurrent][CCurrent - 1] == 4){
					  cout << "Drone " << tid << " is currently waiting at current location at " << RCurrent << " " << CCurrent << " to prevent a collision with another drone" << endl;
						sleep(2);
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
					  goto lock2;
					} 
					else{
					if(grid[RCurrent][CCurrent] == 1){
                                                grid[RCurrent][CCurrent] = 0;
                                        }
					if(grid[RCurrent][CCurrent] == 4){
						grid[RCurrent][CCurrent] = 2;
					}
					if(RCurrent == RDecoy && CCurrent == CDecoy){
						grid[RCurrent][CCurrent] = 3;
					}
                                        CCurrent--;
					if(!(grid[RCurrent][CCurrent] == 2)){
						grid[RCurrent][CCurrent] = 1;
					}
					else{
						grid[RCurrent][CCurrent] = 4;
					}
                                        cout << "Drone " << tid << " is currently en-route to " << RDestination << " " << CDestination << " and has moved to " << RCurrent  << " " << CCurrent << endl;
                                        if(CCurrent > CDestination){
                                                proximity = 5;
						if(tid == numDrones-1){
                                        	printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
                                        else{
                                                proximity = 6;
						if(tid == numDrones-1){
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
                                        	printGrid();
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
					}											
				case 6: 
					cout << "Drone " << tid << " has reached its destination at " << RCurrent << " " << CCurrent << " and is landing down" << endl;
					cout << "Drone " << tid << " has dropped its package" << endl;
					cout << "Drone " << tid << " is preparing to lift off" << endl;
					sleep(2);
					if(RCurrent < RDecoy){
                                                proximity = 7;
						if(tid == numDrones-1){
                                        	printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
                                        else if(RCurrent > RDecoy){
                                                proximity = 8;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
                                        }
					else{
						proximity = 9;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
				case 7:
					if(grid[RCurrent + 1][CCurrent] == 1 || grid[RCurrent + 1][CCurrent] == 4){
					  cout << "Drone " << tid << " is currently waiting at current location at " << RCurrent << " " << CCurrent << " to prevent a collision with another drone" << endl;
						sleep(2);
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
					  goto lock2;
					}
					else{ 
					if(grid[RCurrent][CCurrent] == 1){
                                                grid[RCurrent][CCurrent] = 0;
                                        }
					if(grid[RCurrent][CCurrent] == 4){
						grid[RCurrent][CCurrent] = 2;
					}
					if(RCurrent == RDecoy && CCurrent == CDecoy){
						grid[RCurrent][CCurrent] = 3;
					}
                                        RCurrent++;
					if(grid[RCurrent][CCurrent] != 2 && grid[RCurrent][CCurrent] != 3 ){
						grid[RCurrent][CCurrent] = 1;
					}
					else{
						grid[RCurrent][CCurrent] = 4;
					}
                                        cout << "Drone " << tid << " is currently en-route back to the Drone Control Center and has moved to " << RCurrent  << " " << CCurrent << endl;
                                        if(RCurrent < RDecoy){
                                                proximity = 7;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
					else{
						proximity = 9;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
					}
				case 8:
					if(grid[RCurrent - 1][CCurrent] == 1 || grid[RCurrent - 1][CCurrent] == 4){
					  cout << "Drone " << tid << " is currently waiting at current location at " << RCurrent << " " << CCurrent << " to prevent a collision with another drone" << endl;
						sleep(2);
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
					  goto lock2;
					}
					else{
					if(grid[RCurrent][CCurrent] == 1){
                                                grid[RCurrent][CCurrent] = 0;
                                        }
					if(grid[RCurrent][CCurrent] == 4){
						grid[RCurrent][CCurrent] = 2;
					}
					if(RCurrent == RDecoy && CCurrent == CDecoy){
						grid[RCurrent][CCurrent] = 3;
					}
                                        RCurrent--;
					if(grid[RCurrent][CCurrent] != 2 && grid[RCurrent][CCurrent] != 3 ){
						grid[RCurrent][CCurrent] = 1;
					}
					else{
						grid[RCurrent][CCurrent] = 4;
					}
                                        cout << "Drone " << tid << " is currently en-route back to the Drone Control Center and has moved to " << RCurrent  << " " << CCurrent << endl;
                                        if(RCurrent > RDecoy){
                                                proximity = 8;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
					else{
						proximity = 9;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
					}
				case 9:
					if(CCurrent < CDecoy){
                                                proximity = 10;
                                                break;
                                        }
                                        else if(CCurrent > CDecoy){
                                                proximity = 11;
                                                break;
                                        }
					else{
						proximity = 12;
						break;
					}

				case 10:
					if(grid[RCurrent][CCurrent + 1] == 1 || grid[RCurrent][CCurrent + 1] == 4){
					  cout << "Drone " << tid << " is currently waiting at current location at " << RCurrent << " " << CCurrent << " to prevent a collision with another drone" << endl;
						sleep(2);
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
					  goto lock2;
					}
					else{
					if(grid[RCurrent][CCurrent] == 1){
                                                grid[RCurrent][CCurrent] = 0;
                                        }
					if(grid[RCurrent][CCurrent] == 4){
						grid[RCurrent][CCurrent] = 2;
					}
					if(RCurrent == RDecoy && CCurrent == CDecoy){
						grid[RCurrent][CCurrent] = 3;
					}
                                        CCurrent++;
					if(grid[RCurrent][CCurrent] != 2 && grid[RCurrent][CCurrent] != 3 ){
						grid[RCurrent][CCurrent] = 1;
					}
					else{
                                                grid[RCurrent][CCurrent] = 4;
					}
                                        cout << "Drone " << tid << " is currently en-route back to the Drone Control Center and has moved to " << RCurrent  << " " << CCurrent << endl;
                                        if(CCurrent < CDecoy){
                                                proximity = 10;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
					else{
						proximity = 12;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
					}
				case 11:
					if(grid[RCurrent][CCurrent - 1] == 1 || grid[RCurrent][CCurrent - 1] == 4){
					  cout << "Drone " << tid << " is currently waiting at current location at " << RCurrent << " " << CCurrent << " to prevent a collision with another drone" << endl;
						sleep(2);
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
					  goto lock2;
					}
					else{
					if(grid[RCurrent][CCurrent] == 1){
                                                grid[RCurrent][CCurrent] = 0;
                                        }
					if(grid[RCurrent][CCurrent] == 4){
						grid[RCurrent][CCurrent] = 2;
					}
					if(RCurrent == RDecoy && CCurrent == CDecoy){
						grid[RCurrent][CCurrent] = 3;
					}
                                        CCurrent--;
					if(grid[RCurrent][CCurrent] != 2 && grid[RCurrent][CCurrent] != 3 ){
						grid[RCurrent][CCurrent] = 1;
					}
					else{
                                                grid[RCurrent][CCurrent] = 4;
					}
                                        cout << "Drone " << tid << " is currently en-route back to the Drone Control Center and has moved to " << RCurrent  << " " << CCurrent << endl;
                                        if(CCurrent > CDecoy){
                                                proximity = 11;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
					else{
						proximity = 12;
						if(tid == numDrones-1){
                                        	  printGrid();
						  for(int i=0;i < numDrones;i++){
						    droneList[i] = 0;
	 					  }
						}
						else{
							droneList[tid] = 1;
						}
					  Free = true;
                                                goto lock2;
					}
					}
				case 12:
					cout << "Drone " << tid << " has reached Drone Control Center at " << RCurrent << " " << CCurrent << " and is landing down" << endl;
					cout << "Drone " << tid << " has picked up another package" << endl;
					cout << "Drone " << tid << " is preparing to lift off" << endl;
					if(tid == numDrones-1){
                                       	  printGrid();
					  for(int i=0;i < numDrones;i++){
					    droneList[i] = 0;
	 				  }
					}
					else{
						droneList[tid] = 1;
					}
					  Free = true;
                                        goto lock1;
                        }//End of switch loop
	}//End of while switch loop
        pthread_exit(NULL);
}
