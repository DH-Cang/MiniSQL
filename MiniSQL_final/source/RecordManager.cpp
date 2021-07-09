// 2021/6/21 20:36
// released by cdh

#include "RecordManager.h"
using namespace std;

BlockHead::BlockHead() :
    DataSize(-1), BitmapSize(-1), FreeRecordNum(-1), RecordNum(-1),
    NextBlock(-1), PrevBlock(-1), RecordPtr(-1), EmptyPtr(-1)
{}

Record::Record() :
    DataSize(-1), BitmapSize(-1), next(-1),
    data(NULL), bitmap(NULL)
{}

Record::~Record() {
    free(data);
    free(bitmap);
    return;
}

Record::Record(const Record& obj)
{
	this->DataSize = obj.DataSize;
	this->BitmapSize = obj.BitmapSize;
	this->next = obj.next;

	this->data = malloc(obj.DataSize);
	memcpy(this->data, obj.data, obj.DataSize);

	this->bitmap = malloc(obj.BitmapSize);
	memcpy(this->bitmap, obj.bitmap, obj.BitmapSize);
}

void Record::operator=(const Record& obj)
{
    this->DataSize = obj.DataSize;
    this->BitmapSize = obj.BitmapSize;
    this->next = obj.next;

	if (this->data) free(this->data);
    this->data = malloc(obj.DataSize);
    memcpy(this->data, obj.data, obj.DataSize);

	if (this->bitmap) free(this->bitmap);
    this->bitmap = malloc(obj.BitmapSize);
    memcpy(this->bitmap, obj.bitmap, obj.BitmapSize);
    return;
}



//=============================
// Write BlockHead into block
//=============================
void BlockHead::Write(Block& block)
{
    block.WriteInt(0 * sizeof(int), DataSize);
    block.WriteInt(1 * sizeof(int), BitmapSize);
    block.WriteInt(2 * sizeof(int), FreeRecordNum);
    block.WriteInt(3 * sizeof(int), RecordNum);
    block.WriteInt(4 * sizeof(int), NextBlock);
    block.WriteInt(5 * sizeof(int), PrevBlock);
    block.WriteInt(6 * sizeof(int), RecordPtr);
    block.WriteInt(7 * sizeof(int), EmptyPtr);
    block.DirtyBit = true;
}


//=============================
// Read BlockHead from block
//=============================
void BlockHead::Read(Block& block)
{
    DataSize = block.ReadInt(0 * sizeof(int));
    BitmapSize = block.ReadInt(1 * sizeof(int));
    FreeRecordNum = block.ReadInt(2 * sizeof(int));
    RecordNum = block.ReadInt(3 * sizeof(int));
    NextBlock = block.ReadInt(4 * sizeof(int));
    PrevBlock = block.ReadInt(5 * sizeof(int));
    RecordPtr = block.ReadInt(6 * sizeof(int));
    EmptyPtr = block.ReadInt(7 * sizeof(int));
}


//================================================
// connect the information of table with blockhead
//================================================
void BlockHead::BindTable(Table& table)
{
    // calculating data size
    DataSize = 0;
    vector<Attribute>::iterator i;
    for (i = table.AttributeInTable.begin(); i != table.AttributeInTable.end(); i++)
    {
        if (i->AttributeType == -1)  // int
        {
            DataSize += sizeof(int);
        }
        else if (i->AttributeType == 0)  // float
        {
            DataSize += sizeof(float);
        }
        else if (i->AttributeType >= 1 && i->AttributeType <= 255)   // char
        {
            DataSize += CHAR_WIDE;
        }
        else    // error
        {
            throw "UndefinedBehavior";   //UndefinedBehavior();
        }
    }// DataSize complete

    // calculate BitmapSize
    BitmapSize = table.AttributeNumber / 8 + ((table.AttributeNumber % 8 == 0) ? 0 : 1);

    // calculate free space in one block (the size of record = sizeof(int) + BitmapSize + DataSize)
    RecordNum = 0;
    FreeRecordNum = (BLOCK_SIZE - sizeof(BlockHead)) / (sizeof(int) + BitmapSize + DataSize);

    // connect
    NextBlock = PrevBlock = NULL_Block;

    // prepare ptr
    EmptyPtr = 0 + sizeof(BlockHead);
    RecordPtr = NULL_offset;

}





