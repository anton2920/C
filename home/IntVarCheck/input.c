#include <stdio.h>
#include "input.h"

/* This small library supposed to be a «scanf» replacement, but it's not finished, though.
 * The problem of «scanf» is typo errors: if you type letter instead of a number (if it should be a number),
 * then the entire program will crash or something. My library should help you (a bit :) ) */

/* Function declarations */
int lpow(int power);

/* Function defenitions */
int lpow(int power) {

    /* Initializing variables */
    int i, ans = 1;

    /* Main part */
    for (i = 0; i < power; ++i) {
        ans *= 10; /* Calculating the power of ten */
    }
    return ans;
}

long int linput(const char *out, long int *res) {

    /* This function allows you to assign some integer value to a variable. If you type something but integer,
     * the program asks you to type proper value. It's an endless loop, so you must type an integer */

    /* Initializing variables */
    int a, flag = -1, multiplier = 1, counter = 0;
    *res = 0;

    /* Main part */
    while (1) {
        printf("%s", out); /* Prints user-specified string */
        while (1) {
            a = getchar(); /* Gets one typed char at a time */
            if (a == '-') { /* Detects a negative sign */
                multiplier = -1;
            } else if (a == '+') { /* Detects a positive sign */
                multiplier = 1;
            } else if (a >= '0' && a <= '9' && flag != 0) { /* Detects digits */
                *res = (*res * 10) + (a - '0'); /* Making of the answer */
                flag = 1; /* «1» if a digit was detected */
                ++counter; /* Counts a number of successfully scanned digits */
            } else if (a != '\n' && a != '+' && a != '-') { /* Detects «garbage» */
                flag = 0; /* «0» if «garbage» was detected */
                *res = 0; /* Answer is equals to zero now */
                counter = 0;
                multiplier = 1;
            } else if (a == '\n') { /* Detects End-Of-Line char */
                break; /* End-Of-Line («Enter» button usually) is the only way to finish a number */
            }
        }
        if (flag == 1) { /* Checks whether only digits were typed */
            break; /* Congrats! Your number were successfully scanned */
        }
        flag = -1; /* «-1» if a previous attempt to scan a number was a fail */
    }
    *res *= multiplier; /* Applies negative sign, if necessary */

    return counter; /* Returns a number of successfully scanned digits */
}

int input(const char *out, int *res) {

    /* This function allows you to assign some integer value to a variable. If you type something but integer,
     * the program asks you to type proper value. It's an endless loop, so you must type an integer */

    /* Initializing variables */
    int a, flag = -1, multiplier = 1, counter = 0;
    *res = 0;

    /* Main part */
    while (1) {
        printf("%s", out); /* Prints user-specified string */
        while (1) {
            a = getchar(); /* Gets one typed char at a time */
            if (a == '-') { /* Detects a negative sign */
                multiplier = -1;
            } else if (a == '+') { /* Detects a positive sign */
                multiplier = 1;
            } else if (a >= '0' && a <= '9' && flag != 0) { /* Detects digits */
                *res = (*res * 10) + (a - '0'); /* Making of the answer */
                flag = 1; /* «1» if a digit was detected */
                ++counter; /* Counts a number of successfully scanned digits */
            } else if (a != '\n' && a != '+' && a != '-') { /* Detects «garbage» */
                flag = 0; /* «0» if «garbage» was detected */
                *res = 0; /* Answer is equals to zero now */
                counter = 0;
                multiplier = 1;
            } else if (a == '\n') { /* Detects End-Of-Line char */
                break; /* End-Of-Line («Enter» button usually) is the only way to finish a number */
            }
        }
        if (flag == 1) { /* Checks whether only digits were typed */
            break; /* Congrats! Your number were successfully scanned */
        }
        flag = -1; /* «-1» if a previous attempt to scan a number was a fail */
    }
    *res *= multiplier; /* Applies negative sign, if necessary */

    return counter; /* Returns a number of successfully scanned digits */    
}

