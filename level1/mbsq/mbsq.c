#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct map
{
    int lines;
    char empty;
    char obstacle;
    char full;
    char** map;
    int col;
    int row;
}t_map;


int min(int a,int b,int c) { int n = (a < b)?a:b; return (n < c)?n :c; }

void free_map(t_map m ,int row)
{
    if (!m.map)
        return;
    int i = 0;
    while (i < row)
        free(m.map[i++]);
    free(m.map);
    m.map = NULL;
    
}

int check_print(char c)
{
    if (c < 32 || c > 126)
        return 0;
    return 1;
}

int parse_header(FILE *f,t_map *m){
    
    int rd = fscanf(f,"%i %c %c %c",&m->lines,&m->empty,&m->obstacle,&m->full);
    if (rd != 4)
        return 0;
    if(m->lines < 1 || m->empty == m->obstacle ||  m->empty == m->full || m->obstacle == m->full)
        return 0;
    if (!check_print(m->empty) || !check_print(m->obstacle) || !check_print(m->full))
        return 0;
    return 1;
}

int ft_strlen(char* line)
{
    int i =  0;
    while (line[i])
        i++;
    if (line[i - 1] == '\n' || line[i - 1] == '\r')
    {
        line[i - 1] = '\0';
        --i;
    }
    return i;
}

int parse_map(FILE *f,t_map *m)
{
    int     len = 0, i = 0;
    size_t  cap = 0;
    char*   line = NULL;
    getline(&line,&cap,f);

    m->row = m->lines;
    m->col = -1;
    m->map = malloc(sizeof(char *) * m->lines);
    if (!m->map)
        return -1;
    while (i < m->row)
    {
        if (getline(&line,&cap,f) == -1)
        {
            free(line);
            free_map(*m,i);
            m->map = NULL;
            return 0;
        }
        len = ft_strlen(line);
        if (m->col == -1)
            m->col = len;
        if (m->col != len)
        {
           free(line);
           free_map(*m,i);
           m->map = NULL;
           return 0;
        }
        int c = 0;
        while (c < len)
        {
            if (line[c] != m->empty && line[c] != m->obstacle && line[c] != m->full)
            {
                free(line);
                free_map(*m,i);
                m->map = NULL;
                return 0;
            }
            c++;
        }
        m->map[i] = malloc(sizeof(char) * (len + 1));
        if (!m->map)
            return 0;
        for (size_t j = 0; j < m->col; j++)
        {
            m->map[i][j] = (line[j] == m->full)?m->empty:line[j];
        }
        m->map[i][len] = '\0';
        i++;
    }
    free(line);
    line = NULL;
}

void solve(t_map *m)
{
    int Bsize = 0,br = 0,bc = 0;
    int dp[m->row][m->col];
    for (size_t i = 0; i < m->row; i++)
    {
       for (size_t j = 0; j < m->col; j++)
       {
            if (m->map[i][j] == m->obstacle)
                dp[i][j] = 0;
            else
            {
                if (i == 0 || j == 0)
                    dp[i][j] = 1;
                else
                    dp[i][j] = min(dp[i - 1][j],dp[i - 1][j -1],dp[i][j - 1]) + 1;
                int sz = dp[i][j];
                int tr = i - sz + 1;
                int tc = j - sz + 1;
                if (sz > Bsize)
                {
                    Bsize = sz;
                    br = tr;
                    bc = tc;
                }
            }
       }
    }
    for (size_t i = br; i < br + Bsize; i++)
    {
        for (size_t j = bc; j < bc + Bsize; j++)
        {
            m->map[i][j] = m->full;
        }
    }
    for (size_t i = 0; i < m->row; i++)
    {
        fprintf(stdout,"%s\n",m->map[i]);
    }
   
}


int process(FILE *f)
{
    t_map mp;
    if (!parse_header(f,&mp) ||!parse_map(f,&mp))
    {
        printf("Error: invalid map");
    }
    
    solve(&mp);
    free_map(mp,mp.row);

    return 1;
}

int main(int ac, char** argv)
{
    if (ac == 1)
    {
       if (!process(stdin))
            return 1;
        return 0;
    }
    int i = 1;
    FILE *f;
    while (argv[i])
    {
        f = fopen(argv[i],"r");
        if (!f)
            return 1;
        if (!process(f))
        {
            i++;
            continue;
        }
        if (i + 1 < ac)
			printf("\n");
        i++; 
    }
    fclose(f);
    return 0;
}
