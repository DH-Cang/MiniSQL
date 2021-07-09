// version: 0.6.0
// date: 2021/6/20

// How to use this module? PLZ read me first
// 1. Call SetUpFile(), which will help in creating a data file
//      DO NOT create a data file by yourself !!!
// 2. Initialize a Buffer (There must be only one buffer)
// 3. Apply for an empty Block / Retrieve an expected Block from Buffer
//      Create Block* instead of Block object.
//      DO NOT access a Block that is not derived by your calling GetEmptyBlock(),
//      otherwise you will interfere other modules.
// 4. Access Blocks
//      If you suppose to modify its content, remember SET DirtyBit = True !!!
// 5. When your main function for test is over, Buffer will be deleted automatically, and all modification will be done then, so don't be bothered.
//      DO NOT free/delete Block*, which will be done when you end your functions
//
//
// DO NOT pin/lock over 8(BUFFER_SIZE) Blocks. If so, when you access any other Blocks by calling:
//      1. Retrieve() will free the most insignificant block
//      2. GetEmptyBlock() will return NULL
//

#ifndef BUFFER_MANAGER_FLAG
#define BUFFER_MANAGER_FLAG

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;

#define DATAFILE "MiniSQL_Data.dat"
#define BLOCK_SIZE 4096     // block size is 4KB
#define BUFFER_SIZE 8       // the buffer contains 8 blocks
typedef int BUFFER_ERROR;
typedef int BlockID;

void SetUpFile(void);

class Block {
public:
    Block(BlockID id);
    ~Block();
    char& operator[](int i);    // allow user to modify content as string
    BlockID GetID();            // get the id of Block
    int ReadInt(int offset);    // read an integer from Block[offset]
    void WriteInt(int offset, int src);     // store an integer at Block[offset]
    bool Lock;                  // change the status of lock
    bool DirtyBit;              // set True if user want to modify it

private:
    char* block_ptr;
    bool clear;
    BlockID bid;
};


class Buffer {
public:
    Buffer();
    ~Buffer();
    Block* Retrieve(BlockID id);   // Get a Block by ID, noticing that Block 0 is forbidden
    Block* GetEmptyBlock(void);     // Get an empty Block, whose DirtyBit is True by default
    void SetEmpty(BlockID id);      // Set a Block empty if you don't need it any more
    void SetEmpty(Block* BlockPtr);

    /* void showable(void){
        list<BlockID>::iterator i;
        for(i=AbleSpace.begin(); i!=AbleSpace.end(); i++){
            cout << *i << ' ';
        }
        cout << endl;
    } */

private:
    list<Block*> buffer;
    list<BlockID> AbleSpace;
    int TotalNum;
    Block* AddToBuffer(Block* add);
};

#endif