//================================================
// read an integer from data[start]
//================================================
int Record::GetInt(int start)
{
    int ret;
    ret = *(int*)((char*)data + start);
    return ret;
}


//================================================
// read a float from data[start]
//================================================
float Record::GetFloat(int start)
{
    float ret;
    ret = *(float*)((char*)data + start);
    return ret;
}


//================================================
// read a string from data[start]
//================================================
string Record::GetString(int start)
{
    string ret;
    char* ptr = (char*)data + start;
    for (int i = 0; ptr[i] && i < 256; i++)
    {
        ret.push_back(ptr[i]);
    }
    
    //memcpy(&ret, ptr, sizeof(ptr));
    return ret;
}


//================================================
// return "next" of record
//================================================
inline Offset Record::GetNext()
{
    return this->next;
}


//=========================================================
// copy bitmap and data into Record
// if user want to construct Record by offering such data
//=========================================================
void Record::ReadOuter(void* bitmap, void* data, int bitmapSize, int DataSize)
{
    this->DataSize = DataSize;
    this->BitmapSize = bitmapSize;
    next = NULL_offset;

	if (this->data) free(this->data);
    this->data = malloc(DataSize);
    memcpy(this->data, data, DataSize);

	if (this->bitmap) free(this->bitmap);
    this->bitmap = malloc(bitmapSize);
    memcpy(this->bitmap, bitmap, bitmapSize);
    return;
}


//=========================================================
// read a record from block[addr]
//=========================================================
void Record::ReadBlock(Block& block, BlockHead& head, Offset addr)
{
    DataSize = head.DataSize;
    BitmapSize = head.BitmapSize;

    // read ptr
    next = block.ReadInt(addr);
    addr += sizeof(int);

    // read bitmap
	if (bitmap) free(bitmap);
    bitmap = malloc(BitmapSize);
    for (int i = 0; i < BitmapSize; i++, addr++)
        *((char*)bitmap + i) = block[addr];

    // read data
	if (data) free(data);
    data = malloc(DataSize);
    for (int i = 0; i < DataSize; i++, addr++)
        *((char*)data + i) = block[addr];

    return;
}




//=========================================================
// write a record into block
// do not test if the block is full
//=========================================================
void Record::WriteInto(Block& block, BlockHead& head)
{
    Offset StartAddr = head.EmptyPtr;               // take start address(an empty space, i.e. record)
    head.EmptyPtr = block.ReadInt(head.EmptyPtr);   // empty ptr -> next empty
    block.WriteInt(StartAddr, head.RecordPtr);      // record->next = RecordPtr
    head.RecordPtr = StartAddr;                     // RecordPtr = record

    this->next = block.ReadInt(StartAddr); // modify record
    head.FreeRecordNum--;                   // modify head
    head.RecordNum++;

    StartAddr += sizeof(int);   // jump over "next ptr"
    for (int i = 0; i < BitmapSize; i++, StartAddr++)
        block[StartAddr] = *((char*)bitmap + i);

    for (int i = 0; i < DataSize; i++, StartAddr++)
        block[StartAddr] = *((char*)data + i);

    head.Write(block);  // change block
    return;
}


//=================================
// read a record from (bid, offset)
//=================================
Record RecordAccess::Read(Buffer& buf)
{
    Record ret;
    Block* block = buf.Retrieve(bid);
    BlockHead head;
    head.Read(*block);
    ret.bitmap = malloc(head.BitmapSize);
    ret.data = malloc(head.DataSize);
    ret.ReadBlock(*block, head, offset);

    return ret;
}


