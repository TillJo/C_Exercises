#include <ctype.h>
#include <f2fs_fs.h>
#include<math.h>

#define INVALID_INPUT "Invalid Argument combination! Enter function [-cos -sin -tan] start end and offset\n"

bool isValidArgumentCombination(int argc, const char *const *argv);

double (*getFunction(const char *const *argv))(double);

void printValueTable(const char *const *argv);

bool isInvalidFktArgument();

int main(int argc, char const *argv[]) {

    if (isValidArgumentCombination(argc, argv)) {
        printValueTable(argv);
    } else {
        printf(INVALID_INPUT);
    }

    return 0;
}

void printValueTable(const char *const *argv) {

    double (*trigonometricFkt)(double) = getFunction(argv);
    if (isInvalidFktArgument(trigonometricFkt)) {
        printf(INVALID_INPUT);
    } else {
        float start = atof(argv[2]);
        float end = atof(argv[3]);
        float offset = atof(argv[4]);

        char fktNameBuff[4];
        memcpy(fktNameBuff, &argv[1][1], 3);
        fktNameBuff[3] = '\0';

        while (start <= end) {
            printf("Value for %s(%f.10)  : %f.10\n", fktNameBuff, start, trigonometricFkt(start));
            start += offset;
        }
    }
}

double (*getFunction(const char *const *argv))(double) {

    double (*trigonometricFkt)(double) = NULL;

    if (strcmp("-cos", argv[1]) == 0) {
        trigonometricFkt = cos;
    } else if (strcmp("-sin", argv[1]) == 0) {
        trigonometricFkt = sin;
    } else if (strcmp("-tan", argv[1]) == 0) {
        trigonometricFkt = tan;
    }
    return trigonometricFkt;
}

bool isInvalidFktArgument(double (*trigonometricFkt)(double)) {
    return trigonometricFkt == NULL;
}

bool isValidArgumentCombination(int argc, const char *const *argv) {
    return argc > 1
           && isdigit(*argv[2])
           && isdigit(*argv[3])
           && isdigit(*argv[4]);
}

float ownTan(float f) {
    return f * M_PI / 180;
}
