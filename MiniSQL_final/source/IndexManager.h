//�汾3 2.21/6/20 20:00

//��ΰ汾�ֲ���֮ǰ�޷�����Χ���ҵ����⣬ʹ��ͳһ��find�������з�Χ���ң��������ؽ��ֱ����ʾ��
//Value���е�vector�Ӷ�ʵ�ֽ���Ĵ���||����||
//delete ҲҪ��Χɾ������������ӿڿ������ǿ��Եĵ�����ʵ�����������鷳һЩ����Ϊ��Χɾ������
//����index�洢�ṹ�����ϴ�ı�,����������Ҫ��createindex��ʱ��ͬ����begin_record�����ҽ���
//index����������¹���

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


/*�ⲿ����RecordManager���棬��������
class RecordAccess{
    public:
        BlockID bid;
        Offset offset;

        Record Read(Buffer& buf);
        // another delete function
};
*/
/*�ⲿ����CatalogManager������֣��������
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
struct Value {//֧��������������5�ֱȽϷ�ʽ
public:
    vector<Record> search_result;
    int int_value[2];
    float float_value[2];//�����ֻ��Ҫ����Ч�����ݽ��и�ֵ��������ֵ��Ӱ��
    string string_value[2];//�һ����index_info�����ж�ȡֵ
    int type[2];//-2 > ; -1 >= ; 0 == ; 1 <= ; 2 < ;
    int number;//������������
};


//ͨ�������������Ϣ�������������Զ������ݲ��뵽����,
//begin_record��ֵ�Ǹô洢��¼��һ��Ԫ�صĴ洢��ַ
void Create_index(Index& index_info, Table& table, Buffer& buffer1);


void index_Insert_record(Index& index_info, Table& table, RecordAccess new_record, Buffer& buffer1);

//�ҵ�ĳ����Ϣ��Ҷ�ӽڵ�
void index_Find(Index& index_info,  Value& find_value , Buffer& buffer1 );

//ɾ��ĳ����Ϣǰɾ��index
int index_Delete_record(Index& index_info, Table& table, Value& delete_value, Buffer& buffer1);

//��index��blockȫ���ӵ�
void Drop_index(Index& index_info, Buffer& buffer1);


//update��ʱ��ֻ��Ҫ��ɾ�������

#endif