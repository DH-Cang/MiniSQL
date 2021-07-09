// 2021/06/22/11:10

#ifndef _CATALOGMANAGER_H_
#define _CATALOGMANAGER_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;


class Attribute {
public:
	string AttributeName;
	int AttributeType;      //int=-1,float=0,char=1~255,else -2
	int IsUnique;           //unique=1,not unique=0
	int IsPrimaryKey;       //is primary key = 1, else = 0
	int IfHasIndex;         //0 means no and 1 means yes
	string IndexName;       //"@@" means no index


	Attribute(string a) {
		AttributeName = a;
		AttributeType = -2;
		IsUnique = 0;
		IsPrimaryKey = 0;
		IfHasIndex = 0;
		IndexName = "";
	}
	void SetAttribute(int type, int unique, int primarykey, int hasindex, string indexname);
};

class Index {
public:
	string TableName;
	string IndexName;
	string AttributeName;
	int BlockNumber; //root
	int AttributeOffset;
	int AttributeType;

	Index(string tn, string in, string an) {
		TableName = tn;
		IndexName = in;
		AttributeName = an;
		BlockNumber = -1;
		AttributeOffset = -1;
		AttributeType = -2;
	}
	~Index() {}
	void SetIndexAttri(int offset, int type);
	string GetIndexName();
	string GetTableName();
	string GetAttributeName();
};


//============================================
// class Table used to create table
//============================================
class Table {
public:
	string TableName;
	int AttributeNumber;
	int BlockNumber; //the number of the block which the data of this table is stored in
	vector<Attribute> AttributeInTable;

	Table(string tn) {
		BlockNumber = -1; // -1 means not defined
		TableName = tn;
		AttributeNumber = 0;
		AttributeInTable.clear();
	}
	Table() {
		BlockNumber = -1; // -1 means not defined
		TableName = "";
		AttributeNumber = 0;
		AttributeInTable.clear();
	}
	void AddAttribute(Attribute& a);
	void ShowTable();
	void AddIndex(Index& index);
};

class CatalogManager {
private:
	vector<Table> tables;
	int TablesNumber;
	vector<Index> indexes;
	int IndexesNumber;
public:
	void UpdateTable(Table& t);
	void UpdateIndex(Index& in);
	void ShowTables();
	void FindTableByName(string tn, Table& tb);
	void FindIndexByName(string name, Index& returnindex);
	void GetTableFromFile();
	void GetIndexFromFile();
	void WriteTableIntoFile();
	void WriteIndexIntoFile();
	void CreateTable(Table& CreateTheTable);
	void CreateIndex(Index& CreateTheIndex);
	void DropTable(string DropTableName);
	void DropIndex(string DropName, int TableOrIndex); //if TableOrIndex==0, drop all the indexes of this table,if ==1, just drop one index
	CatalogManager() {
		tables.clear();
		indexes.clear();
		GetTableFromFile();
		GetIndexFromFile();
	}
	~CatalogManager() {
		//WriteTableIntoFile();
		//WriteIndexIntoFile();
	}
};


enum ConditionOperator { Oequal, Onotequal, Ogreater, Oless, Ogreater_equal, Oless_equal };

class Condition {
public:
	ConditionOperator co;       //condition operator
	int CAttributeNum;          //condition attribute number
	int CAttributeType;         //condition attribute type
	int Cint;                   //if is int
	float Cfloat;               //if is float
	string Cchar;               //if is char

	Condition(ConditionOperator o, int can, int cat, int ci, float cf, string cc);
	~Condition() {}
	void ShowCondition();
};

class CheckCondition {
public:
	bool ifall;
	vector<Condition> condition;
	Table ConditionTable;

	CheckCondition(Table& t) {
		ifall = false;
		ConditionTable = t;
		condition.clear();
	}
	~CheckCondition() {}
	void AddCondition(Condition& c);
	void ShowCheckCondition();
	void SetAll() {
		ifall = true;
	}
};

#endif