

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <ctype.h>
#include <f2fs_fs.h>


int lengthOfInteger(int i);

void printMatrix(int *triangle, int lengthOfMatrix, int rows);

void printNumber(int number, int maxLengthOfNums);

int *createPascalTriangle(int rows);

void addOneEdgesToTriangle(int nums, int *pascalTriangle);

void calculateNormalNumbers(int nums, int *pascalTriangle);

void printSpaceNTimes(int n);

int gaussianSumFormula(int rows);

int maxLengthOfIntsInNums(int *nums, int size);

bool isValidInput(int argc, char *const *argv);

void addBeginOfTriangle(int *pascalTriangle);

int main(int argc, char *argv[]) {
    if (isValidInput(argc, argv)) {
        int size = atoi(argv[1]);
        int *pascalTriangle = createPascalTriangle(size);
        printMatrix(pascalTriangle, gaussianSumFormula(size), size);
    } else {
        printf("Please enter a valid number as first argument!\n");
    }
    return 0;
}

bool isValidInput(int argc, char *const *argv) {
    return argc > 1 && isdigit(*argv[1]) != 0;
}

int *createPascalTriangle(int rows) {
    int nums = gaussianSumFormula(rows);
    int *pascalTriangle = malloc(nums * sizeof(int));
    addBeginOfTriangle(pascalTriangle);
    addOneEdgesToTriangle(nums, pascalTriangle);
    calculateNormalNumbers(nums, pascalTriangle);
    return pascalTriangle;
}


/**
 * First fourth entries are one.
 * @param pascalTriangle pascalTriangle to create
 */
void addBeginOfTriangle(int *pascalTriangle) {
    pascalTriangle[0] = 1;
    pascalTriangle[1] = 1;
    pascalTriangle[2] = 1;
    pascalTriangle[3] = 1;
}

void calculateNormalNumbers(int nums, int *pascalTriangle) {
    int row = 3;
    int numInRow = 0;
    for (int i = 4 ; i < nums; i++) {
        if (pascalTriangle [i] != 1) {
            pascalTriangle[i] = pascalTriangle[i - row] + pascalTriangle[i - row + 1];
        }
        numInRow++;
        if (numInRow == row) {
            row++;
            numInRow = 0;
        }
    }
}

/*
 * Add ones to the triangle. In graphical representation this would be every number that is at the start or end of a
 * row.
 */
void addOneEdgesToTriangle(int nums, int *pascalTriangle) {
    int jumps = 1;

    for (int i = 4 ; i < nums; i++) {
        i += jumps;
        pascalTriangle[i] = 1;
        pascalTriangle[i + 1] = 1;
        i++;
        jumps++;
    }
}

void printMatrix(int *triangle, int lengthOfMatrix, int rows) {

    int maxLength = maxLengthOfIntsInNums(triangle, lengthOfMatrix) + 1;
    int middleOfMatrix = ( (rows + 1) * maxLength) / 2;
    int spaceAtNewLine = middleOfMatrix - maxLength;

    int lengthOfRow = 1;
    int idx = 0;

    while (idx < lengthOfMatrix) {
        int printedOfRow = 0;
        printSpaceNTimes(spaceAtNewLine);
        while (printedOfRow < lengthOfRow) {
            printNumber(triangle[idx], maxLength);
            idx++;
            printedOfRow++;
        }
            spaceAtNewLine -= (maxLength / 2);

        printf("\n");
        lengthOfRow++;
    }
}


void printNumber(int number, int maxLengthOfNums) {
    int space = (maxLengthOfNums - lengthOfInteger(number)) / 2 + 1;
    printf("%d", number);
    printSpaceNTimes(space);
}


void printSpaceNTimes(int n) {
    for (int i = 0; i < n; i++) {
        printf(" ");
    }
}

int maxLengthOfIntsInNums(int *nums, int size) {
    int length = 1;
    for (int i = 0 ; i < size ; i++) {
        if (lengthOfInteger(nums[i]) > length) {
            length = lengthOfInteger(nums[i]);
        }
    }

    return length;
}

int lengthOfInteger(int i) {
    return  (i==0 ) ? 1 : floor(log10(abs(i))) + 1;
}

int gaussianSumFormula(int rows) {
    return (int) (pow((double) rows, 2) + rows) / 2;
}
