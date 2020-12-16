#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "fnv.h"

#define HASH_TABLE_EXP 11
#define HASH_TABLE_SIZE (1L << HASH_TABLE_EXP)

struct _node;

typedef struct _edge
{
    struct _node *target;
    int count;
    bool incoming;
} edge;

typedef struct _node
{
    char *label;
    edge *edges;
    int edge_capacity;
    int edge_size;
    bool seen;
} node;

static const char half_split_delimiter[] = " contain";

static void edge_new(edge *e, node *target, int count, bool incoming)
{
    e->target = target;
    e->count = count;
    e->incoming = incoming;
}

static void node_new(node *n, char *label)
{
    int len = strlen(label);
    n->label = calloc((len + 1), sizeof(char));
    strncpy(n->label, label, len);
    n->edge_capacity = 10;
    n->edge_size = 0;
    n->edges = calloc(n->edge_capacity, sizeof(edge));
    n->seen = false;
}

static void node_free(node *n)
{
    free(n->label);
    free(n->edges);
}

static void node_add_edge(node *n, node *target, int count, bool incoming)
{
    // First check if the edge already exists. This shouldn't happen.
    for (int i = 0; i < n->edge_size; ++i)
    {
        if (n->edges[i].target == target && n->edges[i].incoming == incoming)
        {
            return;
        }
    }
    if (n->edge_size == n->edge_capacity)
    {
        // Not checking for NULL because yolo.
        n->edges = realloc(n->edges, n->edge_capacity * sizeof(edge) * 2);
        n->edge_capacity *= 2;
    }
    edge_new(&n->edges[n->edge_size], target, count, incoming);
    n->edge_size += 1;
}

static uint32_t hash_string(const char *s)
{
    uint32_t hsh = fnv32_char(s);
    return (hsh >> (32 - HASH_TABLE_EXP)) ^ (hsh & (HASH_TABLE_SIZE - 1));
}

static node *find_or_insert_node(node *hashes, char *label)
{
    uint32_t hash = hash_string(label);
    while (true)
    {
        if (hashes[hash].label == NULL)
        {
            // Add the new value.;
            node_new(&hashes[hash], label);
            return &hashes[hash];
        }
        else if (strcmp(hashes[hash].label, label) == 0)
        {
            // Return the existing value.
            return &hashes[hash];
        }
        else
        {
            hash = (hash + 1) % HASH_TABLE_SIZE;
        }
    }
}

static void remove_bag_suffix(char *s)
{
    char *start = strstr(s, " bag");
    s[start - s] = '\0';
}

static void process_line(node *hashes, char *line)
{
    char *middle_split = strstr(line, half_split_delimiter);
    line[middle_split - line] = '\0';
    remove_bag_suffix(line);

    node *container_node = find_or_insert_node(hashes, line);
    assert(container_node != NULL);

    char *rest = middle_split + sizeof(half_split_delimiter) - 1;
    if (strstr(rest, "no other"))
    {
        return;
    }
    char *chunk = strtok(rest, ",");
    char *space = NULL;
    while (chunk)
    {
        ++chunk;
        remove_bag_suffix(chunk);

        // Split the chunk into a number and a color
        space = strchr(chunk, ' ');
        chunk[space - chunk] = '\0';
        int n = atoi(chunk);
        char *color = space + 1;
        // Add the child nodes here.
        node *child_node = find_or_insert_node(hashes, color);
        assert(child_node != NULL);
        node_add_edge(container_node, child_node, n, true);
        node_add_edge(child_node, container_node, n, false);

        chunk = strtok(NULL, ",");
    }
}

static void create_graph(FILE *fp, node *hashes)
{
    char buffer[256] = {0};
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        process_line(hashes, buffer);
    }
}

void free_nodes(node *hashes)
{
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        node_free(&hashes[i]);
    }
}

void day_seven_solution(FILE *fp)
{
    node hashes[HASH_TABLE_SIZE] = {0};
    create_graph(fp, hashes);

    node *stack[500] = {0};

    node *current = NULL;
    int stack_size = 1;
    stack[0] = find_or_insert_node(hashes, "shiny gold");

    int count = -1;

    while (stack_size > 0)
    {
        stack_size -= 1;
        current = stack[stack_size];
        ++count;

        for (int i = 0; i < current->edge_size; i++)
        {
            if (current->edges[i].incoming || current->edges[i].target->seen)
            {
                // We only want outgoing, unseen.
                continue;
            }
            stack[stack_size++] = current->edges[i].target;
            current->edges[i].target->seen = true;
        }
    }

    printf("Solution: %d\n", count);

    free_nodes(hashes);
}

static long bag_size(node *bag)
{
    long total = 1;
    for (int i = 0; i < bag->edge_size; i++)
    {
        if (!bag->edges[i].incoming)
        {
            // We only want incoming edges.
            continue;
        }
        int child_size = bag->edges[i].count * bag_size(bag->edges[i].target);
        total += child_size;
    }
    return total;
}

void day_seven_b_solution(FILE *fp)
{
    node hashes[HASH_TABLE_SIZE] = {0};
    create_graph(fp, hashes);

    node *current = find_or_insert_node(hashes, "shiny gold");

    int count = bag_size(current) - 1;

    printf("Solution: %d\n", count);

    free_nodes(hashes);
}