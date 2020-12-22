#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define STARTING_WIDTH 8
#define STARTING_HEIGHT 8
#define STARTING_DEPTH 1
#define STARTING_TIME 1

#define N_ROUNDS 6

#define MAX_WIDTH (STARTING_WIDTH + (N_ROUNDS * 2) + 2)
#define MAX_HEIGHT (STARTING_HEIGHT + (N_ROUNDS * 2) + 2)
#define MAX_DEPTH (STARTING_DEPTH + (N_ROUNDS * 2) + 2)
#define MAX_TIME (STARTING_TIME + (N_ROUNDS * 2) + 2)

#define N_CELLS_3D (MAX_WIDTH * MAX_HEIGHT * MAX_DEPTH)
#define N_CELLS_4D (MAX_WIDTH * MAX_HEIGHT * MAX_DEPTH * MAX_TIME)

#define LOOP_3D(w, h, d, ...)                       \
    for (int w = 0; (w) < MAX_WIDTH; ++(w))         \
    {                                               \
        for (int h = 0; (h) < MAX_HEIGHT; ++(h))    \
        {                                           \
            for (int d = 0; (d) < MAX_DEPTH; ++(d)) \
            {                                       \
                __VA_ARGS__                         \
            }                                       \
        }                                           \
    }

#define LOOP_4D(w, h, d, t, ...)               \
    LOOP_3D((w), (h), (d), {                   \
        for (int t = 0; (t) < MAX_TIME; ++(t)) \
        {                                      \
            __VA_ARGS__                        \
        }                                      \
    })

typedef uint8_t neighbor_grid_3d[MAX_WIDTH][MAX_HEIGHT][MAX_DEPTH];
typedef bool active_grid_3d[MAX_WIDTH][MAX_HEIGHT][MAX_DEPTH];

typedef uint8_t neighbor_grid_4d[MAX_WIDTH][MAX_HEIGHT][MAX_DEPTH][MAX_TIME];
typedef bool active_grid_4d[MAX_WIDTH][MAX_HEIGHT][MAX_DEPTH][MAX_TIME];

static void update_neighborhood_3d(neighbor_grid_3d neighbors, int w, int h, int d)
{
    for (int x = w - 1; x < w + 2; ++x)
    {
        for (int y = h - 1; y < h + 2; ++y)
        {
            for (int z = d - 1; z < d + 2; ++z)
            {
                if (x == w && y == h && z == d)
                {
                    // Don't actually update the current cell.
                    continue;
                }
                neighbors[x][y][z] += 1;
            }
        }
    }
}

static void update_neighborhood_4d(neighbor_grid_4d neighbors, int w, int h, int d, int t)
{
    for (int x = w - 1; x < w + 2; ++x)
    {
        for (int y = h - 1; y < h + 2; ++y)
        {
            for (int z = d - 1; z < d + 2; ++z)
            {
                for (int a = t - 1; a < t + 2; ++a)
                {
                    if (x == w && y == h && z == d && a == t)
                    {
                        // Don't actually update the current cell.
                        continue;
                    }
                    neighbors[x][y][z][a] += 1;
                }
            }
        }
    }
}

static void update_neighbors_3d(active_grid_3d active, neighbor_grid_3d neighbors)
{
    memset(neighbors, 0, N_CELLS_3D * sizeof(uint8_t));
    LOOP_3D(w, h, d,
            {
                if (active[w][h][d])
                {
                    update_neighborhood_3d(neighbors, w, h, d);
                }
            });
}

static void update_neighbors_4d(active_grid_4d active, neighbor_grid_4d neighbors)
{
    memset(neighbors, 0, N_CELLS_4D * sizeof(uint8_t));
    LOOP_4D(w, h, d, t,
            {
                if (active[w][h][d][t])
                {
                    update_neighborhood_4d(neighbors, w, h, d, t);
                }
            });
}