//================================
// Erase a record based on given address(bid, offset)
//================================
void RecordAccess::Erase(Buffer& buf)
{
    Block* block = buf.Retrieve(bid);
    BlockHead head;
    head.Read(*block);

    // find the location of that record
    Offset ptr = head.RecordPtr;
    Offset prev = NULL_offset;
    while (1)
    {
        if (ptr == this->offset)   // find
        {
            break;
        }
        else if (ptr == NULL_offset)    // doesn't find
        {
            throw "RecordAccess::Erase fails";
        }
        else
        {
            prev = ptr;
            ptr = block->ReadInt(ptr);  // ptr = ptr->next;
        }
    }

    if (prev == NULL_offset)               // if ptr == head ptr
        head.RecordPtr = block->ReadInt(ptr);   // head ptr = ptr->next
    else
        block->WriteInt(prev, block->ReadInt(ptr)); // prev->next = ptr->next
    block->WriteInt(ptr, head.EmptyPtr);         // ptr->next = empty ptr
    head.EmptyPtr = ptr;                        // empty ptr = ptr;

    head.RecordNum--;
    head.FreeRecordNum++;

    if (head.RecordNum == 0 && head.PrevBlock != NULL_Block) // this block should be cleared
    {
        Block* tmpBlock;
        BlockHead tmphead;
        // next
        if (head.NextBlock != NULL_Block)    // next != NULL
        {
            tmpBlock = buf.Retrieve(head.NextBlock);
            tmphead.Read(*tmpBlock);
            tmphead.PrevBlock = head.PrevBlock;
            tmphead.Write(*tmpBlock);
        }
        // prev
        tmpBlock = buf.Retrieve(head.PrevBlock);
        tmphead.Read(*tmpBlock);
        tmphead.NextBlock = head.NextBlock;
        tmphead.Write(*tmpBlock);

        //delete this block
        buf.SetEmpty(block);
    }
    else
    {
        head.Write(*block);
    }

    return;
}

