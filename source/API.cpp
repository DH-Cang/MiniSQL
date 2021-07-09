// 2021/06/22/11:09

#include "API.h"

#pragma warning(disable:4996);

void OneInsertValue::SetValue(int t, int i, int n) {
	type = t;
	intvalue = i;
	ifnull = n;
}

void OneInsertValue::SetValue(int t, float f, int n) {
	type = t;
	floatvalue = f;
	ifnull = n;
}

void OneInsertValue::SetValue(int t, string s, int n) {
	type = t;
	charvalue = s;
	ifnull = n;
}

void OneInsertValue::ShowOneValue() {
	if (type == -1) {
		if (ifnull == 0) {
			cout << "int" << " " << intvalue << endl;
		}
		else {
			cout << "int" << " " << "null" << endl;
		}
	}
	else if (type == 0) {
		if (ifnull == 0) {
			cout << "float" << " " << floatvalue << endl;
		}
		else {
			cout << "float" << " " << "null" << endl;
		}
	}
	else {
		if (ifnull == 0) {
			cout << "string" << " " << charvalue << endl;
		}
		else {
			cout << "string" << " " << "null" << endl;
		}
	}
}

void AllInsertValue::AddValue(OneInsertValue& o) {
	InsertValue.push_back(o);
	return;
}

void AllInsertValue::ShowValue() {
	InsertTable.ShowTable();
	for (int i = 0; i < InsertValue.size(); i++) {
		InsertValue[i].ShowOneValue();
	}
}

void AllInsertValue::ValueToRecord(Buffer& buf, void* nullbit, void* re, int* datalength, int* bitlength) {
	int mallocsize = *datalength;
	int bitnum = *bitlength;
	/*
	bitnum = InsertValue.size();
	if (bitnum % 8 == 0) {
		bitnum = bitnum / 8;
	}
	else {
		bitnum = bitnum / 8 + 1;
	}
	nullbit = malloc(bitnum);
	for (int i = 0; i < bitnum; i++) {
		*((char*)nullbit + i) = 0;
	}
	*/
	for (int i = 0; i < InsertValue.size(); i++) {
		/*
		if (InsertValue[i].type == -1 || InsertValue[i].type == 0) {
			mallocsize = mallocsize + 4;
		}
		else {
			mallocsize = mallocsize + 256;
		}
		*/
		if (InsertValue[i].ifnull == 1) {
			*((char*)nullbit + (i / 8)) = *((char*)nullbit + (i / 8)) << 1;
			*((char*)nullbit) |= 1;
		}
		else {
			*((char*)nullbit + (i / 8)) = *((char*)nullbit + (i / 8)) << 1;
			*((char*)nullbit) |= 0;
		}
		//cout << *((char*)nullbit) << "coutnullbit" << endl;
	}
	if (InsertValue.size() % 8 != 0) {
		int shift = 8 - (InsertValue.size() % 8);
		while (shift > 0) {
			//cout << *((char*)nullbit) << "coutnullbit" << endl;
			*((char*)nullbit + (InsertValue.size() / 8)) = *((char*)nullbit + (InsertValue.size() / 8)) << 1;
			shift--;
		}
	}
	/*
	re = malloc(mallocsize);
	if (re == NULL) {
		throw("no space");
	}
	*/
	int nowoff = 0;
	for (int i = 0; i < InsertValue.size(); i++) {
		if (InsertValue[i].type == -1) {
			//int
			void* temp = malloc(4);
			if (temp == NULL) {
				throw("no space");
			}
			if (InsertValue[i].ifnull == 1) {
				int z = 0;
				memcpy((char*)temp, &z, sizeof(temp));
			}
			else {
				int testint;
				//cout << InsertValue[i].intvalue << endl;
				memcpy((char*)temp, &InsertValue[i].intvalue, sizeof(temp));
				memcpy(&testint, temp, sizeof(temp));
				//cout << testint << endl;
			}
			for (int j = 0; j < 4; j++) {
				*((char*)(re)+nowoff + j) = *((char*)(temp)+j);
			}
			nowoff = nowoff + 4;

			free(temp);// modified by cdh 6/27
		}
		else if (InsertValue[i].type == 0) {
			//float
			void* temp = malloc(4);
			if (temp == NULL) {
				throw("no space");
			}
			if (InsertValue[i].ifnull == 1) {
				float f = 0;
				memcpy((char*)temp, &f, sizeof(temp));
			}
			else {
				//float x;
				//sprintf((char*)temp, "%f", InsertValue[i].floatvalue);
				//cout << InsertValue[i].floatvalue << endl;
				memcpy((char*)temp, &InsertValue[i].floatvalue, sizeof(temp));
				//memcpy(&x, temp, sizeof(temp));
				//cout << setiosflags(ios::fixed) << x << endl;
			}
			for (int j = 0; j < 4; j++) {
				*((char*)(re)+nowoff + j) = *((char*)(temp)+j);
			}
			nowoff = nowoff + 4;

			free(temp);// modified by cdh 6/27
		}
		else {
			//string
			char* temp = (char*)malloc(256);
			if (InsertValue[i].ifnull == 1) {
				InsertValue[i].charvalue = "x";
			}
			//cout << InsertValue[i].charvalue << endl;
			string tempstr = InsertValue[i].charvalue;
			int tempi = 0;
			for (; tempi < tempstr.size(); tempi++) {
				temp[tempi] = tempstr[tempi];
			}
			temp[tempi] = '\0';

			for (int j = 0; j < 256; j++) {
				*((char*)(re)+nowoff + j) = *((char*)(temp)+j);
			}

			nowoff = nowoff + 256;

			free(temp);// modified by cdh 6/27
		}
	}
	*datalength = mallocsize;
	*bitlength = bitnum;

	return;
}

