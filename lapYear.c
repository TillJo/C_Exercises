#include<stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool isLapYear(int year);


int main(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        if (isdigit(*argv[i]) != 0) {
            int year = atoi(argv[i]);

            if (isLapYear(year)) {
                printf("%d is a lap year\n", year);
            } else {
                printf("%d isn't a lap year\n", year);
            }

        } else {
            printf("%s isn't a valid year\n", argv[i]);
        }
    }

    return 0;
}

bool isLapYear(int year) {
    bool fourIsReminder = year % 4 == 0;
    bool hundredIsReminder = year % 100 == 0;
    bool fourHundredIsReminder = year % 400 == 0;

    return fourIsReminder && (!hundredIsReminder || fourHundredIsReminder);
}
