//版本3 2.21/6/20 20:00

//这次版本弥补了之前无法做范围查找的问题，使用统一的find函数进行范围查找，并将返回结果直接显示到
//Value类中的vector从而实现结果的传递||引用||
//delete 也要范围删除吗，这个函数接口看起来是可以的但是这实现起来可能麻烦一些，因为范围删除可能
//引起index存储结构发生较大改变,所以这里需要和createindex的时候同样的begin_record方便我进行
//index的整体的重新构造

#ifndef INDEX_FLAG
#define INDEX_FLAG

#include <iterator>
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include <string>
#include "BufferManager.h"
#include "CatalogManager.h"
#include "RecordManager.h"
using namespace std;


/*这部分在RecordManager里面，这里会调用
class RecordAccess{
    public:
        BlockID bid;
        Offset offset;

        Record Read(Buffer& buf);
        // another delete function
};
*/
/*这部分在CatalogManager里面出现，这里调用
class Index{
private:
    string TableName;
    string IndexName;
    string AttributeName;
    int BlockNumber;
    int BlockOffset;
public:
    Index(string a, string b, string c){
        TableName = a;
        IndexName = b;
        AttributeName = c;
        BlockNumber = -1;
        BlockOffset = -1;
    }
    ~Index(){}
    void SetIndexBlock(int blocknum, int offset);
    string GetIndexName();
    string GetTableName();
    string GetAttributeName();
};
*/
struct Value {//支持两个操作数，5种比较方式
public:
    vector<Record> search_result;
    int int_value[2];
    float float_value[2];//这个你只需要对有效地数据进行赋值，其他赋值不影响
    string string_value[2];//我会根据index_info进行判断取值
    int type[2];//-2 > ; -1 >= ; 0 == ; 1 <= ; 2 < ;
    int number;//操作数的数量
};


//通过传入参数的信息建立索引并且自动将数据插入到索引,
//begin_record的值是该存储记录第一个元素的存储地址
void Create_index(Index& index_info, Table& table, Buffer& buffer1);


void index_Insert_record(Index& index_info, Table& table, RecordAccess new_record, Buffer& buffer1);

//找到某个信息的叶子节点
void index_Find(Index& index_info,  Value& find_value , Buffer& buffer1 );

//删除某个信息前删除index
int index_Delete_record(Index& index_info, Table& table, Value& delete_value, Buffer& buffer1);

//将index的block全部扔掉
void Drop_index(Index& index_info, Buffer& buffer1);


//update的时候只需要先删除再添加

#endif