void API::APICreateTable(Table& TableA) {
	CatalogManager ToCreateTable;
	//ask a block
	//TableA.ShowTable();
	ToCreateTable.CreateTable(TableA);
	BindBlock(buf, TableA);
	ToCreateTable.UpdateTable(TableA);
	ToCreateTable.WriteTableIntoFile();
	for (int i = 0; i < TableA.AttributeNumber; i++) {
		if (TableA.AttributeInTable[i].IsPrimaryKey == 1) {
			//Index TempIndex(TableA.TableName,TableA.AttributeInTable[i].IndexName,TableA.AttributeInTable[i].AttributeName);
			//在这里调用index manager生成树
			//并得到blocknumber和blockoffset
			//int blocknumber;
			//int blockoffset;
			//TempIndex.SetIndexBlock(blocknumber,blockoffset);
			//ToCreateTable.CreateIndex(TempIndex);
			string indexname;
			indexname = TableA.TableName + "_" + TableA.AttributeInTable[i].AttributeName;
			Index temp(TableA.TableName, indexname, TableA.AttributeInTable[i].AttributeName);
			APICreateIndex(temp);
			ToCreateTable.GetIndexFromFile();
			ToCreateTable.GetTableFromFile();
			break;
		}
	}
	ToCreateTable.UpdateTable(TableA);
}

void API::APICreateIndex(Index& IndexA) {
	//先调用catalog得到Table信息，找到attributeoffset和blockoffset
	//setattributeoffset和blockoffset后
	//在这里调用index manager生成树
	//catalog将indexinfo写回
	//table更新索引信息
	int attributeoffset = 0;
	int attributetype = -2;
	CatalogManager CT;
	Table TT(IndexA.GetTableName());
	CT.FindTableByName(IndexA.GetTableName(), TT);
	for (int i = 0; i < TT.AttributeNumber; i++) {
		if (TT.AttributeInTable[i].AttributeName == IndexA.GetAttributeName()) {
			attributetype = TT.AttributeInTable[i].AttributeType;
			break;
		}
		if (TT.AttributeInTable[i].AttributeType == -1 || TT.AttributeInTable[i].AttributeType == 0) {
			attributeoffset = attributeoffset + 4;
		}
		else {
			attributeoffset = attributeoffset + 256;
		}
	}
	IndexA.SetIndexAttri(attributeoffset, attributetype);
	Create_index(IndexA, TT, buf);
	CT.CreateIndex(IndexA);
	CT.GetIndexFromFile();
	CT.GetTableFromFile();
	for (int i = 0; i < TT.AttributeNumber; i++) {
		if (TT.AttributeInTable[i].AttributeName == IndexA.AttributeName) {
			TT.AttributeInTable[i].IfHasIndex = 1;
			TT.AttributeInTable[i].IndexName = IndexA.IndexName;
			break;
		}
	}
	CT.UpdateTable(TT);
	//cout << "APIend" << endl;
}

