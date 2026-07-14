#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;
	int wd = atoi(av[1]);
	int ht = atoi(av[2]);
	int it = atoi(av[3]);
	if (ht <= 0 || wd <= 0 || it < 0)
		return 1;

	char *board = malloc(ht * wd * sizeof(char));
	char *new_board = malloc(ht * wd * sizeof(char));
	if (!board || !new_board)
		return 1;

	int i = 0;
	while (i < ht * wd)
		board[i++] = 0;

	char c;
	int pen = 0, x = 0, y = 0;
	while (read(STDIN_FILENO, &c, 1))
	{
		if (c == 'w' && y > 0)
			y--;
		else if (c == 's' && y < ht - 1)
			y++;
		else if (c == 'a' && x > 0)
			x--;
		else if (c == 'd' && x < wd - 1)
			x++;
		else if (c == 'x')
			pen = !pen;

		if (pen)
			board[y * wd + x] = 1;
	}
	i = 0;
	while (it--)
	{
		for (int y = 0; y < ht; y++)
			for (int x = 0; x < wd; x++)
			{
				int n = 0;
				for (int dy = -1; dy <= 1; dy++)
					for (int dx = -1; dx <= 1; dx++)
						if ((dx || dy) && y+dy >= 0 && y+dy < ht && x+dx >= 0 && x+dx < wd)
							n += board[(y+dy)*wd + (x+dx)];
				new_board[y*wd+x] = (n == 3 || (n == 2 && board[y*wd+x]));
			}
		for (int i = 0; i < ht * wd; i++)
			board[i] = new_board[i];
	}
	i = 0;
	while (i < ht)
	{
		int j = 0;
		while (j < wd)
		{
			if (board[i * wd + j])
				putchar('O');
			else
				putchar(' ');
			j++;
		}
		putchar('\n');
		i++;
	}
	free(board);
	free(new_board);
	return 0;
}
