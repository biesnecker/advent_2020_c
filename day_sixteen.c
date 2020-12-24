#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_RULES 20
#define N_TICKETS 250

#define SET_BIT(bits, idx) ((bits) | (1UL << (idx)))
#define CHK_BIT(bits, idx) (((bits) & (1UL << (idx))) != 0)

#define MEETS_RULE(rule, value)                                                \
    (((value) >= (rule).fst_lo && (value) <= (rule).fst_hi) ||                 \
     ((value) >= (rule).snd_lo && (value) <= (rule).snd_hi))

typedef struct _rule {
    int fst_lo;
    int fst_hi;
    int snd_lo;
    int snd_hi;
    uint32_t fields;
    bool is_departure;
} rule;

typedef struct _ticket {
    int fields[20];
} ticket;

static int parse_ticket(ticket* t, const char* buffer) {
    int idx = 0;
    const char* next = buffer - 1;
    while (next) {
        next += 1;
        t->fields[idx] = atoi(next);
        next = strchr(next, ',');
        idx += 1;
    }
    return idx;
}

static void read_input(FILE* fp,
                       rule* rules,
                       int* rules_len,
                       ticket* yourticket,
                       int* fields_len,
                       ticket* nearby_tickets,
                       int* tickets_len) {
    (void)(yourticket);
    (void)(nearby_tickets);
    char buffer[100] = {0};
    int rule_idx = 0;
    int ticket_idx = 0;

    // 0 = rules, 1 = your ticket, 2 = nearby tickets
    int mode = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (mode == 0) {
            if (buffer[0] == '\n') {
                ++mode;
                continue;
            }
            rules[rule_idx].is_departure =
                strstr(buffer, "departure") == buffer;
            rules[rule_idx].fields = 0xFFFFF;
            char* range_start = strchr(buffer, ':') + 2;
            sscanf(range_start,
                   "%d-%d or %d-%d",
                   &rules[rule_idx].fst_lo,
                   &rules[rule_idx].fst_hi,
                   &rules[rule_idx].snd_lo,
                   &rules[rule_idx].snd_hi);
            rule_idx += 1;
        } else if (mode == 1) {
            if (strcmp(buffer, "your ticket:\n") == 0) {
                // Skip this line.
                continue;
            }
            *fields_len = parse_ticket(yourticket, buffer);
            ++mode;
        } else if (mode == 2) {
            if (buffer[0] == '\n') {
                continue;
            } else if (strcmp(buffer, "nearby tickets:\n") == 0) {
                continue;
            }
            parse_ticket(&nearby_tickets[ticket_idx], buffer);
            ++ticket_idx;
        }
    }
    *rules_len = rule_idx;
    *tickets_len = ticket_idx;
}

static bool has_invalid_fields(const ticket* t,
                               int fields_len,
                               const rule* rules,
                               int rules_len,
                               int* sum) {
    int total = 0;
    int v = 0;
    bool failed = true;
    bool has_invalid_fields = false;
    for (int i = 0; i < fields_len; ++i) {
        v = t->fields[i];
        failed = true;
        for (int j = 0; j < rules_len; ++j) {
            if (MEETS_RULE(rules[j], v)) {
                failed = false;
                break;
            }
        }
        if (failed) {
            total += v;
            has_invalid_fields = true;
        }
    }
    if (sum != NULL) {
        *sum += total;
    }
    return has_invalid_fields;
}

void day_sixteen_solution(FILE* fp) {
    rule rules[N_RULES] = {0};
    ticket yourticket = {0};
    ticket tickets[N_TICKETS] = {0};
    int rules_len, fields_len, tickets_len;
    read_input(
        fp, rules, &rules_len, &yourticket, &fields_len, tickets, &tickets_len);

    int invalid_sum = 0;

    for (int i = 0; i < tickets_len; ++i) {
        has_invalid_fields(
            &tickets[i], fields_len, rules, rules_len, &invalid_sum);
    }
    printf("Solution: %d\n", invalid_sum);
}

void day_sixteen_b_solution(FILE* fp) {
    rule rules[N_RULES] = {0};
    ticket yourticket = {0};
    ticket tickets[N_TICKETS] = {0};
    int rules_len, fields_len, tickets_len;
    read_input(
        fp, rules, &rules_len, &yourticket, &fields_len, tickets, &tickets_len);

    long field_sums[N_RULES] = {0};

    for (int ticket_id = 0; ticket_id < tickets_len; ++ticket_id) {
        ticket* t = &tickets[ticket_id];
        if (has_invalid_fields(t, fields_len, rules, rules_len, NULL)) {
            // Skip invalid tickets.
            continue;
        }
        for (int field_id = 0; field_id < fields_len; ++field_id) {
            int v = t->fields[field_id];
            field_sums[field_id] += v;
            for (int rule_id = 0; rule_id < rules_len; ++rule_id) {
                if (MEETS_RULE(rules[rule_id], v)) {
                    continue;
                }
                // This field is not eligible for this rule.
                rules[rule_id].fields &= ~SET_BIT(0UL, field_id);
            }
        }
    }

    long long product = 1;
    bool finished = false;

    while (!finished) {
        finished = true;

        for (int rule_id = 0; rule_id < rules_len; ++rule_id) {
            rule* r = &rules[rule_id];
            if (__builtin_popcount(r->fields) == 1) {
                finished = false;
                // This rule only has one possible field.
                int field_id = __builtin_ctz(r->fields);
                if (r->is_departure) {
                    product *= yourticket.fields[field_id];
                }
                uint32_t mask = ~(r->fields);
                for (int j = 0; j < rules_len; ++j) {
                    // Unset the current value
                    rules[j].fields &= mask;
                }
            }
        }
    }

    printf("Solution: %lld\n", product);
}