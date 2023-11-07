#include <stdint.h>

#define BOOLMAPHEADERS

typedef struct
{
    int *blocks;
    int numBlocks;
    int blockSize;
} boolmap;
// creates a struct containing a pointer to a compressed boolean array and some data about that array, returns a pointer
boolmap *makeBoolMap(int size);

// grabs the truth value from the specified index
// returns 1 on true, 0 on false, -1 on error
int searchBoolMap(boolmap *map, int index);

// writes the specified truth value (0 = false, anything else = true) to the specified index
int writeBoolMap(boolmap *map, int index, int truth);