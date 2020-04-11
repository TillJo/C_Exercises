#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#define DEAD_SYMBOL " | "
#define LIVE_SYMBOL " * "
#define NEIGHBOUR_TO_LIVE 3
#define NEIGHBOUR_TO_STAY 2
#define TO_PERCENTAGE 100


typedef enum {
    TO_ALIVE, KILL, SAME
} CellStatusTrans;

typedef enum {
    ALIVE, DEAD
} CellStatus;


typedef struct {
    CellStatus **world;
    int xSize;
    int ySize;
    int alive;
    int diffToLastGen;
} Generation;

bool notPosOfCell(int yPos, int xPos, int i, int j);

void mallocWorld(int ySize, int xSize, Generation *generation);

Generation createFirstGenerationRandomly(int ySize, int xSize, double propAlive);

void printGeneration(Generation generation);

Generation calcNextGeneration(Generation lastGen);

Generation initGeneration(Generation *lastGen);

Generation calcWorld(Generation *lastGen, Generation *nextGen);

CellStatusTrans getTransitionForNextGen(int yCellPos, int xCellPos, Generation lastGen);

bool isValidArgComb(int argc, const char *const *argv);

int main(int argc, char const *argv[]) {
    if (isValidArgComb(argc, argv)) {
        int ySize = atoi(argv[1]);
        int xSize = atoi(argv[2]);
        double probToLive = atof(argv[3]);

        Generation currentGeneration = createFirstGenerationRandomly(ySize, xSize, probToLive);
        Generation nextGeneration;

        int numOfGenerations = atoi(argv[4]);
        double delayInMs = atof(argv[5]) * 1000;
        int generationCount = 0;
        while (generationCount < numOfGenerations) {
            printGeneration(currentGeneration);
            nextGeneration = calcNextGeneration(currentGeneration);
            free(currentGeneration.world);
            currentGeneration = nextGeneration;
            usleep(delayInMs);
            generationCount++;
        }

        free(nextGeneration.world);
    } else {
        printf("Please enter valid combination of arg. The arguments are: "
               "ySize xSize probabilityToBeAlive numOfGenerations delayBetweenGenerationsInMs\n");
    }

    return 0;
}


/**
 *
 * @param ySize count of rows of the world
 * @param xSize count of columns of the world
 * @param propAlive probability of one cell to be alive in first generation
 * @return created generation
 */
Generation createFirstGenerationRandomly(int ySize, int xSize, double propAlive) {
    Generation generation;
    generation.ySize = ySize;
    generation.xSize = xSize;
    generation.alive = 0;
    generation.diffToLastGen = 0;
    mallocWorld(ySize, xSize, &generation);
    srand ( time(NULL) );

    for (int i = 0; i < ySize; i++) {
        for (int j = 0; j < xSize; j++) {
            double random = (rand() % TO_PERCENTAGE + 1) ;
            if (random < propAlive * TO_PERCENTAGE) {
                generation.world[i][j] = ALIVE;
                generation.alive++;
            } else {
                generation.world[i][j] = DEAD;
            }
        }
    }

    return generation;
}

/**
 * Calculate next generation dependent on last generation
 * @param lastGen last generation in history
 * @return next generation in history
 */
Generation calcNextGeneration(Generation lastGen) {
    Generation nextGen = initGeneration(&lastGen);
    nextGen = calcWorld(&lastGen, &nextGen);
    nextGen.diffToLastGen = nextGen.alive - lastGen.alive;
    return nextGen;
}

/**
 * Calculate world of next generation by using last.
 * @param lastGen last generation of history
 * @param nextGen next generation of history
 * @return calculated world
 */
Generation calcWorld(Generation *lastGen, Generation *nextGen) {
    for (int i = 0; i < (*lastGen).ySize; i++) {
        for (int j = 0; j < (*lastGen).xSize; j++) {
            CellStatusTrans cellStatusTrans = getTransitionForNextGen(i, j, (*lastGen));
            switch (cellStatusTrans) {
                case TO_ALIVE:
                    (*nextGen).world[i][j] = ALIVE;
                    break;
                case KILL:
                    (*nextGen).world[i][j] = DEAD;
                    break;
                case SAME:
                    (*nextGen).world[i][j] = (*lastGen).world[i][j];
                    break;
                default:
                    break;
            }

            if ((*nextGen).world[i][j] == ALIVE) {
                (*nextGen).alive++;
            }
        }
    }
    return (*nextGen);
}


/**
 * Calculate number of neighbours in world and return TO_ALIVE if number equals NEIGHBOUR_TO_LIVE/3, SAME if
 * number equals NEIGHBOUR_TO_STAY/2 otherwise kill.
 * @param yCellPos yPos in world of cell in last and next generation
 * @param xCellPos xPos in world of cell in last and next generation
 * @param lastGen last generation used for count number of neighbours
 * @return
 */
CellStatusTrans getTransitionForNextGen(int yCellPos, int xCellPos, Generation lastGen) {
    int countOfNeighbors = 0;
    for (int i = yCellPos - 1; i < yCellPos + 2; i++) {
        for (int j = xCellPos - 1; j < xCellPos + 2; j++) {
            int yLookPos = (i + lastGen.ySize) % lastGen.ySize;
            int xLookPos = (j + lastGen.xSize) % lastGen.xSize;

            if (lastGen.world[yLookPos][xLookPos] == ALIVE && notPosOfCell(yCellPos, xCellPos, i, j)) {
                countOfNeighbors++;
            }
        }
    }
    switch (countOfNeighbors) {
        case NEIGHBOUR_TO_LIVE:
            return TO_ALIVE;
        case NEIGHBOUR_TO_STAY:
            return SAME;
        default:
            return KILL;
    }
}


/**
 * Init generation struct with size of last generation, zero alive and empty/only malloced world.
 * @param lastGen
 * @return
 */
Generation initGeneration(Generation *lastGen) {
    Generation nextGen;
    nextGen.alive = 0;
    nextGen.ySize = (*lastGen).ySize;
    nextGen.xSize = (*lastGen).xSize;
    mallocWorld((*lastGen).ySize, (*lastGen).xSize, &nextGen);
    return nextGen;
}


/**
 * Print generation to cmd. For alive cells using LIVE_SYMBOL and dead cells DEAD_SYMBOL
 * @param generation
 */
void printGeneration(Generation generation) {

    printf("\n");
    for (int i = 0; i < generation.ySize; i++) {
        for (int j = 0; j < generation.xSize; j++) {
            if (generation.world[i][j] == ALIVE) {
                printf(LIVE_SYMBOL);
            } else {
                printf(DEAD_SYMBOL);
            }
        }
        printf("\n");
    }

    printf("Total Alive: %d\n", generation.alive);
    printf("Diff to last: %d\n", generation.diffToLastGen);
}

void mallocWorld(int ySize, int xSize, Generation *generation) {
    (*generation).world = malloc(ySize * xSize * sizeof(CellStatus *));
    (*generation).world = (CellStatus **) malloc(ySize * sizeof(CellStatus *));
    for (int i = 0; i < ySize; i++) {
        *((*generation).world + i) = (CellStatus *) malloc(xSize * sizeof(CellStatus));
    }
}

bool notPosOfCell(int yPos, int xPos, int i, int j) {
    return !(i == yPos && j == xPos);
}

bool isValidArgComb(int argc, const char *const *argv) {
    return argc > 5 && isdigit(*argv[1]) && isdigit(*argv[2])
           && isdigit(*argv[3]) && isdigit(*argv[4]) && isdigit(*argv[5]);
}