bool check(Record& r, CheckCondition& c) {
    int i, j, k;
    int offset;
    int isint;
    float isfloat;
    string ischar;
    if (c.condition.size() == 0) {
        return true;
    }
    if (c.ifall == true) {
        return true;
    }
    for (i = 0; i < c.condition.size(); i++) {
        offset = 0;
        for (j = 0; j < c.ConditionTable.AttributeNumber; j++) {
            if (j == c.condition[i].CAttributeNum) {
                break;
            }
            if (c.ConditionTable.AttributeInTable[j].AttributeType == -1) {
                offset = offset + 4;
            }
            else if (c.ConditionTable.AttributeInTable[j].AttributeType == 0) {
                offset = offset + 4;
            }
            else {
                offset = offset + 256;
            }
        }
        void* temp = malloc(1);
        *((char*)temp) = *((char*)r.bitmap + j / 8);
        int shift = 8 - j % 8 - 1;
        *((char*)temp) = *((char*)temp) >> shift;
        if (*((char*)temp) != 0) {
            return false;
        }
        // read from r by offset
        if (c.condition[i].CAttributeType == -1) {
            //int
            isint = r.GetInt(offset);
        }
        else if (c.condition[i].CAttributeType == 0) {
            //float
            isfloat = r.GetFloat(offset);
        }
        else {
            //char
            ischar = r.GetString(offset);
			ischar = "'" + ischar + "'";
        }
        if (c.condition[i].co == Oequal) {
            if (c.condition[i].CAttributeType == -1) { //int
                if (c.condition[i].Cint != isint) {
                    return false;
                }
            }
            else if (c.condition[i].CAttributeType == 0) { //float
                if (c.condition[i].Cfloat != isfloat) {
                    return false;
                }
            }
            else { //char
                if (c.condition[i].Cchar != ischar) {
                    return false;
                }
            }
        }
        else if (c.condition[i].co == Onotequal) {
            if (c.condition[i].CAttributeType == -1) { //int
                if (c.condition[i].Cint == isint) {
                    return false;
                }
            }
            else if (c.condition[i].CAttributeType == 0) { //float
                if (c.condition[i].Cfloat == isfloat) {
                    return false;
                }
            }
            else { //char
                if (c.condition[i].Cchar == ischar) {
                    return false;
                }
            }
        }
        else if (c.condition[i].co == Ogreater) {
            if (c.condition[i].CAttributeType == -1) { //int
                if (c.condition[i].Cint >= isint) {
                    return false;
                }
            }
            else if (c.condition[i].CAttributeType == 0) { //float
                if (c.condition[i].Cfloat >= isfloat) {
                    return false;
                }
            }
            else { //char
                if (c.condition[i].Cchar >= ischar) {
                    return false;
                }
            }
        }
        else if (c.condition[i].co == Oless) {
            if (c.condition[i].CAttributeType == -1) { //int
                if (c.condition[i].Cint <= isint) {
                    return false;
                }
            }
            else if (c.condition[i].CAttributeType == 0) { //float
                if (c.condition[i].Cfloat <= isfloat) {
                    return false;
                }
            }
            else { //char
                if (c.condition[i].Cchar <= ischar) {
                    return false;
                }
            }
        }
        else if (c.condition[i].co == Ogreater_equal) {
            if (c.condition[i].CAttributeType == -1) { //int
                if (c.condition[i].Cint > isint) {
                    return false;
                }
            }
            else if (c.condition[i].CAttributeType == 0) { //float
                if (c.condition[i].Cfloat > isfloat) {
                    return false;
                }
            }
            else { //char
                if (c.condition[i].Cchar > ischar) {
                    return false;
                }
            }
        }
        else if (c.condition[i].co == Oless_equal) {
            if (c.condition[i].CAttributeType == -1) { //int
                if (c.condition[i].Cint < isint) {
                    return false;
                }
            }
            else if (c.condition[i].CAttributeType == 0) { //float
                if (c.condition[i].Cfloat < isfloat) {
                    return false;
                }
            }
            else { //char
                if (c.condition[i].Cchar < ischar) {
                    return false;
                }
            }
        }
    }
    return true;
}

