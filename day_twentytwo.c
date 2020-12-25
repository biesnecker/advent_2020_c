#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#include "macros.h"

typedef struct card {
    unsigned value;
    STAILQ_ENTRY(card) next;
} card;

typedef struct deck {
    unsigned n_cards;
    STAILQ_HEAD(cards, card) cards;
} deck;

typedef struct game {
    deck* player_one;
    deck* player_two;
    unsigned winner;
    STAILQ_HEAD(subgames, game) subgames;
} game;

static void read_input(FILE* fp, deck* player_one, deck* player_two);

static deck* deck_new() {
    deck* d = malloc(sizeof(deck));
    assert(d);
    d->n_cards = 0;
    STAILQ_INIT(&d->cards);
    return d;
}

static void deck_free(deck* d) {
    if (d == NULL) {
        return;
    }
    card *c, *tmp;
    STAILQ_FOREACH_SAFE(c, &d->cards, next, tmp) {
        free(c);
    }
    free(d);
}

static game* game_new_from_file(FILE* fp) {
    game* g = malloc(sizeof(game));
    assert(g);
    g->player_one = deck_new();
    g->player_two = deck_new();
    read_input(fp, g->player_one, g->player_two);
    STAILQ_INIT(&g->subgames);
    return g;
}

static void game_free(game* g) {
    if (g == NULL) {
        return;
    }
    deck_free(g->player_one);
    deck_free(g->player_two);
}

static void read_input(FILE* fp, deck* player_one, deck* player_two) {
    int mode = 0;
    char buffer[100] = {0};
    deck* current_deck = player_one;

    unsigned num = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (mode == 0 && strcmp("Player 1:\n", buffer) == 0) {
            mode = 1;
        } else if (mode == 1 && buffer[0] == '\n') {
            mode = 2;
            current_deck = player_two;
        } else if (mode == 2 && strcmp("Player 2:\n", buffer) == 0) {
            mode = 3;
        } else {
            num = 0;
            sscanf(buffer, "%u", &num);
            assert(num != 0);
            card* card = calloc(1, sizeof(card));
            assert(card);
            card->value = num;
            STAILQ_INSERT_TAIL(&current_deck->cards, card, next);
            ++current_deck->n_cards;
        }
    }
}

static bool game_over(const game* g) {
    return g->player_one->n_cards == 0 || g->player_two->n_cards == 0;
}

static unsigned play_game(game* g) {
    while (!game_over(g)) {
        card* player_one_card = STAILQ_FIRST(&g->player_one->cards);
        card* player_two_card = STAILQ_FIRST(&g->player_two->cards);

        STAILQ_REMOVE_HEAD(&g->player_one->cards, next);
        g->player_one->n_cards -= 1;
        STAILQ_REMOVE_HEAD(&g->player_two->cards, next);
        g->player_two->n_cards -= 1;

        if (player_one_card->value > player_two_card->value) {
            // Player one wins, add their card first.
            STAILQ_INSERT_TAIL(&g->player_one->cards, player_one_card, next);
            STAILQ_INSERT_TAIL(&g->player_one->cards, player_two_card, next);
            g->player_one->n_cards += 2;
            g->winner = 1;
        } else {
            // Player two wins, add their card first.
            STAILQ_INSERT_TAIL(&g->player_two->cards, player_two_card, next);
            STAILQ_INSERT_TAIL(&g->player_two->cards, player_one_card, next);
            g->player_two->n_cards += 2;
            g->winner = 2;
        }
    }
    return g->winner;
}

unsigned long score_deck(const deck* winner) {
    unsigned long score = 0;
    unsigned mult = winner->n_cards;
    card* c;
    STAILQ_FOREACH(c, &winner->cards, next) {
        score += (c->value * mult--);
    }
    return score;
}

void day_twentytwo_solution(FILE* fp) {
    game* g = game_new_from_file(fp);

    unsigned winner = play_game(g);

    unsigned long score = 0;

    switch (winner) {
    case 0:
        FAIL("There was no winner :-(\n");
    case 1:
        score = score_deck(g->player_one);
        break;
    case 2:
        score = score_deck(g->player_two);
        break;
    }

    printf("Solution: %lu\n", score);

    game_free(g);
}

void day_twentytwo_b_solution(FILE* fp) {
    (void)(fp);
}
