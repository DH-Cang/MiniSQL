// 2021/6/21 20:36
// released by cdh

#ifndef RECORD_MANAGER_FLAG
#define RECORD_MANAGER_FLAG

#include "BufferManager.h"
#include "CatalogManager.h"
#include <string>
#include <string.h>
#include <sstream>
#include <sstream>
using namespace std;


#define MAX_ATTRIBUTE 32    // a table contains at most 32 attributes
#define NULL_Block -1       // indicating NULL block pointer
#define NULL_offset -1      // indicating NULL offset pointer
// the size of different types
#define CHAR_WIDE 256
#define INT_WIDE 4
#define FLOAT_WIDE 4

typedef int Offset; // the ptr inside the block


class BlockHead
{
public:
    BlockHead();
    ~BlockHead() {};
    void Write(Block& block);
    void Read(Block& block);
    void BindTable(Table& t);   // when creating an empty table, bind it with a Blockhead

    int DataSize;
    int BitmapSize;
    int FreeRecordNum;
    int RecordNum;
    BlockID NextBlock;
    BlockID PrevBlock;
    Offset RecordPtr;
    Offset EmptyPtr;
};


// Physically:
// ptr to next -> bitmap -> data
// ---------------------------------------------
class Record    // covering the data of one record / a slot of an empty record
{
public:
    Record();
    Record(const Record& obj);
    void operator=(const Record& obj);
    ~Record();
    void ReadBlock(Block& block, BlockHead& head, Offset addr);
    void ReadOuter(void* bitmap, void* data, int bitmapSize, int DataSize);
    void WriteInto(Block& block, BlockHead& head);

    // get data from a piece of Retrieved() record
    int GetInt(int start);      // int result = data[start];
    float GetFloat(int start);  // float result = data[start];
    string GetString(int start);    // string result = data[start];
    Offset GetNext();           // return next

    int DataSize;
    int BitmapSize;
    void* data;
    void* bitmap;   // 0000_0000, 0000_0000,    1 means null, 0 means exist

private:
    Offset next;
};


// quickly access to record
class RecordAccess {
public:
    RecordAccess() {};
    ~RecordAccess() {};

    BlockID bid;
    Offset offset;
	bool operator< (const RecordAccess & ss) {
		if (bid < ss.bid) {
			return true;
		}
		else if (bid == ss.bid) return offset < ss.offset;
		else return false;
	}
    Record Read(Buffer& buf);
    void Erase(Buffer& buf);
};



//================================================
// Bind the table with a new block
// when you need to initialize a table
//================================================
void BindBlock(Buffer& buf, Table& table);


//================================================
// DeBind the table with all its block list
//================================================
void DeBind(Buffer& buf, Table& table);


//====================================
// insert a tuple into table t
// it doesn't check PrimaryKey/Unique
//====================================
RecordAccess InsertRecord(Buffer& buf, Table& t, Record& tuple);



//================================================
// Traverse every record in table
// delete them if check() == true
// block may be clear to save storage, but it isn't implemented yet
// not pass the test yet!!!!
//================================================
int DeleteRecord(Buffer& buf, Table& t, CheckCondition& condition);



//================================================
// Traverse every record in table
// copy them into returnlist if check() == true
// not pass the test yet!!!!
//================================================
int RetrieveRecord(Buffer& buf, Table& t, CheckCondition& condition, vector<Record>& returnlist);



bool check(Record& r, CheckCondition& c);
void DrawTable(Table& t, vector<Record>& r, int* AttriIsSelect);

/* to be implemented
    // update a record
int UpdateRecord(Buffer& buf, Table& t, CheckCondition& condition, void* data);
    // initialize record
    // access each record
    // update if needed
*/
#endif