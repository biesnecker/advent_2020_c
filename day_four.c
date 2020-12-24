#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _parse_state {
    bool has_valid_byr;
    bool has_valid_iyr;
    bool has_valid_eyr;
    bool has_valid_hgt;
    bool has_valid_hcl;
    bool has_valid_ecl;
    bool has_valid_pid;
    bool has_valid_cid;
} parse_state;

static bool is_valid_state(parse_state* state) {
    if (state == NULL) {
        return false;
    }
    return state->has_valid_byr && state->has_valid_iyr &&
           state->has_valid_eyr && state->has_valid_hgt &&
           state->has_valid_hcl && state->has_valid_ecl && state->has_valid_pid;
}

static bool is_valid_field(const char* prefix, const char* s) {
    if (strlen(s) < 5 || strlen(prefix) != 3) {
        return false;
    }
    return prefix[0] == s[0] && prefix[1] == s[1] && prefix[2] == s[2] &&
           s[3] == ':';
}

static bool is_valid_year_field(const char* s,
                                int len,
                                const char* prefix,
                                int min_year,
                                int max_year) {
    if (!is_valid_field(prefix, s)) {
        return false;
    }
    if (len != 8) {
        return false;
    }
    const char* year_part = s + 4;
    int year = atoi(year_part);
    return year >= min_year && year <= max_year;
}

static bool is_valid_byr_field(const char* s, int len) {
    return is_valid_year_field(s, len, "byr", 1920, 2002);
}

static bool is_valid_iyr_field(const char* s, int len) {
    return is_valid_year_field(s, len, "iyr", 2010, 2020);
}

static bool is_valid_eyr_field(const char* s, int len) {
    return is_valid_year_field(s, len, "eyr", 2020, 2030);
}

static bool is_valid_hgt_field(const char* s, int len) {
    if (!is_valid_field("hgt", s)) {
        return false;
    }
    if (len != 8 && len != 9) {
        return false;
    }
    const char* suffix = s + (len - 2);
    const char* num_part = s + 4;
    if (strcmp(suffix, "in") == 0 && len == 8) {
        int n = atoi(num_part);
        return n >= 59 && n <= 76;
    } else if (strcmp(suffix, "cm") == 0 && len == 9) {
        int n = atoi(num_part);
        return n >= 150 && n <= 193;
    } else {
        return false;
    }
}

static bool is_valid_hcl_field(const char* s, int len) {
    if (!is_valid_field("hcl", s)) {
        return false;
    }
    if (len != 11) {
        return false;
    }
    const char* color_part = s + 4;
    if (color_part[0] != '#') {
        return false;
    }
    for (int i = 1; i < 7; ++i) {
        if (!isxdigit(color_part[i])) {
            return false;
        }
    }
    return true;
}

static bool is_valid_ecl_field(const char* s, int len) {
    if (!is_valid_field("ecl", s)) {
        return false;
    }
    if (len != 7) {
        return false;
    }
    const char* color_part = s + 4;
    if (strcmp("amb", color_part) == 0 || strcmp("blu", color_part) == 0 ||
        strcmp("brn", color_part) == 0 || strcmp("gry", color_part) == 0 ||
        strcmp("grn", color_part) == 0 || strcmp("hzl", color_part) == 0 ||
        strcmp("oth", color_part) == 0) {
        return true;
    }
    return false;
}

static bool is_valid_pid_field(const char* s, int len) {
    if (!is_valid_field("pid", s)) {
        return false;
    }
    if (len != 13) {
        return false;
    }
    const char* numeric_part = s + 4;
    for (int i = 0; i < 9; ++i) {
        if (!isdigit(numeric_part[i])) {
            printf("Invalid pid: %d %d %s\n", i, len, numeric_part);
            printf("###%s####\n", s);
            return false;
        }
    }
    return true;
}

void day_four_solution(FILE* fp) {
    char buffer[512] = {0};
    parse_state* state = NULL;
    int valid = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strlen(buffer) == 1) {
            if (is_valid_state(state)) {
                ++valid;
            }
            free(state);
            state = NULL;
        }

        // Create a new state if needed.
        if (state == NULL) {
            state = calloc(1, sizeof(parse_state));
        }

        char* found = strtok(buffer, " ");
        while (found) {
            if (!state->has_valid_byr && is_valid_field("byr", found)) {
                state->has_valid_byr = true;
            } else if (!state->has_valid_iyr && is_valid_field("iyr", found)) {
                state->has_valid_iyr = true;
            } else if (!state->has_valid_eyr && is_valid_field("eyr", found)) {
                state->has_valid_eyr = true;
            } else if (!state->has_valid_hgt && is_valid_field("hgt", found)) {
                state->has_valid_hgt = true;
            } else if (!state->has_valid_hcl && is_valid_field("hcl", found)) {
                state->has_valid_hcl = true;
            } else if (!state->has_valid_ecl && is_valid_field("ecl", found)) {
                state->has_valid_ecl = true;
            } else if (!state->has_valid_pid && is_valid_field("pid", found)) {
                state->has_valid_pid = true;
            } else if (!state->has_valid_cid && is_valid_field("cid", found)) {
                state->has_valid_cid = true;
            }

            // Get the next token.
            found = strtok(NULL, " ");
        }
    }

    // Make sure if we have a state at the end of the loop that we check it.
    if (state != NULL) {
        if (is_valid_state(state)) {
            ++valid;
        }
        free(state);
    }

    printf("Solution: %d\n", valid);
}

void day_four_b_solution(FILE* fp) {
    char buffer[512] = {0};
    parse_state* state = NULL;
    int valid = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strlen(buffer) == 1) {
            if (is_valid_state(state)) {
                ++valid;
            }
            free(state);
            state = NULL;
        }

        // Create a new state if needed.
        if (state == NULL) {
            state = calloc(1, sizeof(parse_state));
        }

        char* found = strtok(buffer, " ");
        while (found) {
            int found_len = strlen(found);
            if (found[found_len - 1] == '\n') {
                found[found_len - 1] = '\0';
                found_len -= 1;
            }
            if (!state->has_valid_byr && is_valid_byr_field(found, found_len)) {
                state->has_valid_byr = true;
            } else if (!state->has_valid_iyr &&
                       is_valid_iyr_field(found, found_len)) {
                state->has_valid_iyr = true;
            } else if (!state->has_valid_eyr &&
                       is_valid_eyr_field(found, found_len)) {
                state->has_valid_eyr = true;
            } else if (!state->has_valid_hgt &&
                       is_valid_hgt_field(found, found_len)) {
                state->has_valid_hgt = true;
            } else if (!state->has_valid_hcl &&
                       is_valid_hcl_field(found, found_len)) {
                state->has_valid_hcl = true;
            } else if (!state->has_valid_ecl &&
                       is_valid_ecl_field(found, found_len)) {
                state->has_valid_ecl = true;
            } else if (!state->has_valid_pid &&
                       is_valid_pid_field(found, found_len)) {
                state->has_valid_pid = true;
            }

            // Get the next token.
            found = strtok(NULL, " ");
        }
    }

    // Make sure if we have a state at the end of the loop that we check it.
    if (state != NULL) {
        if (is_valid_state(state)) {
            ++valid;
        }
        free(state);
    }

    printf("Solution: %d\n", valid);
}