void API::APIInsert(Table& InsertTable, void* data, void* bitmap, int datalength, int bitlength) {
	Record RecordInsert;
	//cout << "before readouter" << endl;
	RecordInsert.ReadOuter(bitmap, data, bitlength, datalength);
	RecordAccess ra;
	//cout << "before insertrecord" << endl;
	ra = InsertRecord(buf, InsertTable, RecordInsert);
	//cout << "after insertrecord" << endl;
	for (int i = 0; i < InsertTable.AttributeNumber; i++) {
		if (InsertTable.AttributeInTable[i].IfHasIndex == 1) {
			string indexname = InsertTable.AttributeInTable[i].IndexName;
			string tablename = InsertTable.TableName;
			string attriname = InsertTable.AttributeInTable[i].AttributeName;
			Index index_info(tablename, indexname, attriname);
			CatalogManager CT;
			CT.FindIndexByName(indexname, index_info);
			index_Insert_record(index_info, InsertTable, ra, buf);
			CT.UpdateIndex(index_info);
		}
	}
	return;
}

void API::APIDelete(Table& TableS, CheckCondition& DeleteCondition) {
	int i;
	int k = -1;
	int deletenumber = 0;
	for (i = 0; i < DeleteCondition.condition.size(); i++) {
		if (TableS.AttributeInTable[DeleteCondition.condition[i].CAttributeNum].IfHasIndex == 1) {
			break;
		}
	}
	if (i == DeleteCondition.condition.size() || DeleteCondition.condition[i].co == Onotequal) {
		//Record delete
		vector<Record> returnlist;
		vector<Index> rebuildindex;
		for (int j = 0; j < TableS.AttributeNumber; j++) {
			if (TableS.AttributeInTable[j].IfHasIndex == 1) {
				CatalogManager CT;
				Index indexdelete("1", "1", "1");
				CT.FindIndexByName(TableS.AttributeInTable[j].IndexName, indexdelete);
				rebuildindex.push_back(indexdelete);
				APIDropIndex(indexdelete);
			}
		}
		RetrieveRecord(buf, TableS, DeleteCondition, returnlist);
		deletenumber = DeleteRecord(buf, TableS, DeleteCondition);
		for (int j = 0; j < rebuildindex.size(); j++) {
			APICreateIndex(rebuildindex[j]);
		}
	}
	else {
		//Index delete
		int j;
		for (j = 0; j < DeleteCondition.condition.size(); j++) {
			if (i != j && DeleteCondition.condition[j].CAttributeNum == DeleteCondition.condition[i].CAttributeNum) {
				k = j;
			}
		}
		Value v;
		if (k == -1) {
			//one condition
			v.number = 1;
			if (DeleteCondition.condition[i].CAttributeType == -1) {
				//int
				v.int_value[0] = DeleteCondition.condition[i].Cint;
			}
			else if (DeleteCondition.condition[i].CAttributeNum == 0) {
				//float
				v.float_value[0] = DeleteCondition.condition[i].Cfloat;
			}
			else {
				//string
				v.string_value[0] = DeleteCondition.condition[i].Cchar.substr(1, DeleteCondition.condition[i].Cchar.size() - 2);
			}
			if (DeleteCondition.condition[i].co == Ogreater) {
				v.type[0] = -2;
			}
			else if (DeleteCondition.condition[i].co == Ogreater_equal) {
				v.type[0] = -1;
			}
			else if (DeleteCondition.condition[i].co == Oequal) {
				v.type[0] = 0;
			}
			else if (DeleteCondition.condition[i].co == Oless_equal) {
				v.type[0] = 1;
			}
			else if (DeleteCondition.condition[i].co == Oless) {
				v.type[0] = 2;
			}
		}
		else {
			//two condition
			v.number = 2;
			if (DeleteCondition.condition[i].CAttributeType == -1) {
				//int
				v.int_value[0] = DeleteCondition.condition[i].Cint;
			}
			else if (DeleteCondition.condition[i].CAttributeNum == 0) {
				//float
				v.float_value[0] = DeleteCondition.condition[i].Cfloat;
			}
			else {
				//string
				v.string_value[0] = DeleteCondition.condition[i].Cchar.substr(1, DeleteCondition.condition[i].Cchar.size() - 2);
			}
			if (DeleteCondition.condition[i].co == Ogreater) {
				v.type[0] = -2;
			}
			else if (DeleteCondition.condition[i].co == Ogreater_equal) {
				v.type[0] = -1;
			}
			else if (DeleteCondition.condition[i].co == Oequal) {
				v.type[0] = 0;
			}
			else if (DeleteCondition.condition[i].co == Oless_equal) {
				v.type[0] = 1;
			}
			else if (DeleteCondition.condition[i].co == Oless) {
				v.type[0] = 2;
			}

			if (DeleteCondition.condition[k].CAttributeType == -1) {
				//int
				v.int_value[1] = DeleteCondition.condition[k].Cint;
			}
			else if (DeleteCondition.condition[k].CAttributeNum == 0) {
				//float
				v.float_value[1] = DeleteCondition.condition[k].Cfloat;
			}
			else {
				//string
				v.string_value[1] = DeleteCondition.condition[k].Cchar.substr(1, DeleteCondition.condition[k].Cchar.size() - 2);
			}
			if (DeleteCondition.condition[k].co == Ogreater) {
				v.type[1] = -2;
			}
			else if (DeleteCondition.condition[k].co == Ogreater_equal) {
				v.type[1] = -1;
			}
			else if (DeleteCondition.condition[k].co == Oequal) {
				v.type[1] = 0;
			}
			else if (DeleteCondition.condition[k].co == Oless_equal) {
				v.type[1] = 1;
			}
			else if (DeleteCondition.condition[k].co == Oless) {
				v.type[1] = 2;
			}
		}
		string name = TableS.AttributeInTable[DeleteCondition.condition[i].CAttributeNum].IndexName;
		string attri = TableS.AttributeInTable[DeleteCondition.condition[i].CAttributeNum].AttributeName;
		Index index_info(TableS.TableName, name, attri);

		CatalogManager CT;
		CT.FindIndexByName(name, index_info);

		//delete
		deletenumber = index_Delete_record(index_info, TableS, v, buf);
		CT.UpdateIndex(index_info);

		for (int u = 0; u < TableS.AttributeNumber; u++) {
			if (TableS.AttributeInTable[u].IfHasIndex == 1 && TableS.AttributeInTable[u].IndexName != index_info.IndexName) {
				Index TempIndex(",", ",", ",");
				CT.FindIndexByName(TableS.AttributeInTable[u].IndexName, TempIndex);
				APIDropIndex(TempIndex);
				APICreateIndex(TempIndex);
			}
		}
	}
	cout << deletenumber << "rows deleted" << endl;
}

