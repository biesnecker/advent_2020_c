#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#define NUM_STACK_CAP 50
#define OP_STACK_CAP 50

void day_eighteen_solution(FILE *fp)
{
    char buffer[200] = {0};

    long num_stack[NUM_STACK_CAP] = {0};
    int num_stack_size = 0;

    char op_stack[OP_STACK_CAP] = {0};
    int op_stack_size = 0;

    long total = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        long last_num = 0;
        char last_op = 0;
        num_stack_size = 0;
        op_stack_size = 0;

        char *p = buffer;
        while (*p && *p != '\n')
        {
            char c = *p++;
            if (isdigit(c))
            {
                long this = c - '0';
                if (last_num == 0)
                {
                    // Save the current number.
                    last_num = this;
                }
                else
                {
                    assert(last_op != 0);
                    long res = last_op == '+' ? (last_num + this) : (last_num * this);
                    last_num = res;
                    last_op = 0;
                }
            }
            else if (c == '+' || c == '*')
            {
                assert(last_num != 0);
                assert(last_op == 0);
                last_op = c;
            }
            else if (c == '(')
            {
                assert(num_stack_size + 1 < NUM_STACK_CAP);
                assert(op_stack_size + 1 < OP_STACK_CAP);
                if (last_num == 0 && last_op == 0)
                {
                    // This either a nested parens start or at the beginning of the line.
                    num_stack[num_stack_size++] = 1;
                    op_stack[op_stack_size++] = '*';
                }
                else
                {
                    num_stack[num_stack_size++] = last_num;
                    op_stack[op_stack_size++] = last_op;
                    last_num = 0;
                    last_op = 0;
                }
            }
            else if (c == ')')
            {
                assert(last_num != 0);
                assert(((void)last_op, last_op == 0));
                assert(num_stack_size > 0);
                assert(op_stack_size > 0);
                long stack_num = num_stack[--num_stack_size];
                char stack_op = op_stack[--op_stack_size];
                long res = stack_op == '+' ? (last_num + stack_num) : (last_num * stack_num);
                last_num = res;
                last_op = 0;
            }
        }
        assert(last_num != 0);
        assert(last_op == 0);
        assert(num_stack_size == 0);
        assert(op_stack_size == 0);
        total += last_num;
    }
    printf("Solution: %ld\n", total);
}

void day_eighteen_b_solution(FILE *fp)
{
    (void)(fp);
}