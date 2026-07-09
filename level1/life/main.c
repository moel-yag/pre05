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
	while (i < it)
	{
		int yy = 0;
		while (yy < ht)
		{
			int xx = 0;
			while (xx < wd)
			{
				int dense = 0;
				int dy = -1;
				while (dy <= 1)
				{
					int dx = -1;
					while (dx <= 1)
					{
						int ny = yy + dy;
						int nx = xx + dx;
						if ((dx || dy) && ny >= 0 && ny < ht && nx >= 0 && nx < wd)
							dense += board[ny * wd + nx];
						dx++;
					}
					dy++;
				}
				if (board[yy * wd + xx] && (dense == 2 || dense == 3))
					new_board[yy * wd + xx] = 1;
				else if (!board[yy * wd + xx] && dense == 3)
					new_board[yy * wd + xx] = 1;
				else
					new_board[yy * wd + xx] = 0;
				xx++;
			}
			yy++;
		}
		int j = 0;
		while (j < ht * wd)
		{
			board[j] = new_board[j];
			j++;
		}
		i++;
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