double lfinput(const char *out, double *res) {

    /* This function allows you to assign some floating-point value to a variable. If you type something but floating-point,
     * the program asks you to type proper value. It's an endless loop, so you must type a floating-point value
     * It's a bit more complicated in comparison with int «input». But still, it's relatively simple */

    /* Initializing variables */
    int a, flag = -1, multiplier = 1, counter = 0, counter2 = 1, flag2 = 0, power10 = 0, flag3 = 0, multiplier2 = 1;
    *res = 0;

    /* Main part */
    while (1) {
        printf("%s", out); /* Prints user-specified string */
        while (1) {
            a = getchar(); /* Gets one typed char at a time */
            if (a == '-' && flag3 == 0) { /* Detects a negative sign of a number */
                multiplier = -1;
            } else if (a == '+' && flag3 == 0) { /* Detects a positive sign of a number */
                multiplier = 1;
            } else if (a == '-' && flag3 == 1) { /* Detects a negative sign of an exponent-part (scientific notation) */
                multiplier2 = -1;
            } else if (a == '+' && flag3 == 1) { /* Detects a positive sign of an exponent-part (scientific notation) */
                multiplier2 = 1;
            } else if (a >= '0' && a <= '9' && flag != 0) { /* Detects digits… */
                if (flag2 == 0 && flag3 == 0) { /* …of a significand's whole part */
                    *res = (*res * 10) + (a - '0'); /* Making of the answer */
                    flag = 1; /* «1» if a digit was detected*/
                    ++counter; /* Counts a number of successfully scanned digits */
                } else if (flag2 == 1 && flag3 == 0) { /* of a significand's fractional part */
                    *res = (*res * 10) + (a - '0'); /* Making of the number plus fractional part */
                    flag = 1; /* «1» if a digit was detected */
                    ++counter; /* Counts a number of successfully scanned digits */
                    counter2 *= 10; /* Counts a number of digits in the fractional part as a power of ten*/
                } else if (flag3 == 1) { /* …of an exponent */
                    power10 = (power10 * 10) + (a - '0'); /* Making of the exponent */
                    ++counter; /* Counts a number of successfully scanned digits */
                }
            } else if (a == '.' && flag2 == 0) { /* Detects the decimal point */
                flag2 = 1; /* «1» if the decimal point was detected */
            } else if ((a == 'e' || a == 'E') && flag3 == 0) { /* Detects the beginning of an exponent part */
                flag3 = 1; /* «1» if the beginning of an exponent part was detected */
            } else if (a != '\n' && a != '+' && a != '-') { /* Detects «garbage» and reinitializes variables */
                flag = 0; /* «0» if «garbage» was detected */
                *res = 0;
                counter2 = 1;
                counter = 0;
                power10 = 0;
                flag2 = 0;
                flag3 = 0;
                multiplier = 1;
                multiplier2 = 1;
            } else if (a == '\n') { /* Detects End-Of-Line char */
                break; /* End-Of-Line («Enter» button usually) is the only way to finish a number */
            }


        }
        if (flag == 1) { /* Checks whether only digits were typed */
            break; /* Congrats! Your number were successfully scanned */
        }
        flag = -1; /* «-1» if a previous attempt to scan a number was a fail */
    }
    *res /= counter2; /* Making of a «full» floating-point number (both whole and fractional parts */
    *res *= (power10 != 0) ? ((multiplier2 > 0) ? lpow(power10) : (1.0 / lpow(power10))) : 1; /* Making a scientific view applies, if necessary */
    *res *= multiplier; /* Applies negative sign, if necessary */

    return counter; /* Returns a number of successfully scanned digits */
}


float finput(const char *out, float *res) {

    /* This function allows you to assign some floating-point value to a variable. If you type something but floating-point,
     * the program asks you to type proper value. It's an endless loop, so you must type a floating-point value
     * It's a bit more complicated in comparison with int «input». But still, it's relatively simple */

    /* Initializing variables */
    int a, flag = -1, multiplier = 1, counter = 0, counter2 = 1, flag2 = 0, power10 = 0, flag3 = 0, multiplier2 = 1;
    *res = 0;

    /* Main part */
    while (1) {
        printf("%s", out); /* Prints user-specified string */
        while (1) {
            a = getchar(); /* Gets one typed char at a time */
            if (a == '-' && flag3 == 0) { /* Detects a negative sign of a number */
                multiplier = -1;
            } else if (a == '+' && flag3 == 0) { /* Detects a positive sign of a number */
                multiplier = 1;
            } else if (a == '-' && flag3 == 1) { /* Detects a negative sign of an exponent-part (scientific notation) */
                multiplier2 = -1;
            } else if (a == '+' && flag3 == 1) { /* Detects a positive sign of an exponent-part (scientific notation) */
                multiplier2 = 1;
            } else if (a >= '0' && a <= '9' && flag != 0) { /* Detects digits… */
                if (flag2 == 0 && flag3 == 0) { /* …of a significand's whole part */
                    *res = (*res * 10) + (a - '0'); /* Making of the answer */
                    flag = 1; /* «1» if a digit was detected*/
                    ++counter; /* Counts a number of successfully scanned digits */
                } else if (flag2 == 1 && flag3 == 0) { /* of a significand's fractional part */
                    *res = (*res * 10) + (a - '0'); /* Making of the number plus fractional part */
                    flag = 1; /* «1» if a digit was detected */
                    ++counter; /* Counts a number of successfully scanned digits */
                    counter2 *= 10; /* Counts a number of digits in the fractional part as a power of ten*/
                } else if (flag3 == 1) { /* …of an exponent */
                    power10 = (power10 * 10) + (a - '0'); /* Making of the exponent */
                    ++counter; /* Counts a number of successfully scanned digits */
                }
            } else if (a == '.' && flag2 == 0) { /* Detects the decimal point */
                flag2 = 1; /* «1» if the decimal point was detected */
            } else if ((a == 'e' || a == 'E') && flag3 == 0) { /* Detects the beginning of an exponent part */
                flag3 = 1; /* «1» if the beginning of an exponent part was detected */
            } else if (a != '\n' && a != '+' && a != '-') { /* Detects «garbage» and reinitializes variables */
                flag = 0; /* «0» if «garbage» was detected */
                *res = 0;
                counter2 = 1;
                counter = 0;
                power10 = 0;
                flag2 = 0;
                flag3 = 0;
                multiplier = 1;
                multiplier2 = 1;
            } else if (a == '\n') { /* Detects End-Of-Line char */
                break; /* End-Of-Line («Enter» button usually) is the only way to finish a number */
            }


        }
        if (flag == 1) { /* Checks whether only digits were typed */
            break; /* Congrats! Your number were successfully scanned */
        }
        flag = -1; /* «-1» if a previous attempt to scan a number was a fail */
    }
    *res /= counter2; /* Making of a «full» floating-point number (both whole and fractional parts */
    *res *= (power10 != 0) ? ((multiplier2 > 0) ? lpow(power10) : (1.0 / lpow(power10))) : 1; /* Making a scientific view applies, if necessary */
    *res *= multiplier; /* Applies negative sign, if necessary */

    return counter; /* Returns a number of successfully scanned digits */
}