#include <stdio.h>
#include <string.h>

#include "bitvector.h"

#define MAX_PROG_LENGTH 700

typedef enum _instr_type
{
    NOP = 0,
    ACC = 1,
    JMP = 2
} instr_type;

typedef struct _instr
{
    instr_type tp;
    int arg;
} instr;

static int read_input(FILE *fp, instr *program)
{
    (void)(program);
    char buffer[50] = {0};
    int arg = 0;
    int idx = 0;
    while (fscanf(fp, "%s %d\n", buffer, &arg) != EOF)
    {
        instr_type t = NOP;
        if (strcmp(buffer, "acc") == 0)
        {
            t = ACC;
        }
        else if (strcmp(buffer, "jmp") == 0)
        {
            t = JMP;
        }
        program[idx].tp = t;
        program[idx].arg = arg;
        ++idx;
    }
    return idx;
}

void day_eight_solution(FILE *fp)
{
    instr prog[MAX_PROG_LENGTH];
    read_input(fp, prog);

    int pc = 0;
    long acc = 0;
    bitvector *seen = bitvector_new(MAX_PROG_LENGTH);
    while (true)
    {
        if (bitvector_check(seen, pc))
        {
            printf("Solution: %ld\n", acc);
            break;
        }
        bitvector_set(seen, pc);
        switch (prog[pc].tp)
        {
        case NOP:
            pc += 1;
            break;
        case ACC:
            acc += prog[pc++].arg;
            break;
        case JMP:
            pc += prog[pc].arg;
            break;
        }
    }
    bitvector_free(seen);
}

void day_eight_b_solution(FILE *fp)
{
    instr prog[MAX_PROG_LENGTH];
    int len = read_input(fp, prog);

    bitvector *seen = bitvector_new(MAX_PROG_LENGTH);
    bitvector *tried = bitvector_new(MAX_PROG_LENGTH);

    while (true)
    {
        int pc = 0;
        long acc = 0;
        bool switched = false;
        while (true)
        {
            if (bitvector_check(seen, pc))
            {
                break;
            }
            bitvector_set(seen, pc);
            instr_type t = prog[pc].tp;
            if ((t == JMP || t == NOP) && !switched && !bitvector_check(tried, pc))
            {
                t = t == JMP ? NOP : JMP;
                bitvector_set(tried, pc);
                switched = true;
            }
            switch (t)
            {
            case NOP:
                pc += 1;
                break;
            case ACC:
                acc += prog[pc++].arg;
                break;
            case JMP:
                pc += prog[pc].arg;
                break;
            }
            if (pc == len)
            {
                printf("Solution: %ld\n", acc);
                goto done;
            }
        }
        bitvector_clear(seen);
    }
done:
    bitvector_free(seen);
    bitvector_free(tried);
}