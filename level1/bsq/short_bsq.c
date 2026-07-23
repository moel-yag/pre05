#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int min(int a, int b) { return a < b ? a : b; }

int slen(char *s)
{
    int i = 0;
    while (s[i] && s[i] != '\n') i++;
    return i;
}

void solve(FILE *f)
{
    char *line = 0, **g = 0;
    size_t l = 0;
    int n, w = 0, i, j, s = 0, x = 0, y = 0, *d = 0;
    char e, o, p;

    if (fscanf(f, "%d %c %c %c", &n, &e, &o, &p) != 4 || n <= 0 || e == o || e == p || o == p) goto err;
    if (getline(&line, &l, f) <= 0) goto err;

    g = malloc(n * sizeof(char *));
    for (i = 0; i < n; i++) {
        if (getline(&line, &l, f) <= 0) goto err;
        if ((j = slen(line)) != w && w) goto err;
        if (!w) w = j;
        if (w <= 0) goto err;
        g[i] = line;
        line = 0; l = 0;
        for (j = 0; j < w; j++)
            if (g[i][j] != e && g[i][j] != o) goto err;
    }

    d = calloc(n * w, sizeof(int));
    for (i = 0; i < n; i++)
        for (j = 0; j < w; j++)
            if (g[i][j] != o) {
                d[i * w + j] = (i && j) ? min(min(d[(i - 1) * w + j - 1], d[(i - 1) * w + j]), d[i * w + j - 1]) + 1 : 1;
                if (d[i * w + j] > s) s = d[i * w + j], x = j - s + 1, y = i - s + 1;
            }

    for (i = 0; i < n; i++) {
        for (j = 0; j < w; j++)
            g[i][j] = (i >= y && i < y + s && j >= x && j < x + s) ? p : g[i][j];
        g[i][w] = '\n';
        g[i][w + 1] = 0;
        fputs(g[i], stdout);
    }

    for (i = 0; i < n; i++) free(g[i]);
    free(g); free(d); free(line);
    return;
err:
    fprintf(stderr, "map error\n");
    if (g) for (i = 0; i < n && g[i]; i++) free(g[i]);
    free(g); free(d); free(line);
}

int main(int c, char **v)
{
    if (c == 1) solve(stdin);
    else for (int k = 1; k < c; k++) {
        FILE *f = fopen(v[k], "r");
        if (!f) fprintf(stderr, "map error\n");
        else { solve(f); fclose(f); }
    }
    return 0;
}