static void step_3d(active_grid_3d current, active_grid_3d previous, neighbor_grid_3d neighbors)
{
    int active_neighbors = 0;
    update_neighbors_3d(previous, neighbors);
    LOOP_3D(w, h, d,
            {
                active_neighbors = neighbors[w][h][d];
                if (previous[w][h][d])
                {
                    // was active
                    if (active_neighbors == 2 || active_neighbors == 3)
                    {
                        // stays active
                        current[w][h][d] = true;
                    }
                    else
                    {
                        // goes inactive
                        current[w][h][d] = false;
                    }
                }
                else if (active_neighbors == 3)
                {
                    // was inactive, goes active
                    current[w][h][d] = true;
                }
                else
                {
                    // was inactive, stays inactive
                    current[w][h][d] = false;
                }
            });
}

static void step_4d(active_grid_4d current, active_grid_4d previous, neighbor_grid_4d neighbors)
{
    int active_neighbors = 0;
    update_neighbors_4d(previous, neighbors);
    LOOP_4D(
        w, h, d, t, {
            active_neighbors = neighbors[w][h][d][t];
            if (previous[w][h][d][t])
            {
                // was active
                if (active_neighbors == 2 || active_neighbors == 3)
                {
                    // stays active
                    current[w][h][d][t] = true;
                }
                else
                {
                    // goes inactive
                    current[w][h][d][t] = false;
                }
            }
            else if (active_neighbors == 3)
            {
                // was inactive, goes active
                current[w][h][d][t] = true;
            }
            else
            {
                // was inactive, stays inactive
                current[w][h][d][t] = false;
            }
        });
}

static int count_active_3d(active_grid_3d current)
{
    int total = 0;
    LOOP_3D(w, h, d, {
        if (current[w][h][d])
        {
            ++total;
        }
    });
    return total;
}

static int count_active_4d(active_grid_4d current)
{
    int total = 0;
    LOOP_4D(w, h, d, t, {
        if (current[w][h][d][t])
        {
            ++total;
        }
    });
    return total;
}

void day_seventeen_solution(FILE *fp)
{
    neighbor_grid_3d neighbors = {0};

    active_grid_3d active_a = {0};
    active_grid_3d active_b = {0};

    char buffer[20] = {0};
    int y = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        for (int x = 0; x < STARTING_WIDTH; ++x)
        {
            // Place the active cells in the middle of the space.
            if (buffer[x] == '#')
            {
                active_b[x + N_ROUNDS + 1][y + N_ROUNDS + 1][N_ROUNDS + 1] = true;
            }
        }
        ++y;
    }

    bool tock = true;

    for (int round = 0; round < N_ROUNDS; ++round)
    {
        if (tock)
        {
            step_3d(active_a, active_b, neighbors);
        }
        else
        {
            step_3d(active_b, active_a, neighbors);
        }
        tock = !tock;
    }
    int solution = count_active_3d(tock ? active_b : active_a);
    printf("Solution: %d\n", solution);
}

void day_seventeen_b_solution(FILE *fp)
{
    neighbor_grid_4d neighbors = {0};

    active_grid_4d active_a = {0};
    active_grid_4d active_b = {0};

    char buffer[20] = {0};
    int y = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        for (int x = 0; x < STARTING_WIDTH; ++x)
        {
            // Place the active cells in the middle of the space.
            if (buffer[x] == '#')
            {
                active_b[x + N_ROUNDS + 1][y + N_ROUNDS + 1][N_ROUNDS + 1][N_ROUNDS + 1] = true;
            }
        }
        ++y;
    }

    bool tock = true;

    for (int round = 0; round < N_ROUNDS; ++round)
    {
        if (tock)
        {
            step_4d(active_a, active_b, neighbors);
        }
        else
        {
            step_4d(active_b, active_a, neighbors);
        }
        tock = !tock;
    }
    int solution = count_active_4d(tock ? active_b : active_a);
    printf("Solution: %d\n", solution);
}