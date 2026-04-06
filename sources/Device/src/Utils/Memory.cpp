// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Memory.h"
#include <cstring>


/*
    Распределение памяти начинается с первого байта пула.
    Информация о выделенных блоках хранится в массиве структур Block, которые размещаются от конца к началу пула. Стуктуры расположены таким образом, что адреса, на которые они указывают, 
    увеличиваются от конца пула к началу, т.е. (рассмотрим массив из четырёх структур)

    lastBlock - 4, lastBlock - 3, lastBlock - 2, lastBlock - 1
    address      > address       > address      > begin
*/


// Описывает выделенный блок памяти.
struct Block
{
    uint8 *address;   // Адрес выделенного блока памяти
    uint size;      // Размер выделенного блока памяти
    Block *Next() { return nullptr; }
};


#define SIZE_POOL (1 * 1024)
static uint8 pool[SIZE_POOL];
//static uint8 *begin = pool;
static Block *firstBlock = ((Block *)&pool[SIZE_POOL]) - 1;
//static Block *lastBlock = nullptr;



// Найти свободный участок памяти. Если памяти нет - возвращает ноль.
static uint8 *FindRegion(uint size);
// Возвращает указатель на блок, отвечающий за адрес address
//static Block *FindBlockWithAddress(const uint8 *address);
// Вставить структуру Block в массив
static void InsertBlock(Block *block);
// Возвращает адрес первого (по адресу) выделенного блока памяти
static Block *FirstBlock();
// Возвращает адрес последнего (по адресу) выделенного блока памяти. LastBlokc() < FirstBlock()
//static Block *LastBlock();



void Memory::Init()
{
    std::memset(pool, 0, SIZE_POOL);
}


void *Memory::Allocate(uint size)
{
    uint8 *mem = FindRegion(size);

    if (mem)
    {
        Block block = { mem, size };
        InsertBlock(&block);
    }

    return (void *)mem;
}


static void InsertBlock(Block * /*block*/)
{
    //Block *first = FirstBlock();
    //Block *last = LastBlock();
}



void Memory::Free(void * /*address*/)
{

}


static uint8 *FindRegion(uint /*size*/)
{
    Block *block = FirstBlock();

    while (block->address != nullptr)
    {
        block = block->Next();
    }
    
    return nullptr;
}


/*
static Block *FindBlockWithAddress(const uint8 *address)
{
    Block *block = lastBlock;
    while (block->address != 0)
    {
        if (block->address == address)
        {
            return block;
        }
    }

    return 0;
}
*/


static Block *FirstBlock()
{
    if (firstBlock->address == nullptr)
    {
        return nullptr;
    }

    return firstBlock;
}


/*
static Block *LastBlock()
{
    if (!FirstBlock())
    {
        return nullptr;
    }

    Block *block = FirstBlock();

    do
    {
        block--;														// -V769
    } while (block->address != 0);

    return block + 1;
}
*/
