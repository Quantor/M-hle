#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class Mills
{
public:
	char node1;
	char node2;
	char node3;
};

class Connections
{
public:
	char Knoten1;
	char Knoten2;
};

int best_put_search(char board[], int number, Mills mill[], int n, int current_player,
	int board_search(char Zeichen, char board[], int number, int tries),
	int mill_search(char board[], int number, char piece_put, Mills mill[], int n, int current_player))
{
	int is_best=0;
	int is_best_points = 0;
	for(int i=0; i<number; i++)
	{
		int may_best = board_search('.', board, 24, i);
		int may_best_points = 0;
		if(mill_search(board, number, may_best+'A', mill, n, current_player))
		{
			may_best_points = may_best_points + 100;
		}
		if (mill_search(board, number, may_best + 'A', mill, n, !current_player))
		{
			may_best_points = may_best_points + 90;
		}
		if (may_best + 'A' == 'E'|| may_best + 'A' == 'N' || may_best + 'A' == 'K' || may_best + 'A' == 'T')
		{
			may_best_points = may_best_points + 20;
		}
		if(may_best + 'A' == 'B' || may_best + 'A' == 'H' || may_best + 'A' == 'J' || may_best + 'A' == 'L' || 
		may_best + 'A' == 'M' || may_best + 'A' == 'O' || may_best + 'A' == 'Q' || may_best + 'A' == 'W')
		{
			may_best_points = may_best_points + 10;
		}
		if(may_best_points>=is_best_points)
		{
			is_best = may_best;
			is_best_points = may_best_points;
		}
	}
	std::cout << is_best << std::endl;
	return is_best;
}

