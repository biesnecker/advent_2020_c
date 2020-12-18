#include <stdio.h>
#include <stdlib.h>

#define MAX_COMMANDS 775

typedef struct _command
{
    char cmd;
    int arg;
} command;

typedef struct _ship
{
    long x;
    long y;
    long wp_x;
    long wp_y;
    int direction;
} ship;

static void init_ship(ship *ship)
{
    ship->x = 0;
    ship->y = 0;
    ship->wp_x = 10;
    ship->wp_y = 1;
    ship->direction = 0;
}

static void move(char direction, int amount, long *x, long *y)
{
    switch (direction)
    {
    case 'N':
        *y += amount;
        break;
    case 'S':
        *y -= amount;
        break;
    case 'E':
        *x += amount;
        break;
    case 'W':
        *x -= amount;
    }
}

static void move_ship(ship *ship, const command *command)
{
    move(command->cmd, command->arg, &ship->x, &ship->y);
}

static void move_waypoint(ship *ship, const command *command)
{
    move(command->cmd, command->arg, &ship->wp_x, &ship->wp_y);
}

static void rotate_ship(ship *ship, const command *command)
{
    int degrees = command->arg;
    if (command->cmd == 'L')
    {
        degrees = 360 - degrees;
    }
    int turns = degrees / 90;
    ship->direction = (ship->direction + turns) % 4;
}

static void rotate_waypoint(ship *ship, const command *command)
{
    int degrees = command->arg;
    if (command->cmd == 'L')
    {
        degrees = 360 - degrees;
    }
    int turns = degrees / 90;
    long temp = 0;
    for (int i = 0; i < turns; ++i)
    {
        temp = ship->wp_x;
        ship->wp_x = ship->wp_y;
        ship->wp_y = temp;
        ship->wp_y *= -1;
    }
}

static void move_ship_forward(ship *ship, const command *command)
{
    const int x_offsets[4] = {1, 0, -1, 0};
    const int y_offsets[4] = {0, -1, 0, 1};
    ship->x += x_offsets[ship->direction] * command->arg;
    ship->y += y_offsets[ship->direction] * command->arg;
}

static void move_ship_to_waypoint(ship *ship, const command *command)
{
    ship->x += ship->wp_x * command->arg;
    ship->y += ship->wp_y * command->arg;
}

static int read_input(FILE *fp, command *command_list)
{
    char buffer[10] = {0};
    int idx = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        char *argp = buffer + 1;
        int arg = atoi(argp);
        command_list[idx].cmd = buffer[0];
        command_list[idx].arg = arg;
        ++idx;
    }
    return idx;
}

static long distance(const ship *ship)
{
    return labs(ship->x) + labs(ship->y);
}

void day_twelve_solution(FILE *fp)
{
    command command_list[MAX_COMMANDS] = {0};
    int count = read_input(fp, command_list);

    ship minnow;
    init_ship(&minnow);

    for (int i = 0; i < count; ++i)
    {
        switch (command_list[i].cmd)
        {
        case 'N':
        case 'S':
        case 'E':
        case 'W':
            move_ship(&minnow, &command_list[i]);
            break;
        case 'L':
        case 'R':
            rotate_ship(&minnow, &command_list[i]);
            break;
        case 'F':
            move_ship_forward(&minnow, &command_list[i]);
            break;
        default:
            break;
        }
    }

    printf("Solution: %ld\n", distance(&minnow));
}

void day_twelve_b_solution(FILE *fp)
{
    command command_list[MAX_COMMANDS] = {0};
    int count = read_input(fp, command_list);

    ship minnow;
    init_ship(&minnow);

    for (int i = 0; i < count; ++i)
    {
        switch (command_list[i].cmd)
        {
        case 'N':
        case 'S':
        case 'E':
        case 'W':
            move_waypoint(&minnow, &command_list[i]);
            break;
        case 'L':
        case 'R':
            rotate_waypoint(&minnow, &command_list[i]);
            break;
        case 'F':
            move_ship_to_waypoint(&minnow, &command_list[i]);
            break;
        default:
            break;
        }
    }

    printf("Solution: %ld\n", distance(&minnow));
}