void API::APISelect(Table& TableS, CheckCondition& SelectCondition, int* SelectAttri) {
	int i;
	int k = -1;
	for (i = 0; i < SelectCondition.condition.size(); i++) {
		if (TableS.AttributeInTable[SelectCondition.condition[i].CAttributeNum].IfHasIndex == 1) {
			break;
		}
	}
	if (i == SelectCondition.condition.size() || SelectCondition.condition[i].co == Onotequal) {
		//Record select
		vector<Record> returnlist;
		RetrieveRecord(buf, TableS, SelectCondition, returnlist);
		DrawTable(TableS, returnlist, SelectAttri);
		return;
	}
	else {
		//Index select
		int j;
		for (j = 0; j < SelectCondition.condition.size(); j++) {
			if (i != j && SelectCondition.condition[j].CAttributeNum == SelectCondition.condition[i].CAttributeNum) {
				k = j;
			}
		}
		Value v;
		if (k == -1) {
			//one condition
			v.number = 1;
			if (SelectCondition.condition[i].CAttributeType == -1) {
				//int
				v.int_value[0] = SelectCondition.condition[i].Cint;
			}
			else if (SelectCondition.condition[i].CAttributeType == 0) {
				//float
				v.float_value[0] = SelectCondition.condition[i].Cfloat;
			}
			else {
				//string
				v.string_value[0] = SelectCondition.condition[i].Cchar.substr(1, SelectCondition.condition[i].Cchar.size() - 2);
			}
			if (SelectCondition.condition[i].co == Ogreater) {
				v.type[0] = -2;
			}
			else if (SelectCondition.condition[i].co == Ogreater_equal) {
				v.type[0] = -1;
			}
			else if (SelectCondition.condition[i].co == Oequal) {
				v.type[0] = 0;
			}
			else if (SelectCondition.condition[i].co == Oless_equal) {
				v.type[0] = 1;
			}
			else if (SelectCondition.condition[i].co == Oless) {
				v.type[0] = 2;
			}
		}
		else {
			//two condition
			v.number = 2;
			if (SelectCondition.condition[i].CAttributeType == -1) {
				//int
				v.int_value[0] = SelectCondition.condition[i].Cint;
			}
			else if (SelectCondition.condition[i].CAttributeType == 0) {
				//float
				v.float_value[0] = SelectCondition.condition[i].Cfloat;
			}
			else {
				//string
				v.string_value[0] = SelectCondition.condition[i].Cchar.substr(1, SelectCondition.condition[i].Cchar.size() - 2);
			}
			if (SelectCondition.condition[i].co == Ogreater) {
				v.type[0] = -2;
			}
			else if (SelectCondition.condition[i].co == Ogreater_equal) {
				v.type[0] = -1;
			}
			else if (SelectCondition.condition[i].co == Oequal) {
				v.type[0] = 0;
			}
			else if (SelectCondition.condition[i].co == Oless_equal) {
				v.type[0] = 1;
			}
			else if (SelectCondition.condition[i].co == Oless) {
				v.type[0] = 2;
			}

			if (SelectCondition.condition[k].CAttributeType == -1) {
				//int
				v.int_value[1] = SelectCondition.condition[k].Cint;
			}
			else if (SelectCondition.condition[k].CAttributeType == 0) {
				//float
				v.float_value[1] = SelectCondition.condition[k].Cfloat;
			}
			else {
				//string
				v.string_value[1] = SelectCondition.condition[k].Cchar.substr(1, SelectCondition.condition[k].Cchar.size() - 2);
			}
			if (SelectCondition.condition[k].co == Ogreater) {
				v.type[1] = -2;
			}
			else if (SelectCondition.condition[k].co == Ogreater_equal) {
				v.type[1] = -1;
			}
			else if (SelectCondition.condition[k].co == Oequal) {
				v.type[1] = 0;
			}
			else if (SelectCondition.condition[k].co == Oless_equal) {
				v.type[1] = 1;
			}
			else if (SelectCondition.condition[k].co == Oless) {
				v.type[1] = 2;
			}
		}
		string name = TableS.AttributeInTable[SelectCondition.condition[i].CAttributeNum].IndexName;
		string attri = TableS.AttributeInTable[SelectCondition.condition[i].CAttributeNum].AttributeName;
		Index index_info(TableS.TableName, name, attri);

		CatalogManager CT;
		CT.FindIndexByName(name, index_info);

		index_Find(index_info, v, buf);
		CT.UpdateIndex(index_info);

		bool flag;
		for (int x = 0; x < v.search_result.size();) {
			flag = check(v.search_result[x], SelectCondition);
			if (flag == false) {
				v.search_result[x] = v.search_result[v.search_result.size() - 1];
				v.search_result.pop_back();
			}
			else {
				x++;
			}
		}
		DrawTable(TableS, v.search_result, SelectAttri);
		cout << v.search_result.size() << "lines record returned" << endl;
	}
}

