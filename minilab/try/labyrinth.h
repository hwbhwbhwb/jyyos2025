#define MAX_ROWS 100
#define MAX_COLS 100
#define VERSION_INFO "Labyrinth Game"

typedef struct {
    char map[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Labyrinth;

typedef struct {
    int row;
    int col;
} Position;

bool loadMap(Labyrinth *labyrinth, const char *filename);

