#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class Connections
{
public:
	char Knoten1;
	char Knoten2;
};

/* convert player number to board symbols */
static char num2sym(char input)
{
	switch (input) {
	case '0':
		return 'X';
	case '1':
		return 'O';
	case '.':
		return ' ';
	default:
		return input;
	}
}

// Search for a specific charakter on the board
int board_search(char Zeichen, char board[], int number, int tries)
{
	for (int i = tries; i < number; i++)
	{
		if (board[i] == Zeichen)
		{
			return i;
		}
	}
	return 0;
}

//Search for connection between 'A' + i and a piece on board
char connect_search(int i, char board[], int number,
	Connections edge[], int n, unsigned current_player)
{
	for (int k = 0; k < n; k++)//i=J
	{
		int q, p;
		q = edge[k].Knoten1 - 'A';//q=A	
		p = edge[k].Knoten2 - 'A';//p=J	
		std::cout << board[0] << std::endl;
		std::cout << board[q] << std::endl;
		std::cout << edge[k].Knoten2 << std::endl;
		if (edge[k].Knoten2 == 'A' + i &&
			board[q] == 'X')
		{
			std::cout << k << std::endl;
			return edge[k].Knoten1;
		}
		if (edge[k].Knoten1 == 'A' + i &&
			board[p] == 'X')
		{
			std::cout << k << std::endl;
			return edge[k].Knoten2;
		}
	}
	return 0;
}



int main(void)
{
	/* current game state is delivered via file descriptor 3 */
	FILE *state_input = fdopen(3, "r");
	/* we output our move via file descriptor 4 */
	FILE *move_output = fdopen(4, "w");

	if (!state_input || !move_output)
	{
		fprintf(stderr, "error accessing file descriptors\n");
		exit(1);
	}

	while (1)
	{
		unsigned current_player;
		unsigned unplaced_pieces[2];
		char board[24];
		char piece_move = ' ', piece_put = ' ', piece_kill = ' ';
		char newline;
		Connections edge[32];
		int matches;
		// initialize edges with nodes
		edge[0].Knoten1 = 'A'; edge[1].Knoten1 = 'A'; edge[2].Knoten1 = 'B'; edge[3].Knoten1 = 'B';
		edge[0].Knoten2 = 'B'; edge[1].Knoten2 = 'J'; edge[2].Knoten2 = 'C'; edge[3].Knoten2 = 'E';

		edge[4].Knoten1 = 'C'; edge[5].Knoten1 = 'D'; edge[6].Knoten1 = 'D'; edge[7].Knoten1 = 'E';
		edge[4].Knoten2 = 'O'; edge[5].Knoten2 = 'E'; edge[6].Knoten2 = 'K'; edge[7].Knoten2 = 'F';

		edge[8].Knoten1 = 'E'; edge[9].Knoten1 = 'F'; edge[10].Knoten1 = 'G'; edge[11].Knoten1 = 'G';
		edge[8].Knoten2 = 'H'; edge[9].Knoten2 = 'N'; edge[10].Knoten2 = 'H'; edge[11].Knoten2 = 'L';

		edge[12].Knoten1 = 'H'; edge[13].Knoten1 = 'I'; edge[14].Knoten1 = 'J'; edge[15].Knoten1 = 'J';
		edge[12].Knoten2 = 'I'; edge[13].Knoten2 = 'M'; edge[14].Knoten2 = 'K'; edge[15].Knoten2 = 'V';

		edge[16].Knoten1 = 'K'; edge[17].Knoten1 = 'K'; edge[18].Knoten1 = 'L'; edge[19].Knoten1 = 'M';
		edge[16].Knoten2 = 'L'; edge[17].Knoten2 = 'S'; edge[18].Knoten2 = 'P'; edge[19].Knoten2 = 'N';

		edge[20].Knoten1 = 'M'; edge[21].Knoten1 = 'N'; edge[22].Knoten1 = 'N'; edge[23].Knoten1 = 'O';
		edge[20].Knoten2 = 'R'; edge[21].Knoten2 = 'O'; edge[22].Knoten2 = 'U'; edge[23].Knoten2 = 'X';

		edge[24].Knoten1 = 'P'; edge[25].Knoten1 = 'Q'; edge[26].Knoten1 = 'Q'; edge[27].Knoten1 = 'S';
		edge[24].Knoten2 = 'Q'; edge[25].Knoten2 = 'R'; edge[26].Knoten2 = 'T'; edge[27].Knoten2 = 'T';

		edge[28].Knoten1 = 'T'; edge[29].Knoten1 = 'T'; edge[30].Knoten1 = 'V'; edge[31].Knoten1 = 'W';
		edge[28].Knoten2 = 'U'; edge[29].Knoten2 = 'W'; edge[30].Knoten2 = 'W'; edge[31].Knoten2 = 'X';


		/* First, read the current game state, given as two lines of text. */

		/* The first line contains three decimal numbers: the current player
		* number (0 or 1) and the number of pieces not yet placed on the board
		* for both players. */
		matches = fscanf(state_input, "%u %u %u%c", &current_player,
			&unplaced_pieces[0], &unplaced_pieces[1], &newline);
		if (matches != 4)
		{
			fprintf(stderr, "error while reading the game state\n");
			break;
		}

		/* The next line has 24 characters, one for each place on the board,
		* starting in the top left. Each of these characters is either a .,
		* a 0 or a 1, showing that the place is free or occupied by the
		* respective player. */
		matches = fscanf(state_input, "%24c%c", board, &newline);
		if (matches != 2)
		{
			fprintf(stderr, "error while reading the board state\n");
			break;
		}
		//Rules
		//Phase 1
		if (unplaced_pieces[current_player])
		{
			int i = board_search('.', board, 24, 0);
			piece_put = 'A' + i;
		}

		//Phase 2
		if (unplaced_pieces[current_player] == 0)
		{
			for (int tries = 0; piece_move == ' ', tries<24; tries++)
			{
				int i = board_search('.', board, 24, tries);
				if (connect_search(i, board, 24, edge, 32, current_player))
				{
					piece_move = connect_search(i, board, 24, edge, 32, current_player);
					piece_put = 'A' + i;
				}
			}
		}



		/* Output our move as three characters, followed by a newline.
		* The first character specifies the piece to move or a space if an
		* unplaced piece is used.
		* The second character specifies the place to put the piece.
		* The third character specifies an opponent's piece to remove, or a
		* space if this is not appropriate. */
		fprintf(move_output, "%c%c%c\n", piece_move, piece_put, piece_kill);

		/* Flush the buffer so the MCP actually receives our move. */
		fflush(move_output);
	}

	/* cleanup */
	fclose(state_input);
	fclose(move_output);

	/* in all cases we get here, an error happened */
	return 1;
}