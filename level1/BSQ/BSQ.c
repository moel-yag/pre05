#include "BSQ.h"

t_game g_game;

int check_line(char *line, int read)
{
    if (read == -1 || read == 0)
        return -1;
    if (line[read - 1] == '\n')
    {
        line[read - 1] = '\0';
        read--;
    }
    if (g_game.width == -1)
        g_game.width = read;
    if (read != g_game.width)
        return -1;
    for (int i = 0; i < g_game.width; i++)
    {
        if (line[i] != g_game.empty && line[i] != g_game.obstacle)
            return -1;
    }
    return 0;
}

int isprintable(char c)
{
    return (c >= 32 && c <= 126);
}

int read_curr_game(FILE *file)
{
    //parse the first line
    int result = fscanf(file, "%d %c %c %c\n", &g_game.height, &g_game.empty, &g_game.obstacle, &g_game.draw);
    if (result != 4)
        return -1;
    if (g_game.height <= 0)
        return -1;
    if (!isprintable(g_game.empty) || !isprintable(g_game.obstacle) || !isprintable(g_game.draw))
        return -1;
    if (g_game.empty == g_game.obstacle || g_game.empty == g_game.draw || g_game.obstacle == g_game.draw)
        return -1;
    
    //parse the rest of the lines
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    g_game.cells = calloc(sizeof(char *), g_game.height);
    g_game.width = -1;
    for(int i = 0; i < g_game.height; i++)
    {
        read = getline(&line, &len, file);
        if (check_line(line, read) != 0)
        {
            free(line);
            return -1;
        }
        g_game.cells[i] = line;
        line = NULL;
        len = 0;
    }
    return 0;
}

int read_curr_file(char *filename)
{
    FILE *file;
    file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "map error\n");
        return -1;
    }
    if (read_curr_game(file) != 0)
    {
        fprintf(stderr, "map error\n");
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

void print_map()
{
    for (int i = 0; i < g_game.height; i++)
    {
        fprintf(stdout, "%s\n", g_game.cells[i]);
    }
}

void free_map()
{
    if (g_game.cells == NULL)
        return;
    for (int i = 0; i < g_game.height; i++)
    {
        free(g_game.cells[i]);
        g_game.cells[i] = NULL;
    }
    free(g_game.cells);
    g_game.cells = NULL;
}

void init_game()
{
    g_game.width = -1;
    g_game.height = -1;
    g_game.empty = '\0';
    g_game.obstacle = '\0';
    g_game.draw = '\0';
    g_game.cells = NULL;
    g_game.start.x = -1;
    g_game.start.y = -1;
    g_game.square_size = 0;
}

int check_square(int x, int y, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (g_game.cells[y + i][x + j] == g_game.obstacle)
                return -1;
        }
    }
    return 0;
}

int find_square_size(int x, int y)
{
    int size = 0;
    for(int i = 0; i + y < g_game.height && i + x < g_game.width; i++)
    {
        if (check_square(x, y, i + 1) != 0)
            break;
        size++;
    }
    return size;
}

void find_largest_square()
{
    for(int i = 0; i < g_game.height; i++)
    {
        for(int j = 0; j < g_game.width; j++)
        {
            int curr_size = find_square_size(j, i);
            if (g_game.square_size < curr_size)
            {
                g_game.square_size = curr_size;
                g_game.start.x = j;
                g_game.start.y = i;
            }
        }
    }
}

void draw_largest_square()
{
    for (int i = 0; i < g_game.square_size; i++)
    {
        for (int j = 0; j < g_game.square_size; j++)
        {
            g_game.cells[g_game.start.y + i][g_game.start.x + j] = g_game.draw;
        }
    }
}

int main(int argc, char *argv[])
{
    init_game();
    if (argc == 1)
    {
        if (read_curr_game(stdin))
        {
            fprintf(stderr, "map error\n");
            return -1;
        }
        find_largest_square();
        draw_largest_square();
        print_map();
        free_map();
    }
    for (int i = 1; i < argc; i++)
    {
        if (read_curr_file(argv[i]) == 0)
        {
            find_largest_square();
            draw_largest_square();
            print_map();
        }
        free_map();
        init_game();
    }
}
