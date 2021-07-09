//2021/06/22/11:11

#ifndef _API_H_
#define _API_H_

#include "RecordManager.h"
#include "CatalogManager.h"
#include "IndexManager.h"



class OneInsertValue {
public:
	int type; // -1 = int, 0 = float, 1~255 = char*
	int ifnull; // 0 = not null, 1 = null
	int intvalue;
	float floatvalue;
	string charvalue;

	OneInsertValue() {
		type = -3;
		ifnull = 1;
		intvalue = 0;
		floatvalue = 0;
		charvalue = "";
	}
	void SetValue(int t, int i, int n);
	void SetValue(int t, float f, int n);
	void SetValue(int t, string s, int n);
	void ShowOneValue();
	friend class AllInsertValue;
};

class AllInsertValue {
public:
	Table InsertTable;
	vector<OneInsertValue> InsertValue;

	AllInsertValue(Table& t) {
		InsertTable = t;
		InsertValue.clear();
	}
	void clear() {
		InsertValue.clear();
	}
	void AddValue(OneInsertValue& o);
	void ShowValue();
	void ValueToRecord(Buffer& buf, void* nullbit, void* re, int* datalength, int* bitlength);
};


class API {
public:
	Buffer buf;

	API() {}
	~API() {}
	void APICreateTable(Table& TableA);
	void APICreateIndex(Index& IndexA);
	void APIInsert(Table& InsertTable, void* data, void* bitmap, int datalength, int bitlength);
	void APIDelete(Table& TableS, CheckCondition& DeleteCondition);
	void APISelect(Table& TableS, CheckCondition& SelectCondition, int* SelectAttri);
	void APIDropTable(Table& TableD);
	void APIDropIndex(Index& IndexD);
	int APIInsertSelect(Table& TableS, CheckCondition& SelectCondition);
};




#endif