void API::APIDropTable(Table& TableD) {
	CatalogManager CT;
	for (int i = 0; i < TableD.AttributeNumber; i++) {
		if (TableD.AttributeInTable[i].IfHasIndex == 1) {
			string in = TableD.AttributeInTable[i].IndexName;
			string tn = TableD.TableName;
			string an = TableD.AttributeInTable[i].AttributeName;
			Index IndexD(tn, in, an);
			CT.FindIndexByName(in, IndexD);
			Drop_index(IndexD, buf);
		}
	}
	DeBind(buf, TableD);
	CT.DropIndex(TableD.TableName, 0);
	CT.DropTable(TableD.TableName);
}

void API::APIDropIndex(Index& IndexD) {
	Drop_index(IndexD, buf);
	CatalogManager CT;
	CT.DropIndex(IndexD.IndexName, 1);
}

int API::APIInsertSelect(Table& TableS, CheckCondition& SelectCondition) {
	int i;
	int k = -1;
	for (i = 0; i < SelectCondition.condition.size(); i++) {
		if (TableS.AttributeInTable[SelectCondition.condition[i].CAttributeNum].IfHasIndex == 1) {
			break;
		}
	}
	if (i == SelectCondition.condition.size() || SelectCondition.condition[i].co == Onotequal) {
		//Record select
		vector<Record> returnlist;
		RetrieveRecord(buf, TableS, SelectCondition, returnlist);
		return returnlist.size();
	}
	else {
		//Index select
		int j;
		for (j = 0; j < SelectCondition.condition.size(); j++) {
			if (i != j && SelectCondition.condition[j].CAttributeNum == SelectCondition.condition[i].CAttributeNum) {
				k = j;
			}
		}
		Value v;
		if (k == -1) {
			//one condition
			v.number = 1;
			if (SelectCondition.condition[i].CAttributeType == -1) {
				//int
				v.int_value[0] = SelectCondition.condition[i].Cint;
			}
			else if (SelectCondition.condition[i].CAttributeType == 0) {
				//float
				v.float_value[0] = SelectCondition.condition[i].Cfloat;
			}
			else if (SelectCondition.condition[i].CAttributeType > 0) {
				//string
				v.string_value[0] = SelectCondition.condition[i].Cchar;
			}
			if (SelectCondition.condition[i].co == Ogreater) {
				v.type[0] = -2;
			}
			else if (SelectCondition.condition[i].co == Ogreater_equal) {
				v.type[0] = -1;
			}
			else if (SelectCondition.condition[i].co == Oequal) {
				v.type[0] = 0;
			}
			else if (SelectCondition.condition[i].co == Oless_equal) {
				v.type[0] = 1;
			}
			else if (SelectCondition.condition[i].co == Oless) {
				v.type[0] = 2;
			}
		}
		else {
			//two condition
			v.number = 2;
			if (SelectCondition.condition[i].CAttributeType == -1) {
				//int
				v.int_value[0] = SelectCondition.condition[i].Cint;
			}
			else if (SelectCondition.condition[i].CAttributeType == 0) {
				//float
				v.float_value[0] = SelectCondition.condition[i].Cfloat;
			}
			else {
				//string
				v.string_value[0] = SelectCondition.condition[i].Cchar;
			}
			if (SelectCondition.condition[i].co == Ogreater) {
				v.type[0] = -2;
			}
			else if (SelectCondition.condition[i].co == Ogreater_equal) {
				v.type[0] = -1;
			}
			else if (SelectCondition.condition[i].co == Oequal) {
				v.type[0] = 0;
			}
			else if (SelectCondition.condition[i].co == Oless_equal) {
				v.type[0] = 1;
			}
			else if (SelectCondition.condition[i].co == Oless) {
				v.type[0] = 2;
			}

			if (SelectCondition.condition[k].CAttributeType == -1) {
				//int
				v.int_value[1] = SelectCondition.condition[k].Cint;
			}
			else if (SelectCondition.condition[k].CAttributeType == 0) {
				//float
				v.float_value[1] = SelectCondition.condition[k].Cfloat;
			}
			else {
				//string
				v.string_value[1] = SelectCondition.condition[k].Cchar;
			}
			if (SelectCondition.condition[k].co == Ogreater) {
				v.type[1] = -2;
			}
			else if (SelectCondition.condition[k].co == Ogreater_equal) {
				v.type[1] = -1;
			}
			else if (SelectCondition.condition[k].co == Oequal) {
				v.type[1] = 0;
			}
			else if (SelectCondition.condition[k].co == Oless_equal) {
				v.type[1] = 1;
			}
			else if (SelectCondition.condition[k].co == Oless) {
				v.type[1] = 2;
			}
		}
		string name = TableS.AttributeInTable[SelectCondition.condition[i].CAttributeNum].IndexName;
		string attri = TableS.AttributeInTable[SelectCondition.condition[i].CAttributeNum].AttributeName;
		Index index_info(TableS.TableName, name, attri);
		CatalogManager CT;
		CT.FindIndexByName(name, index_info);

		index_Find(index_info, v, buf);
		CT.UpdateIndex(index_info);

		bool flag;
		for (int x = 0; x < v.search_result.size(); x++) {
			flag = check(v.search_result[x], SelectCondition);
			if (flag == false) {
				v.search_result[v.search_result.size() - 1] = v.search_result[x];
				v.search_result.pop_back();
			}
		}
		return v.search_result.size();
	}
}