#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int ac, char **av)
{
	int wd, ht, it, i, j, x, y, p, yy, xx, d, dy, dx;
	char c, *b, *n;

	if (ac != 4 || (wd = atoi(av[1])) <= 0 || (ht = atoi(av[2])) <= 0 || (it = atoi(av[3])) < 0)
		return 1;

	b = malloc(ht * wd);
	n = malloc(ht * wd);
	if (!b || !n)
		return 1;

	for (i = 0; i < ht * wd; i++)
		b[i] = 0;

	p = 0;
	x = 0;
	y = 0;
	for (; read(0, &c, 1); )
	{
		if (c == 'w' && y > 0) y--;
		else if (c == 's' && y < ht - 1) y++;
		else if (c == 'a' && x > 0) x--;
		else if (c == 'd' && x < wd - 1) x++;
		else if (c == 'x') p = !p;
		if (p) b[y * wd + x] = 1;
	}

	for (; it > 0; it--)
	{
		for (yy = 0; yy < ht; yy++)
			for (xx = 0; xx < wd; xx++)
			{
				d = 0;
				for (dy = -1; dy <= 1; dy++)
					for (dx = -1; dx <= 1; dx++)
						if ((dx || dy) && yy + dy >= 0 && yy + dy < ht && xx + dx >= 0 && xx + dx < wd)
							d += b[(yy + dy) * wd + (xx + dx)];
				n[yy * wd + xx] = (d == 3 || (d == 2 && b[yy * wd + xx]));
			}
		for (i = 0; i < ht * wd; i++)
			b[i] = n[i];
	}

	for (i = 0; i < ht; i++)
	{
		for (j = 0; j < wd; j++)
			putchar(b[i * wd + j] ? 'O' : ' ');
		putchar('\n');
	}

	free(b);
	free(n);
	return 0;
}
