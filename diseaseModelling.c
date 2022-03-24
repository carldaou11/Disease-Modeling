#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>
#include <math.h>

#define POPULATION 1000

struct individual{
	float x;
	float y;
	bool disease;
};

struct society{
	int population;
	struct individual popArray[POPULATION];
	int numInfected;
	float chanceOfInfection;
};

struct society listSocieties[8];

bool isInRadius(struct individual ind1, struct individual ind2){

	float diff_first = ind1.x - ind2.x + 0.0;
	float diff_second = ind1.y - ind2.y + 0.0;
	float first_squared = diff_first * diff_first;
	float second_squared = diff_second * diff_second;

	double sum = first_squared + second_squared;

	double distance = sqrt(sum);

	if((distance > 0) && (distance <= 6)){
		return true;
	} else {
		return false;
	}
}

bool getsDisease(struct individual ind1, float chance){
	int chanceHundred = 100 * chance;
	int randInt = rand() % 100;

	if(randInt <= chanceHundred){
		ind1.disease = true;
		return true;
	}

	return false;
}

void randomizeLocation(struct society soc1){
	for(int i = 0; i < 1000; i++){
		int randx1 = rand() % 100;
		float randx2 = (float) (rand() / (RAND_MAX));
		int randy1 = rand() % 100;
		float randy2 = (float) (rand() / (RAND_MAX));
		soc1.popArray[i].x = randx1 + randx2;
		soc1.popArray[i].y = randy1 + randy2;
	}
}

int main(int argc, char** argv){

	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

	struct society society1;
	struct society society2;
	struct society society3;
	struct society society4;
	struct society society5;
	struct society society6;
	struct society society7;
	struct society society8;

	society1.numInfected = 0;
	society1.chanceOfInfection = 0.1;
	society1.population = POPULATION;

	society2.numInfected = 0;
	society2.chanceOfInfection = 0.15;
	society2.population = POPULATION;

	society3.numInfected = 0;
	society3.chanceOfInfection = 0.2;
	society3.population = POPULATION;

	society4.numInfected = 0;
	society4.chanceOfInfection = 0.125;
	society4.population = POPULATION;

	society5.numInfected = 0;
	society5.chanceOfInfection = 0.175;
	society5.population = POPULATION;

	society6.numInfected = 0;
	society6.chanceOfInfection = 0.225;
	society6.population = POPULATION;

	society7.numInfected = 0;
	society7.chanceOfInfection = 0.15;
	society7.population = POPULATION;

	society8.numInfected = 0;
	society8.chanceOfInfection = 0.25;
	society8.population = POPULATION;

	for(int a = 0; a < 8; a++){
		for(int b = 0; b < 1000; b++){
			struct individual ind1;
			ind1.x = 0.0;
			ind1.y = 0.0;
			getsDisease(ind1, 0.025);
			listSocieties[a].popArray[b] = ind1;
		}
	}

	if(rank == 0){
		struct society listSocieties[8] = {society1, society2, society3, society4, society5, society6, society7, society8};
		for(int n = 0; n < 8; n++){
			int n2 = n + 1;
			printf("society %d:\npercentage: %.3f \n", n2, (listSocieties[n].chanceOfInfection));
		}
	}

	float timeStart = MPI_Wtime();

	for(int m = 0; m < 10; m++){
		printf("time unit %d: \n", m);
		for(int i = 0; i < 8; i++){
			int i2 = i + 1;
			printf("society %d:\nnumber of people with disease: %d \n", i2, (listSocieties[i].numInfected));
			randomizeLocation(listSocieties[i]);
			for(int j = 0; j < 999; j++){
				for(int k = rank + j + 1; k < 999; k += size){
					if(isInRadius(listSocieties[i].popArray[k], listSocieties[i].popArray[j])){
						if(!(listSocieties[i].popArray[k].disease)){
							if(getsDisease(listSocieties[i].popArray[k], listSocieties[i].chanceOfInfection)){
								listSocieties[i].numInfected++;
							}
						}
					}
				}
			}
		}
		printf("\n\n");
	}
	float time = MPI_Wtime() - timeStart;
	printf("time taken: %f", time);

	MPI_Finalize();
}