int mill_search(char board[], int number, char piece_put, Mills mill[], int n, int current_player)
{
	for (int i = 0; i < n; i++)
	{
		int n1, n2, n3;
		n1 = mill[i].node1 - 'A';
		n2 = mill[i].node2 - 'A';
		n3 = mill[i].node3 - 'A';
		if (mill[i].node1 == piece_put && board[n2] == current_player + '0' &&
			board[n3] == current_player + '0')
		{
			return 1;
		}
		if (mill[i].node2 == piece_put && board[n1] == current_player + '0' &&
			board[n3] == current_player + '0')
		{
			return 1;
		}
		if (mill[i].node3 == piece_put && board[n1] == current_player + '0' &&
			board[n2] == current_player + '0')
		{
			return 1;
		}
	}
	return 0;

}
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
	Connections edge[], int n, int current_player)
{
	for (int k = 0; k < n; k++)
	{
		int q, p;
		q = edge[k].Knoten1 - 'A';
		p = edge[k].Knoten2 - 'A';
		if (edge[k].Knoten2 == 'A' + i &&
			board[q] == current_player + '0')
		{
			return edge[k].Knoten1;
		}
		if (edge[k].Knoten1 == 'A' + i &&
			board[p] == current_player + '0')
		{
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
		Mills mill[16];
		int matches;
		int no_piece;
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

		// initialize mills with nodes
		mill[0].node1 = 'A'; mill[1].node1 = 'D'; mill[2].node1 = 'G'; mill[3].node1 = 'J';
		mill[0].node2 = 'B'; mill[1].node2 = 'E'; mill[2].node2 = 'H'; mill[3].node2 = 'K';
		mill[0].node3 = 'C'; mill[1].node3 = 'F'; mill[2].node3 = 'I'; mill[3].node3 = 'L';

		mill[4].node1 = 'M'; mill[5].node1 = 'P'; mill[6].node1 = 'S'; mill[7].node1 = 'V';
		mill[4].node2 = 'N'; mill[5].node2 = 'Q'; mill[6].node2 = 'T'; mill[7].node2 = 'W';
		mill[4].node3 = 'O'; mill[5].node3 = 'R'; mill[6].node3 = 'U'; mill[7].node3 = 'X';

		mill[8].node1 = 'A'; mill[9].node1 = 'D'; mill[10].node1 = 'G'; mill[11].node1 = 'B';
		mill[8].node2 = 'J'; mill[9].node2 = 'K'; mill[10].node2 = 'L'; mill[11].node2 = 'E';
		mill[8].node3 = 'V'; mill[9].node3 = 'S'; mill[10].node3 = 'P'; mill[11].node3 = 'H';

		mill[12].node1 = 'Q'; mill[13].node1 = 'I'; mill[14].node1 = 'F'; mill[15].node1 = 'C';
		mill[12].node2 = 'T'; mill[13].node2 = 'M'; mill[14].node2 = 'N'; mill[15].node2 = 'O';
		mill[12].node3 = 'W'; mill[13].node3 = 'R'; mill[14].node3 = 'U'; mill[15].node3 = 'X';


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
		printf("\n");
		printf("Nine Men's Morris\n");
		printf("=================\n");
		printf("\n");
		if (unplaced_pieces[0])
		{
			printf("Player 0: ");
			for (unsigned i = 0; i < unplaced_pieces[0]; i++)
				printf("%c", num2sym('0'));
			printf("\n");
		}
		if (unplaced_pieces[1])
		{
			printf("Player 1: ");
			for (unsigned i = 0; i < unplaced_pieces[1]; i++)
				printf("%c", num2sym('1'));
			printf("\n\n");
		}
		printf("Current board situation on the left,\n");
		printf("letters for identifying the places on the right:\n");
		printf("\n");
		printf("%c----------%c----------%c            A----------B----------C\n",
			num2sym(board[0]), num2sym(board[1]), num2sym(board[2]));
		printf("|          |          |            |          |          |\n");
		printf("|   %c------%c------%c   |            |   D------E------F   |\n",
			num2sym(board[3]), num2sym(board[4]), num2sym(board[5]));
		printf("|   |      |      |   |            |   |      |      |   |\n");
		printf("|   |   %c--%c--%c   |   |            |   |   G--H--I   |   |\n",
			num2sym(board[6]), num2sym(board[7]), num2sym(board[8]));
		printf("|   |   |     |   |   |            |   |   |     |   |   |\n");
		printf("%c---%c---%c     %c---%c---%c            J---K---L     M---N---O\n",
			num2sym(board[9]), num2sym(board[10]), num2sym(board[11]),
			num2sym(board[12]), num2sym(board[13]), num2sym(board[14]));
		printf("|   |   |     |   |   |            |   |   |     |   |   |\n");
		printf("|   |   %c--%c--%c   |   |            |   |   P--Q--R   |   |\n",
			num2sym(board[15]), num2sym(board[16]), num2sym(board[17]));
		printf("|   |      |      |   |            |   |      |      |   |\n");
		printf("|   %c------%c------%c   |            |   S------T------U   |\n",
			num2sym(board[18]), num2sym(board[19]), num2sym(board[20]));
		printf("|          |          |            |          |          |\n");
		printf("%c----------%c----------%c            V----------W----------X\n",
			num2sym(board[21]), num2sym(board[22]), num2sym(board[23]));
		printf("\n");
		printf("Player %d's move (%c).\n", current_player,
			num2sym((char)current_player + '0'));
		//Rules
		//Phase 1
		if (unplaced_pieces[current_player])
		{
			int i = best_put_search(board, 24, mill, 16, current_player, board_search, mill_search);
			piece_put = 'A' + i;
			if (mill_search(board, 24, piece_put, mill, 16, current_player))
			{
				int i = board_search(!current_player + '0', board, 24, 0);
				for (; i < 24; i++)
				{
					piece_kill = 'A' + i;
					int p_k = piece_kill - 'A';
					if (!mill_search(board, 24, piece_kill, mill, 16,
						!current_player) && board[p_k] == !current_player + '0')
					{
						break;
					}
					piece_kill = ' ';
				}
			}
		}

		//Phase 2
		if (unplaced_pieces[current_player] == 0)
		{
			//Phase 2.b(Only 3 pieces on the board)
			int first_piece = board_search(current_player + '0', board, 24, 0);
			int second_piece = board_search(current_player + '0', board, 24, first_piece + 1);
			int third_piece = board_search(current_player + '0', board, 24, second_piece + 1);
			if (board_search(current_player + '0', board, 24, third_piece + 1) == 0)
			{
				int free_field = best_put_search(board, 24, mill, 16, current_player, board_search, mill_search);
				piece_put = 'A' + free_field;
				int piece = board_search(current_player + '0', board, 24, 0);
				piece_move = 'A' + piece;
				no_piece = piece_move - 'A';
				board[no_piece] = '.';
				if (mill_search(board, 24, piece_put, mill, 16, current_player))
				{
					int i = board_search(!current_player + '0', board, 24, 0);
					for (; i < 24; i++)
					{
						piece_kill = 'A' + i;
						int p_k = piece_kill - 'A';
						if (!mill_search(board, 24, piece_kill, mill, 16,
							!current_player) && board[p_k] == !current_player + '0')
						{
							break;
						}
						piece_kill = ' ';
					}
				}

			}
			//Phase 2.a(more than 3 pieces on the board
			for (int tries = 0; piece_move == ' ' && tries<24; tries++)
			{
				int i = best_put_search(board, 24, mill, 16, current_player, board_search, mill_search);
				if (connect_search(i, board, 24, edge, 32, current_player))
				{
					piece_move = connect_search(i, board, 24, edge, 32, current_player);
					piece_put = 'A' + i;
					no_piece = piece_move - 'A';
					board[no_piece] = '.';
					if (mill_search(board, 24, piece_put, mill, 16, current_player))
					{
						int i = board_search(!current_player + '0', board, 24, 0);
						for (; i < 24; i++)
						{
							piece_kill = 'A' + i;
							int p_k = piece_kill - 'A';
							if (!mill_search(board, 24, piece_kill, mill, 16, !current_player) && board[p_k] == !current_player + '0')
							{
								break;
							}
							piece_kill = ' ';
						}

					}
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