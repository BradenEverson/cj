#include "cj.h"
#include <stddef.h>

int hash(char* str) {
    if (str == NULL) {
        return -1;
    }

    int result = 0;
    int idx = 0;

    while (str[idx] != '\0') {
        result += str[idx];
    }

    return result;
}