void DrawTable(Table& t, vector<Record>& r, int* AttriIsSelect) {
    //cout << "record size" << r.size() << endl;
    int i, j, k;
    int blanknum;
    const int AttriNum = t.AttributeNumber;
    int* length = new int[AttriNum];
    int* startoffset = new int[AttriNum];
    for (i = 0; i < t.AttributeNumber; i++) {
        if (t.AttributeInTable[i].AttributeType == -1) {
            length[i] = 12;
            if (t.AttributeInTable[i].AttributeName.size() > length[i]) {
                length[i] = t.AttributeInTable[i].AttributeName.size() + 2;
            }
        }
        else if (t.AttributeInTable[i].AttributeType == 0) {
            length[i] = 15;
            if (t.AttributeInTable[i].AttributeName.size() > length[i]) {
                length[i] = t.AttributeInTable[i].AttributeName.size() + 2;
            }
        }
        else {
            length[i] = t.AttributeInTable[i].AttributeType;
            if (t.AttributeInTable[i].AttributeName.size() > length[i]) {
                length[i] = t.AttributeInTable[i].AttributeName.size() + 2;
            }
        }
        startoffset[i] = 0;
        for (j = 0; j < i; j++) {
            if (t.AttributeInTable[j].AttributeType == -1) {
                startoffset[i] = startoffset[i] + 4;
            }
            else if (t.AttributeInTable[j].AttributeType == 0) {
                startoffset[i] = startoffset[i] + 4;
            }
            else {
                startoffset[i] = startoffset[i] + 255;
            }
        }
    }
    /*
    for (i = 0; i < t.AttributeNumber; i++) {
        cout << length[i] << " ";
    }
    */
    cout << endl;
    // ↓ print the first line like "+---+---+---+"
    for (i = 0; i < t.AttributeNumber; i++) {
        if (AttriIsSelect[i] == 1) {
            cout << "+";
            for (j = 0; j < length[i]; j++) {
                cout << "-";
            }
        }
    }
    cout << "+" << endl;

    // ↓ print the attribute name like "| a | b | c |" (*Φ皿Φ*)
    for (i = 0; i < t.AttributeNumber; i++) {
        if (AttriIsSelect[i] == 1) {
            cout << "|";
            blanknum = length[i] - t.AttributeInTable[i].AttributeName.size();
            blanknum = blanknum / 2;
            if (blanknum < 0) {
                blanknum = 0;
            }
            for (k = 0; k < blanknum; k++) {
                cout << " ";
            }
            cout << t.AttributeInTable[i].AttributeName;
            for (k = 0; k < (length[i] - blanknum - t.AttributeInTable[i].AttributeName.size()); k++) {
                cout << " ";
            }
        }
    }
    cout << "|" << endl;

    // ↓ print the first line like "+---+---+---+"
    for (i = 0; i < t.AttributeNumber; i++) {
        if (AttriIsSelect[i] == 1) {
            cout << "+";
            for (j = 0; j < length[i]; j++) {
                cout << "-";
            }
        }
    }
    cout << "+" << endl;

    // ↓ print the tuples
    for (i = 0; i < r.size(); i++) {
        int offset = 0;
        cout << "|";
        for (j = 0; j < t.AttributeNumber; j++) {
            if (AttriIsSelect[j] == 1) {
                //用startoffset和Record成员函数返回数据
                void* temp = malloc(1);
                *((char*)temp) = *((char*)r[i].bitmap + j / 8);
                int shift = 8 - j % 8 - 1;
                *((char*)temp) = *((char*)temp) >> shift;
                if (*((char*)temp) != 0) {
                    // is null
                    blanknum = length[i] - 4;
                    blanknum = blanknum / 2;
                    for (k = 0; k < blanknum; k++) {
                        cout << " ";
                    }
                    cout << "null";
                    for (k = 0; k < (length[i] - blanknum - 4); k++) {
                        cout << " ";
                    }
                    if (t.AttributeInTable[j].AttributeType == -1) {
                        offset = offset + 4;
                    }
                    else if (t.AttributeInTable[j].AttributeType == 0) {
                        offset = offset + 4;
                    }
                    else {
                        offset = offset + 256;
                    }
                }
                else {
                    // is not null
                    if (t.AttributeInTable[j].AttributeType == -1) {
                        int intprint;
                        intprint = r[i].GetInt(offset);
                        int intlength = 0;
                        int temp = intprint;
                        while (temp != 0) {
                            temp = temp / 10;
                            intlength++;
                        }
                        blanknum = length[j] - intlength;
                        blanknum = blanknum / 2;
                        //cout << length[i] << " " << blanknum << endl;
                        if (blanknum < 0) {
                            blanknum = 0;
                        }
                        for (k = 0; k < blanknum; k++) {
                            cout << " ";
                        }
                        cout << intprint;
                        for (k = 0; k < (length[j] - blanknum - intlength); k++) {
                            cout << " ";
                        }
                        offset = offset + 4;
                    }
                    else if (t.AttributeInTable[j].AttributeType == 0) {
                        float floatprint;
                        floatprint = r[i].GetFloat(offset);
                        //string fstring = to_string(floatprint);
                        string fstring;
                        ostringstream strstream;
                        strstream << floatprint;
                        fstring = strstream.str();
                        /*
                        for (int w = fstring.size() - 1; w >= 0; w--) {
                            if (fstring[w] == '0' || fstring[w] == '.') {
                                fstring[w] = '\0';
                            }
                            else {
                                break;
                            }
                        }
                        */
                        int floatlength = fstring.size();
                        //cout << "floatstring=" << fstring << endl;
                        //cout << "floatlength=" << floatlength << endl;
                        blanknum = length[j] - floatlength;
                        blanknum = blanknum / 2;
                        if (blanknum < 0) {
                            blanknum = 0;
                        }
                        for (k = 0; k < blanknum; k++) {
                            cout << " ";
                        }
                        cout << fstring;
                        for (k = 0; k < (length[j] - blanknum - floatlength); k++) {
                            cout << " ";
                        }
                        offset = offset + 4;
                    }
                    else {
                        string stringprint;
                        stringprint = r[i].GetString(offset);
                        int stringlength;
                        stringlength = stringprint.size();
                        blanknum = length[j] - stringlength;
                        blanknum = blanknum / 2;
                        if (blanknum < 0) {
                            blanknum = 0;
                        }
                        for (k = 0; k < blanknum; k++) {
                            cout << " ";
                        }
                        cout << stringprint;
                        for (k = 0; k < (length[j] - blanknum - stringlength); k++) {
                            cout << " ";
                        }
                        offset = offset + 256;
                    }
                }
                cout << "|";
            }
            else {
                if (t.AttributeInTable[j].AttributeType == -1) {
                    offset = offset + 4;
                }
                else if (t.AttributeInTable[j].AttributeType == 0) {
                    offset = offset + 4;
                }
                else {
                    offset = offset + 256;
                }
            }
        }
        cout << endl;
    }
    if (r.size() == 0) {
        int wholelength = 0;
        for (i = 0; i < t.AttributeNumber; i++) {
            wholelength = wholelength + length[i];
        }
        wholelength = wholelength + 2;
        cout << "|";
        string norecord = "no record returned";
        blanknum = wholelength - norecord.size();
        blanknum = blanknum / 2;
        if (blanknum < 0) {
            blanknum = 0;
        }
        for (k = 0; k < blanknum; k++) {
            cout << " ";
        }
        cout << norecord;
        for (k = 0; k < (wholelength - blanknum - norecord.size()); k++) {
            cout << " ";
        }
        cout << "|" << endl;
    }
    // ↓ print the last line like "+---+---+---+"
    for (i = 0; i < t.AttributeNumber; i++) {
        if (AttriIsSelect[i] == 1) {
            cout << "+";
            for (j = 0; j < length[i]; j++) {
                cout << "-";
            }
        }
    }
    cout << "+" << endl;
    cout << r.size() << " rows in set" << endl;
}




