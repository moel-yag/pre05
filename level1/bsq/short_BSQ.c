#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b) { return a < b ? a : b; }

void solve(FILE *f) {
    int n, w = 0, i, j, s = 0, x = 0, y = 0, *d;
    char e, o, p, b[4096], **g;

    if (!fgets(b, 4096, f)) goto err;
    n = atoi(b);
    i = strlen(b);
    if (n <= 0 || i < 4 || (e = b[i - 4]) == (o = b[i - 3]) || e == (p = b[i - 2]) || o == p) goto err;

    g = malloc(n * sizeof(char *));
    for (i = 0; i < n; i++) {
        if (!fgets(b, 4096, f)) goto err;
        if ((j = strlen(b) - 1) != w && w) goto err;
        if (!w) w = j;
        if (w <= 0) goto err;
        g[i] = strdup(b);
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
            putchar(i >= y && i < y + s && j >= x && j < x + s ? p : g[i][j]);
        putchar('\n');
    }
    return;
err:
    fprintf(stderr, "map error\n");
}

int main(int c, char **v) {
    if (c == 1) solve(stdin);
    else for (int k = 1; k < c; k++) {
        FILE *f = fopen(v[k], "r");
        if (!f) fprintf(stderr, "map error\n");
        else { solve(f); fclose(f); }
    }
    return 0;
}
