 #include<stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <f2fs_fs.h>

# define PLUS '+'
# define MINUS '-'
# define MUL '*'
# define DIV '/'


double add(double fristAddend, double secondAddend);

double sub(double minuend, double subtrahend);

double multi(double firstFactor, double secondFactor);

double divide(double dividend, double divisor);

bool isValidArgs(int argc, const char *const *argv);

void doCalculation(const char *const *argv, double firstArg, double secondArg);

int main(int argc, char const *argv[]) {


    if (isValidArgs(argc, argv)) {

        double firstArg = atof(argv[1]);
        double secondArg = atof(argv[3]);

        doCalculation(argv, firstArg, secondArg);
    }

    return 0;
}

bool isValidArgs(int argc, const char *const *argv) {
    return argc > 3 && isdigit(*argv[1]) && isdigit(*argv[3]);
}


void doCalculation(const char *const *argv, double firstArg, double secondArg) {
    char operator = *argv[2];
    switch (operator) {
        case PLUS:
            printf("%.2f\n", add(firstArg, secondArg));
            break;
        case MINUS:
            printf("%.2f\n", sub(firstArg, secondArg));
            break;
        case MUL:
            printf("%.2f\n", multi(firstArg, secondArg));
            break;
        case DIV:
            printf("%.2f\n", divide(firstArg, secondArg));
            break;
        default:
            printf("Invalid operation argument!:\n");
    }
}

double add(double fristAddend, double secondAddend) {
    return (fristAddend + secondAddend);
}

double sub(double minuend, double subtrahend) {
    return minuend - subtrahend;
}

double multi(double firstFactor, double secondFactor) {
    return firstFactor * secondFactor;
}

double divide(double dividend, double divisor) {
    return dividend / divisor;
}