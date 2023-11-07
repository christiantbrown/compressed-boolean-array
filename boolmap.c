// We use all the bits in an integer to store a boolean value in each one (store 64 booleans in a 64 bit int)

// This will use the innate site of an integer (the size of a register on the machine), which is the most efficient

// This is really not a great idea unless you're dealing with metadata or severe space constraints

// I just wanted this to exist, it isn't really practical

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "boolmap.h"
// returns a pointer to a custom built boolmap

#ifndef BOOLMAPHEADERS

#endif

boolmap *makeBoolMap(int size)
{
    boolmap *map = calloc(1, sizeof(boolmap));
    map->blockSize = sizeof(int);
    // truncation is fine here
    map->numBlocks = 1 + (int)(size / map->blockSize);
    int *blocks = calloc(map->numBlocks, map->blockSize);
    // if malloc fails return NULL
    if (blocks == NULL)
    {
        return NULL;
    }
    // we don't need an else block, since the if block terminates the function
    map->blocks = blocks;
    return map;
}

int searchBoolMap(boolmap *map, int index)
{
    if (index > ((map->numBlocks) * (map->blockSize)))
    {
        perror("Error: over indexed in Compressed Boolean Array");
        return -1;
    }
    int blockNo = (int)(index / map->blockSize);
    int bitIndex = index % map->blockSize;
    // here we use logical AND to compare with a test bit
    return (map->blocks[blockNo] & (1 << (bitIndex - 1))) ? 1 : 0;
}

int writeBoolMap(boolmap *map, int index, int truth)
{
    // example : index 1 (block number irrelevant)
    // writing false: AND with 0b11111101
    // writing true: OR with 0b00000010
    if (index > ((map->numBlocks) * (map->blockSize)))
    {
        perror("Error: over indexed in Compressed Boolean Array");
        return -1;
    }
    int blockNo = (int)(index / map->blockSize);
    int bitIndex = index % map->blockSize;
    int testBit = 1 << (bitIndex - 1);
    if (truth)
    {
        // performing logical OR that sets test bit to 1
        map->blocks[blockNo] = (map->blocks[blockNo] | testBit);
    }
    else
    {
        testBit = (testBit * -1) - 1;
        // this would turn 0b00100000 into 0b11011111
        // performing logical AND that sets bit to 0 in the array
        map->blocks[blockNo] = (map->blocks[blockNo] & testBit);
    }
    return 1;
}
int main()
{
    boolmap *map = makeBoolMap(45);
    writeBoolMap(map, 8, 1);
    for (int i = 0; i < 45; i++)
    {
        printf("%d %d\n", i, searchBoolMap(map, i));
    }
}