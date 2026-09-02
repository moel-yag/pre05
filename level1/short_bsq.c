#include "BSQ.h"

t_game game;

int check_line(char *line, int read)
{
    if (read < 1) return -1;
    if (line[read - 1] == '\n') { line[read - 1] = '\0'; read--; }
    if (game.width == -1) game.width = read;
    if (read != game.width) return -1;
    for (int i = 0; i < game.width; i++)
        if (line[i] != game.empty && line[i] != game.obst)
            return -1;
    return 0;
}

int isprintable(char c) { return (c >= 32 && c <= 126); }

int read_curr_game(FILE *file)
{
    int result = fscanf(file, "%d %c %c %c\n", &game.height, &game.empty, &game.obst, &game.draw);
    if (result != 4 || game.height <= 0)
        return -1;
    if (!isprintable(game.empty) || !isprintable(game.obst) || !isprintable(game.draw))
        return -1;
    if (game.empty == game.obst || game.empty == game.draw || game.obst == game.draw)
        return -1;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    game.cells = calloc(sizeof(char *), game.height);
    game.width = -1;
    for (int i = 0; i < game.height; i++)
    {
        read = getline(&line, &len, file);
        if (check_line(line, read)) { free(line); return -1; }
        game.cells[i] = line;
        line = NULL;
        len = 0;
    }
    return 0;
}

int read_curr_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "map error\n");
        return -1;
    }
    if (read_curr_game(file))
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
    for (int i = 0; i < game.height; i++)
        fprintf(stdout, "%s\n", game.cells[i]);
}

void free_map()
{
    if (!game.cells) return;
    for (int i = 0; i < game.height; i++)
    {
        free(game.cells[i]);
        game.cells[i] = NULL;
    }
    free(game.cells);
    game.cells = NULL;
}

void init_game()
{
    game.width = -1;
    game.height = -1;
    game.empty = '\0';
    game.obst = '\0';
    game.draw = '\0';
    game.cells = NULL;
    game.start.x = -1;
    game.start.y = -1;
    game.square_size = 0;
}

int check_square(int x, int y, int size)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (game.cells[y + i][x + j] == game.obst)
                return -1;
    return 0;
}

int find_square_size(int x, int y)
{
    int size = 0;
    for (int i = 0; i + y < game.height && i + x < game.width; i++)
    {
        if (check_square(x, y, i + 1))
            break;
        size++;
    }
    return size;
}

void find_largest_square()
{
    for (int i = 0; i < game.height; i++)
        for (int j = 0; j < game.width; j++)
        {
            int current = find_square_size(j, i);
            if (game.square_size < current)
            {
                game.square_size = current;
                game.start.x = j;
                game.start.y = i;
            }
        }
}

void draw_largest_square()
{
    for (int i = 0; i < game.square_size; i++)
        for (int j = 0; j < game.square_size; j++)
            game.cells[game.start.y + i][game.start.x + j] = game.draw;
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
