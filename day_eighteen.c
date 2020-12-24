#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#define NUM_STACK_CAP 50
#define OP_STACK_CAP 50

#define POP(s) (s[--s##_size])
#define PUSH(s, v) (s)[s##_size++] = (v)
#define PEEK(s) (s[s##_size - 1])

void day_eighteen_solution(FILE* fp) {
    char buffer[200] = {0};

    long num_stack[NUM_STACK_CAP] = {0};
    int num_stack_size = 0;

    char op_stack[OP_STACK_CAP] = {0};
    int op_stack_size = 0;

    long total = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        num_stack_size = 0;
        op_stack_size = 0;
        PUSH(num_stack, 0);
        PUSH(op_stack, '+');

        char* p = buffer;
        while (*p && *p != '\n') {
            char c = *p++;
            if (isdigit(c)) {
                assert(num_stack_size > 0);
                assert(op_stack_size > 0);

                long this = c - '0';
                long last_num = POP(num_stack);
                char last_op = POP(op_stack);
                PUSH(num_stack,
                     last_op == '+' ? (last_num + this) : (last_num * this));
            } else if (c == '+' || c == '*') {
                assert(op_stack_size + 1 < OP_STACK_CAP);
                PUSH(op_stack, c);
            } else if (c == '(') {
                // Add this dummy here so that we have something at the top of
                // the stack to operate with when we hit the first number.
                PUSH(num_stack, 0);
                PUSH(op_stack, '+');
            } else if (c == ')') {
                assert(num_stack_size > 1);
                assert(op_stack_size > 0);

                long paren_num = POP(num_stack);
                long stack_num = POP(num_stack);
                char stack_op = POP(op_stack);
                PUSH(num_stack,
                     stack_op == '+' ? (paren_num + stack_num)
                                     : (paren_num * stack_num));
            }
        }
        assert(num_stack_size == 1);
        assert(op_stack_size == 0);
        total += num_stack[0];
    }
    printf("Solution: %ld\n", total);
}

void day_eighteen_b_solution(FILE* fp) {
    char buffer[200] = {0};

    long num_stack[NUM_STACK_CAP] = {0};
    int num_stack_size = 0;

    char op_stack[OP_STACK_CAP] = {0};
    int op_stack_size = 0;

    long total = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        num_stack_size = 0;
        op_stack_size = 0;
        PUSH(num_stack, 1);
        PUSH(op_stack, '*');

        char* p = buffer;
        while (*p && *p != '\n') {
            char c = *p++;
            if (c == ' ') {
                continue;
            }

            if (isdigit(c)) {
                assert(num_stack_size > 0);
                assert(op_stack_size > 0);

                long this = c - '0';
                if (PEEK(op_stack) == '*') {
                    // If the last op was multiply, push this on the stack too.
                    PUSH(num_stack, this);
                } else {
                    // If the last op was addition, do it now.
                    (void)POP(op_stack);
                    long last_num = POP(num_stack);
                    PUSH(num_stack, last_num + this);
                }
            } else if (c == '+' || c == '*') {
                assert(op_stack_size + 1 < OP_STACK_CAP);
                PUSH(op_stack, c);
            } else if (c == '(') {
                // Add this dummy here so that we have something at the top of
                // the stack to operate with when we hit the first number.
                PUSH(op_stack, '!'); // Sentinal to indicate parens start.
                PUSH(num_stack, 1);
                PUSH(op_stack, '*');
            } else if (c == ')') {
                assert(num_stack_size > 1);
                assert(op_stack_size > 0);

                while (PEEK(op_stack) != '!') {
                    // There should only be multiplications
                    assert(POP(op_stack) == '*');
                    long paren_num = POP(num_stack);
                    long stack_num = POP(num_stack);
                    PUSH(num_stack, paren_num * stack_num);
                }

                // Remove the sentinal.
                assert(POP(op_stack) == '!');

                // If this parens was the right side of addition, add now.
                if (op_stack_size > 0 && PEEK(op_stack) == '+') {
                    assert(num_stack_size > 1);
                    assert(op_stack_size > 0);
                    assert(POP(op_stack) == '+');
                    long n1 = POP(num_stack);
                    long n2 = POP(num_stack);
                    PUSH(num_stack, n1 + n2);
                }
            }
        }
        // We should be left with nothing but multiplications.
        for (int i = 0; i < op_stack_size; ++i) {
            assert(op_stack[i] == '*');
        }
        long product = 1;
        for (int i = 0; i < num_stack_size; ++i) {
            product *= num_stack[i];
        }
        total += product;
    }
    printf("Solution: %ld\n", total);
}