//====================================
// insert a tuple into table t
// it doesn't check PrimaryKey/Unique
//====================================
RecordAccess InsertRecord(Buffer& buf, Table& t, Record& tuple) {
    Block* block;
    BlockHead head;
    RecordAccess ret;

    block = buf.Retrieve(t.BlockNumber);
    head.Read(*block);
    while (1) // traverse each block ; find not full
    {
        if (head.FreeRecordNum == 0 && head.NextBlock != NULL_Block)    // this is full, go to next
        {
            block = buf.Retrieve(head.NextBlock);
            head.Read(*block);
            continue;
        }
        else if (head.FreeRecordNum == 0 && head.NextBlock == NULL_Block) // the whole list is full
        {
            int temp;   // save origin head id
            Block* newblock;
            BlockHead newhead;

            temp = t.BlockNumber;
            BindBlock(buf, t);          // table->new block
            newblock = buf.Retrieve(t.BlockNumber);
            t.BlockNumber = temp;       // restore table->block id
            newhead.Read(*newblock);

            // connect
            head.NextBlock = newblock->GetID();
            newhead.PrevBlock = block->GetID();

            // write into new block
            tuple.WriteInto(*newblock, newhead);
            ret.offset = newhead.RecordPtr;
            ret.bid = newblock->GetID();

            // head write back
            head.Write(*block);
            newhead.Write(*newblock);

            break;
        }
        else    // not full, insert
        {
            tuple.WriteInto(*block, head);
            ret.offset = head.RecordPtr;
            ret.bid = block->GetID();
            break;
        }
    }

    return ret;
}


