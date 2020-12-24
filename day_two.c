#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void day_two_solution(FILE* fp) {
    int valid = 0;

    int min = 0;
    int max = 0;
    char target = '\0';
    char pw[100] = {0};

    while (fscanf(fp, "%d-%d %c: %s\n", &min, &max, &target, pw) != EOF) {
        int count = 0;
        char* p;
        for (p = pw; *p != '\0'; ++p) {
            if (*p == target) {
                ++count;
            }
        }
        if ((count >= min) && (count <= max)) {
            ++valid;
        }
    }

    printf("Solution: %d\n", valid);
}

void day_two_b_solution(FILE* fp) {
    int valid = 0;

    int fpos = 0;
    int spos = 0;
    char target = '\0';
    char password[100] = {0};

    while (fscanf(fp, "%d-%d %c: %s\n", &fpos, &spos, &target, password) !=
           EOF) {
        bool first_found = false;
        bool second_found = false;

        // zero index the locations
        fpos -= 1;
        spos -= 1;

        int len = strlen(password);
        if (fpos < len) {
            first_found = password[fpos] == target;
        }
        if (spos < len) {
            second_found = password[spos] == target;
        }
        valid += first_found ^ second_found;
    }

    printf("Solution: %d\n", valid);
}