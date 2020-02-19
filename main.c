#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum player {
	NONE,
	CROSS,
	CIRCLE
};

enum position {
	TOP_LEFT,
	TOP_MID,
	TOP_RIGHT,
	MID_LEFT,
	MID_MID,
	MID_RIGHT,
	BOT_LEFT,
	BOT_MID,
	BOT_RIGHT
};

enum state {
	ONGOING,
	DRAW,
	WINNER
};

typedef struct Game {
	enum player player;
	enum state state;
	int board[9];
} Game;

int moves_left(Game *game) {
	int i, count;
	for (i = 0; i < 9; i++) {
		if (game->board[i] == NONE) {
			count++;
		}
	}
	return count;
}

enum position winning_states[8][3] = {
        {TOP_LEFT,  TOP_MID,   TOP_RIGHT},
        {MID_LEFT,  MID_MID,   MID_RIGHT},
        {BOT_LEFT,  BOT_MID,   BOT_RIGHT},
        {TOP_LEFT,  MID_LEFT,  BOT_LEFT},
        {TOP_MID,   MID_MID,   BOT_MID},
        {TOP_RIGHT, MID_RIGHT, BOT_RIGHT},
        {TOP_LEFT,  MID_MID,   BOT_RIGHT},
        {BOT_LEFT,  MID_MID,   TOP_RIGHT}
};

bool check_state(Game *game, enum position a, enum position b, enum position c) {
    return game->board[a] != NONE && game->board[a] == game->board[b] && game->board[b] == game->board[c];
}

bool check_states(Game *game, enum position states[8][3]) {
    for (int i = 0; i < 8; i++) {
        if (check_state(game, states[i][0], states[i][1], states[i][2])) {
            return true;
        }
    }
    return false;
}

enum state game_state(Game *game) {
	if (check_states(game, winning_states))  {
		return WINNER;
	}

	if (moves_left(game) == 0) {
		return DRAW;
	}
	
	return ONGOING;
}



int play(Game *self, enum position move) {
	if (move > 8) {
		fprintf(stderr, "move out of bounds, please choose a value between 0-8\n");
		return 1;
	}

	if (self->board[move] != (int)NONE) {
		fprintf(stderr, "a piece already exists on chosen move, please try a different move\n");
		return 2;
	}

	printf("Player: %d\n", self->player);
	self->board[move] = self->player;
	self->player = self->player == CIRCLE ? CROSS : CIRCLE;
	self->state = game_state(self);
		
	return 0;
}

void print_game(Game *self) {
	int i;
	for (i = 0; i < 9; i++) {
		printf("%d ", self->board[i]);
		if ((i+1)%3==0) {
			printf("\n");
		}
	}
}

enum player opposite_player(enum player p) {
	if (p == NONE) return NONE;
	return p == CIRCLE ? CROSS : CIRCLE;
}

Game *malloc_game() {
	Game *game;
	game = malloc(sizeof(Game));
	game->player = CIRCLE;
	game->state = ONGOING;
	return game;	
}


int main() {
	Game *game;
	game = malloc_game();

	if (!game) {
		fprintf(stderr, "could not allocate memory for game");
		return 1;
	}

	while (game->state == ONGOING) {
		int move;
		scanf("%d", &move);
		printf("You entered: %d\n", move);

		if (play(game, move) != 0) {
			exit(1);
		}
		print_game(game);
	}

	if (game->state == DRAW) {
		printf("Game Finished as a Draw!\n");
	} else if (game->state == WINNER) {
		printf("Game Finished: Winner is %d\n", opposite_player(game->player));
	} else {
		printf("What did you do!?\n");
	}
	
	free(game);	
	return 0;
}