//================================================
// DeBind the table with all its block list
//================================================
void DeBind(Buffer& buf, Table& table)
{
    Block* ptr;
    BlockHead head;

    ptr = buf.Retrieve(table.BlockNumber);
    head.Read(*ptr);
    buf.SetEmpty(ptr);  // clear this block

    while (head.NextBlock != NULL_Block)
    {
        ptr = buf.Retrieve(head.NextBlock);
        head.Read(*ptr);
        buf.SetEmpty(ptr);
    }

    return;
}


//================================================
// Bind the table with a new block
// when you need to initialize a table
//================================================
void BindBlock(Buffer& buf, Table& table)
{
    // bind table with this block
    Block* block = buf.GetEmptyBlock();
    table.BlockNumber = block->GetID();

    // initialze header
    BlockHead header;
    header.BindTable(table);
    header.Write(*block);   block->DirtyBit = true;

    // initialize empty list
    Offset Ptr = header.EmptyPtr;
    for (int i = 1; i <= header.FreeRecordNum; i++)
    {
        if (i == header.FreeRecordNum)
            block->WriteInt(Ptr, NULL_offset);
        else
            block->WriteInt(Ptr, Ptr + sizeof(int) + header.DataSize + header.BitmapSize);
        Ptr += sizeof(int) + header.DataSize + header.BitmapSize;
    }
}


//================================================
// Traverse every record in table
// copy them into returnlist if check() == true
// not pass the test yet!!!!
//================================================
int RetrieveRecord(Buffer& buf, Table& t, CheckCondition& condition, vector<Record>& returnlist)
{
    returnlist.clear();
    Block* bptr = buf.Retrieve(t.BlockNumber);
    BlockHead head;
    head.Read(*bptr);

    while (1)
    {
        Offset RecordPtr;
        Record tuple;

        // traverse each record in one block
        for (RecordPtr = head.RecordPtr; RecordPtr != NULL_offset; RecordPtr = tuple.GetNext())
        {
            tuple.ReadBlock(*bptr, head, RecordPtr);
            if (check(tuple, condition)) {
                returnlist.push_back(tuple);
            }
        }// end of one block

        if (head.NextBlock != NULL_Block) // there are next block
        {
            bptr = buf.Retrieve(head.NextBlock);
            head.Read(*bptr);
            continue;
        }
        else    // no next block
        {
            break;
        }
    }// end of retrieve

    return returnlist.size();
}


//================================================
// Traverse every record in table
// delete them if check() == true
// block may be clear to save storage, but it isn't implemented yet
// not pass the test yet!!!!
//================================================
int DeleteRecord(Buffer& buf, Table& t, CheckCondition& condition)
{
    int ret = 0;
    Block* bptr = buf.Retrieve(t.BlockNumber);
    BlockHead head;
    head.Read(*bptr);

    while (1)
    {
        Offset RecordPtr, RecordPrev;
        Record tuple;

        // traverse each record in one block
        for (RecordPrev = NULL_offset, RecordPtr = head.RecordPtr; RecordPtr != NULL_offset; RecordPrev = RecordPtr, RecordPtr = tuple.GetNext())
        {
            tuple.ReadBlock(*bptr, head, RecordPtr);
            if (check(tuple, condition))
            {
                ret++;
                // delete this record
                if (RecordPtr == head.RecordPtr) // if head
                {
                    head.RecordPtr = bptr->ReadInt(RecordPtr);
                }
                else    // not head
                {
                    bptr->WriteInt(RecordPrev, bptr->ReadInt(RecordPtr));  // prev->next = ptr->next
                }
                bptr->WriteInt(RecordPtr, head.EmptyPtr);
                head.EmptyPtr = RecordPtr;
                head.FreeRecordNum++;
                head.RecordNum--;
            }
        }// end of one block
        head.Write(*bptr);
        bptr->DirtyBit = true;

        if (head.NextBlock != NULL_Block) // there are next block
        {
            bptr = buf.Retrieve(head.NextBlock);
            head.Read(*bptr);
            continue;
        }
        else    // no next block
        {
            break;
        }
    }// end of retrieve

    // here delete empty block
    return ret;
}





