#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int rows;
    int cols;
    char empty;
    char obstacle;
    char full;
    char **grid;
} Map;

static int min3(int a, int b, int c)
{
    int m = a < b ? a : b;
    return m < c ? m : c;
}

static void free_map(Map *m)
{
    if (!m->grid)
        return;
    for (int i = 0; i < m->rows; i++)
        free(m->grid[i]);
    free(m->grid);
    m->grid = NULL;
}

static bool is_printable(char c)
{
    return c >= 32 && c <= 126;
}

static bool read_header(FILE *f, Map *m)
{
    if (fscanf(f, "%d %c %c %c", &m->rows, &m->empty, &m->obstacle, &m->full) != 4)
        return false;
    if (m->rows < 1
        || m->empty == m->obstacle
        || m->empty == m->full
        || m->obstacle == m->full)
        return false;
    return is_printable(m->empty)
        && is_printable(m->obstacle)
        && is_printable(m->full);
}

static ssize_t strip_newline(char *line)
{
    if (!line)
        return -1;
    ssize_t len = strlen(line);
    if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
        line[--len] = '\0';
    return len;
}

static bool read_map(FILE *f, Map *m)
{
    size_t cap = 0;
    char *line = NULL;
    ssize_t len;

    if (getline(&line, &cap, f) == -1) {
        free(line);
        return false;
    }

    m->cols = -1;
    m->grid = malloc(sizeof(char *) * m->rows);
    if (!m->grid) {
        free(line);
        return false;
    }

    for (int i = 0; i < m->rows; i++) {
        if (getline(&line, &cap, f) == -1) {
            free(line);
            free_map(m);
            return false;
        }

        len = strip_newline(line);
        if (len < 0 || (m->cols != -1 && (int)len != m->cols)) {
            free(line);
            free_map(m);
            return false;
        }

        if (m->cols == -1)
            m->cols = (int)len;

        for (int j = 0; j < (int)len; j++) {
            char c = line[j];
            if (c != m->empty && c != m->obstacle && c != m->full) {
                free(line);
                free_map(m);
                return false;
            }
        }

        m->grid[i] = malloc((m->cols + 1) * sizeof(char));
        if (!m->grid[i]) {
            free(line);
            free_map(m);
            return false;
        }

        for (int j = 0; j < m->cols; j++)
            m->grid[i][j] = line[j] == m->full ? m->empty : line[j];

        m->grid[i][m->cols] = '\0';
    }

    free(line);
    return true;
}

static bool solve(Map *m)
{
    int best = 0;
    int best_r = 0, best_c = 0;

    int *prev = calloc(m->cols, sizeof(int));
    int *curr = calloc(m->cols, sizeof(int));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return false;
    }

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (m->grid[i][j] == m->obstacle) {
                curr[j] = 0;
            } else if (i == 0 || j == 0) {
                curr[j] = 1;
            } else {
                curr[j] = min3(prev[j], prev[j - 1], curr[j - 1]) + 1;
            }

            if (curr[j] > best) {
                best = curr[j];
                best_r = i - best + 1;
                best_c = j - best + 1;
            }
        }
        int *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free(prev);
    free(curr);

    for (int i = best_r; i < best_r + best; i++)
        for (int j = best_c; j < best_c + best; j++)
            m->grid[i][j] = m->full;

    for (int i = 0; i < m->rows; i++)
        puts(m->grid[i]);

    return true;
}

static bool process_file(FILE *f)
{
    Map m = {0};
    if (!read_header(f, &m) || !read_map(f, &m)) {
        free_map(&m);
        printf("Error: invalid map\n");
        return false;
    }

    bool ok = solve(&m);
    free_map(&m);
    return ok;
}

int main(int argc, char **argv)
{
    if (argc == 1)
        return process_file(stdin) ? 0 : 1;

    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f)
            return 1;

        process_file(f);
        fclose(f);

        if (i + 1 < argc)
            putchar('\n');
    }

    return 0;
}