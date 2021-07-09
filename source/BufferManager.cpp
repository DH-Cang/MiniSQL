// date: 2021/6/21/16:41
#include "BufferManager.h"


//===============================
// read a block from file
//===============================
Block::Block(BlockID id)
{
    block_ptr = new char[BLOCK_SIZE];           // allocate block memory
    long long int offset = id * BLOCK_SIZE;     // offset of this Block
    fstream DataHeader(DATAFILE, ios::in | ios::binary);   // open the file
    DataHeader.seekg(offset, ios::beg);     // move header to file[offset]

    DataHeader.read(block_ptr, BLOCK_SIZE);     // read BLOCK_SIZE bytes
    this->bid = id;
    clear = Lock = DirtyBit = false;
    DataHeader.close();         // close file
}


//===============================
// restore the Block back to file if needed
//===============================
Block::~Block()
{
    if (DirtyBit == true)    // need rewrite
    {
        // open the file
        long long int offset = bid * BLOCK_SIZE;
        fstream DataHeader(DATAFILE, ios::in | ios::out | ios::binary);
        DataHeader.seekg(offset, ios::beg);     // header = file[offset]
        // restore it
        DataHeader.write(block_ptr, BLOCK_SIZE);
        // close file
        DataHeader.close();
    }

    delete block_ptr;           // free memory
}


//===============================
// return the id of one block
//===============================
inline BlockID Block::GetID()
{
    return bid;
}


//===============================
// access Block as a string
// throw "Invalid Block access!" if i is not valid
//===============================
char& Block::operator[](int i)
{
    if (i >= BLOCK_SIZE || i < 0) {
        throw "Invalid Block access!";
        return block_ptr[0];
    }
    else {
        return block_ptr[i];
    }

}


//=====================================
// read an integer from Block[offset]
// Generally, 4 bytes
//=====================================

int Block::ReadInt(int offset)
{
    int ret;
    int* dest = &ret;
    for (int i = 0; i < sizeof(int); i++)
        ((char*)dest)[i] = block_ptr[offset + i];

    return ret;
}


//=====================================
// write an integer from Block[offset]
// Generally, 4 bytes
//=====================================
void Block::WriteInt(int offset, int src)
{
	DirtyBit = true;
	int* SrcPtr = &src;
    for (int i = 0; i < sizeof(int); i++)
        block_ptr[offset + i] = ((char*)SrcPtr)[i];
}


//===================
// Initialize buffer
// Load 0~7 blocks
//===================
Buffer::Buffer()
{
    // read Block 0 (storing the information of empty space)
    TotalNum = 1;
    Block* base = new Block(0);
    buffer.push_back(base);

    // initialize ablespace
    int SpaceNum = base->ReadInt(0);
    for (int i = 1; i <= SpaceNum; i++) {
        BlockID EmptyID;
        EmptyID = base->ReadInt(i * sizeof(int));
        AbleSpace.push_back(EmptyID);
    }

}


//===================
// write back all the blocks if needed
//===================
Buffer::~Buffer()
{
    // Restore Block 0
    Block* base = new Block(0);
    base->DirtyBit = true;          // modify it
    int size = AbleSpace.size();
    base->WriteInt(0, size);
    for (int i = 1; i <= size; i++) {
        int EmptyID;
        EmptyID = AbleSpace.front();
        AbleSpace.pop_front();
        base->WriteInt(i * sizeof(int), EmptyID);
    }
    delete base;

    // iterate all the blocks and free them
    list<Block*>::iterator i;
    for (i = buffer.begin(); i != buffer.end(); i++)
        delete* i;  // rewrite will be done in ~Block
}


//==============================
// get the Block* with ID given
//==============================
Block* Buffer::Retrieve(BlockID id)
{
    Block* ret = NULL;

    // iterate the buffer and find such Block
    list<Block*>::iterator i;
    for (i = buffer.begin(); i != buffer.end(); i++)
    {
        if ((*i)->GetID() == id) {
            ret = *i;
            if (i != buffer.begin()) {
                buffer.erase(i);
                buffer.push_front(ret);
            }
            return ret;
        }
    }

    // not find such block
    ret = new Block(id);
    return this->AddToBuffer(ret);
}


//=========================
// Get an empty block
// DirtyBit == TRUE by default
//=========================
Block* Buffer::GetEmptyBlock(void)
{
    BlockID TheBlock = AbleSpace.back();    // top of stack
    AbleSpace.pop_back();
    if (AbleSpace.empty() == true)           // make sure that the bottom block is the end of file
        AbleSpace.push_back(TheBlock + 1);
    Block* ret = new Block(TheBlock);
    ret->DirtyBit = true;                   // modify datafile by default

    // add it to the buffer
    ret = AddToBuffer(ret);
    if (ret == NULL) // the buffer is locked, it fails
    {
        AbleSpace.push_back(TheBlock);
    }
    return ret;
}


//=========================================================
// Remove this block from file
// You shall not use this Block after calling SetEmpty()
//=========================================================
void Buffer::SetEmpty(BlockID id)
{
    // delete it from buffer if needed
    list<Block*>::iterator i;
    for (i = buffer.begin(); i != buffer.end(); i++)
    {
        if ((*i)->GetID() == id) {
            (*i)->DirtyBit = false;
            delete* i;
            buffer.erase(i);
			TotalNum--;
            break;
        }
    }

    AbleSpace.push_back(id);
    return;
}


//=========================================================
// Remove this block from file
// You shall not use this Block after calling SetEmpty()
//=========================================================
void Buffer::SetEmpty(Block* BlockPtr)
{
    BlockID id = BlockPtr->GetID();
    Buffer::SetEmpty(id);
    return;
}


//============================
// add a new Block ptr to Buffer
// return NULL if the whole Buffer is locked
//============================
Block* Buffer::AddToBuffer(Block* add)
{
    if (TotalNum < BUFFER_SIZE)  // buffer is not full
    {
        buffer.push_front(add);
        TotalNum++;
        return add;
    }
    else   // buffer is full
    {
        // find the last unlocked Block
        list<Block*>::iterator i;
        for (i = buffer.end(); i != buffer.begin(); )
        {
            i--;
            if ((*i)->Lock == false) {    // the last unlocked one
                Block* DelBlock;
                DelBlock = *i;

                buffer.erase(i);
                buffer.push_front(add);

                delete DelBlock;
                return add;
            }
        }
        // every block is locked
        delete add;
        return NULL;
    }
}


//======================================
// config the basic file
//======================================
void SetUpFile(void)
{
    fstream file;
    file.open(DATAFILE, ios::in);
    if (file)    // exist
    {
        file.close();
        return;
    }
    else        // doesn't exist. create one
    {
        file.open(DATAFILE, ios::out);
        file.close();
        Block* ptr = new Block(0);
        // Block0: 1 1
        ptr->DirtyBit = true;
        ptr->WriteInt(0, 1);
        ptr->WriteInt(sizeof(int), 1);
        delete ptr;
        return;
    }
}



