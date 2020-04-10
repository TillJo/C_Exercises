#include<math.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <f2fs_fs.h>

#define TO_CART "-c"
#define TO_POL "-p"
#define WRONG_INPUT "Please enter -c with r and phi or -p with real and imaginary part!\n"

typedef struct {
    double real;
    double imaginary;
} CartComplexNum;

typedef struct {
    double r;
    double phi;
} PolComplexNum;

static bool isValidInput(int argc, const char *const *argv);

void convert(const char *flag, double firstArg, double secondArg);

CartComplexNum polToCart(double r, double phi);

PolComplexNum cartToPol(double real, double imaginary);

void printToPol(PolComplexNum *polComplexNum);

void printToCart(CartComplexNum *cartComplexNum);

int main(int argc, char const *argv[]) {

    if (isValidInput(argc, argv)) {
        const char *flag = argv[1];
        double firstArg = atof(argv[2]);
        double secondArg = atof(argv[3]);
        convert(flag, firstArg, secondArg);
    } else {
        printf(WRONG_INPUT);
    }

    return 0;
}

bool isValidInput(int argc, const char *const *argv) {
    return argc > 2 && isdigit(*argv[2]) && isdigit(*argv[3]);
}

void convert(const char *flag, double firstArg, double secondArg) {
    if (strcmp(flag, TO_POL) == 0) {
        PolComplexNum polComplexNum = cartToPol(firstArg, secondArg);
        printToPol(&polComplexNum);
    } else if (strcmp(flag, TO_CART) == 0) {
        CartComplexNum cartComplexNum = polToCart(firstArg, secondArg);
        printToCart(&cartComplexNum);
    } else {
        printf(WRONG_INPUT);
    }
}

void printToCart(CartComplexNum *cartComplexNum) {
    printf("Real is: %f.10\n", (*cartComplexNum).real);
    printf("Complex is: %f.10i\n", (*cartComplexNum).imaginary);
}

void printToPol(PolComplexNum *polComplexNum) {
    printf("R is: %f.10\n", (*polComplexNum).r);
    printf("Phi is: %f.10i\n", (*polComplexNum).phi);
}

CartComplexNum polToCart(double r, double phi) {
    CartComplexNum complexNum;
    complexNum.real = r * cos(phi);
    complexNum.imaginary = r * sin(phi);
    return complexNum;
}

PolComplexNum cartToPol(double real, double imaginary) {
    PolComplexNum complexNum;
    complexNum.r = sqrt(pow(real, 2) + pow(imaginary, 2));
    complexNum.phi = atan(imaginary / real);
    return complexNum;
}
