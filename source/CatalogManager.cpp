// 2021/06/22/11:09

#include "CatalogManager.h"

string Index::GetIndexName() {
	return IndexName;
}

string Index::GetTableName() {
	return TableName;
}

string Index::GetAttributeName() {
	return AttributeName;
}

void Index::SetIndexAttri(int offset, int type) {
	AttributeOffset = offset;
	AttributeType = type;
}

void CatalogManager::FindIndexByName(string name, Index& returnindex) {
	for (int i = 0; i < IndexesNumber; i++) {
		if (indexes[i].IndexName == name) {
			returnindex = indexes[i];
			return;
		}
	}
	throw("No such Index");
	return;
}

void CatalogManager::ShowTables() {
	cout << TablesNumber << endl;
	for (int i = 0; i < TablesNumber; i++) {
		tables[i].ShowTable();
	}
	return;
}

void CatalogManager::GetTableFromFile() {
	const string FileName = "MiniSQL_TableInfo.dat";
	fstream CatalogFile(FileName, ios::in | ios::binary);
	if (!CatalogFile.fail()) {
		CatalogFile >> TablesNumber;
		string TableName;
		for (int i = 0; i < TablesNumber; i++) {
			CatalogFile >> TableName;
			//cout << TableName << endl;
			Table TempTable(TableName);
			CatalogFile >> TempTable.AttributeNumber;
			CatalogFile >> TempTable.BlockNumber;
			//cout << TempTable.AttributeNumber << endl;
			//cout << TempTable.BlockNumber << endl;
			for (int j = 0; j < TempTable.AttributeNumber; j++) {
				string AttributeName;
				CatalogFile >> AttributeName;
				Attribute TempAttribute(AttributeName);
				CatalogFile >> TempAttribute.AttributeType;
				CatalogFile >> TempAttribute.IsUnique;
				CatalogFile >> TempAttribute.IsPrimaryKey;
				CatalogFile >> TempAttribute.IfHasIndex;
				CatalogFile >> TempAttribute.IndexName;
				TempTable.AttributeInTable.push_back(TempAttribute);
			}
			tables.push_back(TempTable);
		}
		//ShowTables();
		CatalogFile.close();
	}
	else {
		TablesNumber = 0;
	}
	return;
}

void CatalogManager::GetIndexFromFile() {
	string FileName = "MiniSQL_IndexInfo.dat";
	fstream CatalogFile(FileName, ios::in | ios::binary);
	if (!CatalogFile.fail()) {
		CatalogFile >> IndexesNumber;
		string tn;
		string in;
		string an;
		int bn;
		int ao;
		int at;
		for (int i = 0; i < IndexesNumber; i++) {
			CatalogFile >> tn;
			CatalogFile >> in;
			CatalogFile >> an;
			Index intemp(tn, in, an);
			CatalogFile >> bn;
			CatalogFile >> ao;
			CatalogFile >> at;
			intemp.BlockNumber = bn;
			intemp.SetIndexAttri(ao, at);
			indexes.push_back(intemp);
		}
		CatalogFile.close();
	}
	else {
		IndexesNumber = 0;
	}
	return;
}

void CatalogManager::WriteTableIntoFile() {
	string FileName = "MiniSQL_TableInfo.dat";
	fstream CatalogFile(FileName, ios::out | ios::binary);
	CatalogFile << TablesNumber << endl;
	for (int i = 0; i < TablesNumber; i++) {
		CatalogFile << tables[i].TableName << " ";
		CatalogFile << tables[i].AttributeNumber << " ";
		CatalogFile << tables[i].BlockNumber << endl;
		for (int j = 0; j < tables[i].AttributeNumber; j++) {
			CatalogFile << tables[i].AttributeInTable[j].AttributeName << " ";
			CatalogFile << tables[i].AttributeInTable[j].AttributeType << " ";
			CatalogFile << tables[i].AttributeInTable[j].IsUnique << " ";
			CatalogFile << tables[i].AttributeInTable[j].IsPrimaryKey << " ";
			CatalogFile << tables[i].AttributeInTable[j].IfHasIndex << " ";
			CatalogFile << tables[i].AttributeInTable[j].IndexName << endl;
		}
	}
	CatalogFile.close();
	return;
}

void CatalogManager::WriteIndexIntoFile() {
	string FileName = "MiniSQL_IndexInfo.dat";
	fstream CatalogFile(FileName, ios::out | ios::binary);
	CatalogFile << IndexesNumber << endl;
	for (int i = 0; i < IndexesNumber; i++) {
		CatalogFile << indexes[i].TableName << " ";
		CatalogFile << indexes[i].IndexName << " ";
		CatalogFile << indexes[i].AttributeName << " ";
		CatalogFile << indexes[i].BlockNumber << " ";
		CatalogFile << indexes[i].AttributeOffset << " ";
		CatalogFile << indexes[i].AttributeType << endl;
	}
	CatalogFile.close();
	return;
}

void CatalogManager::CreateTable(Table& CreateTheTable) {
	for (int i = 0; i < TablesNumber; i++) {
		if (tables[i].TableName == CreateTheTable.TableName) {
			//debind record?
			throw("Duplicate Table Name :(");
		}
	}
	tables.push_back(CreateTheTable);
	TablesNumber = tables.size();
	return;
}

void CatalogManager::CreateIndex(Index& CreateTheIndex) {
	for (int i = 0; i < IndexesNumber; i++) {
		if (indexes[i].GetIndexName() == CreateTheIndex.GetIndexName()) {
			throw("Duplicate Index Name :(");
		}
		if ((indexes[i].GetTableName() == CreateTheIndex.GetTableName()) && (indexes[i].GetAttributeName() == CreateTheIndex.GetAttributeName())) {
			throw("The Index Already Exists :(");
		}
	}
	for (int i = 0; i < TablesNumber; i++) {
		if (tables[i].TableName == CreateTheIndex.GetTableName()) {
			for (int j = 0; j < tables[i].AttributeNumber; j++) {
				if (tables[i].AttributeInTable[j].AttributeName == CreateTheIndex.GetAttributeName()) {
					if (tables[i].AttributeInTable[j].IsUnique == 0) {
						//tables[i].ShowTable();
						//cout << tables[i].AttributeInTable[j].IsUnique << endl;
						throw("This Attribute is not unique, you can only create index on unique attribute");
					}
					/*
					if (tables[i].AttributeInTable[j].IfHasIndex == 1) {
						throw("The Index Already Exists :(");
					}
					*/
					else {
						tables[i].AttributeInTable[j].IfHasIndex = 1;
						tables[i].AttributeInTable[j].IndexName = CreateTheIndex.GetIndexName();
					}
				}
			}
		}
	}
	indexes.push_back(CreateTheIndex);
	IndexesNumber = indexes.size();

	WriteIndexIntoFile();

	return;
}

void CatalogManager::DropTable(string DropTableName) {
	//删数据先（recordmanager -> debind
	//↑在API做
	DropIndex(DropTableName, 0);
	for (int i = 0; i < TablesNumber; i++) {
		if (tables[i].TableName == DropTableName) {
			tables[i] = tables.back();
			tables.pop_back();
			TablesNumber--;
			break;
		}
	}
	WriteIndexIntoFile();
	WriteTableIntoFile();
}

//if TableOrIndex==0, drop all the indexes of this table,if ==1, just drop one index
void CatalogManager::DropIndex(string DropName, int TableOrIndex) {
	//要清空存放Index的block
	//↑在API做了
	if (TableOrIndex == 0) {
		for (int i = 0; i < IndexesNumber; i++) {
			if (indexes[i].GetTableName() == DropName) {
				indexes[i] = indexes.back();
				indexes.pop_back();
				IndexesNumber--;
			}
		}
	}
	else if (TableOrIndex == 1) {
		string IndexTable;
		string IndexAttribute;
		for (int i = 0; i < IndexesNumber; i++) {
			if (indexes[i].GetIndexName() == DropName) {
				IndexTable = indexes[i].GetTableName();
				IndexAttribute = indexes[i].GetAttributeName();
				indexes[i] = indexes.back();
				indexes.pop_back();
				IndexesNumber--;
			}
		}
		for (int i = 0; i < TablesNumber; i++) {
			if (tables[i].TableName == IndexTable) {
				for (int j = 0; j < tables[i].AttributeNumber; j++) {
					if (tables[i].AttributeInTable[j].AttributeName == IndexAttribute) {
						tables[i].AttributeInTable[j].IfHasIndex = 0;
						tables[i].AttributeInTable[j].IndexName = "@@";
					}
				}
				UpdateTable(tables[i]);
			}
		}
	}
	WriteIndexIntoFile();
	WriteTableIntoFile();
}

void CatalogManager::FindTableByName(string tn, Table& tb) {
	for (int i = 0; i < TablesNumber; i++) {
		if (tables[i].TableName == tn) {
			tb = tables[i];
			return;
		}
	}
	throw("No such Table");
	return;
}

void CatalogManager::UpdateTable(Table& t) {
	for (int i = 0; i < TablesNumber; i++) {
		if (tables[i].TableName == t.TableName) {
			tables[i] = t;
			break;
		}
	}
	WriteTableIntoFile();
}

void CatalogManager::UpdateIndex(Index& in) {
	for (int i = 0; i < IndexesNumber; i++) {
		if (indexes[i].IndexName == in.IndexName) {
			indexes[i] = in;
			break;
		}
	}
	WriteIndexIntoFile();
}

void Table::AddAttribute(Attribute& a) {
	for (int i = 0; i < AttributeNumber; i++) {
		if (a.AttributeName == AttributeInTable[i].AttributeName) {
			throw("Duplicate attribute name");
		}
	}
	AttributeInTable.push_back(a);
	AttributeNumber++;
	return;
}

void Table::AddIndex(Index& index) {
	for (int i = 0; i < AttributeNumber; i++) {
		if (index.AttributeName == AttributeInTable[i].AttributeName) {
			AttributeInTable[i].IfHasIndex = 1;
			AttributeInTable[i].IndexName = index.IndexName;
			break;
		}
	}
}

void Table::ShowTable() {
	cout << TableName << endl;
	for (int i = 0; i < AttributeNumber; i++) {
		cout << AttributeInTable[i].AttributeName << " ";
		cout << AttributeInTable[i].AttributeType << " ";
		cout << AttributeInTable[i].IsUnique << " ";
		cout << AttributeInTable[i].IsPrimaryKey << endl;
	}
}

void Attribute::SetAttribute(int type, int unique, int primarykey, int hasindex, string indexname) {
	AttributeType = type;
	IsUnique = unique;
	IsPrimaryKey = primarykey;
	IfHasIndex = hasindex;
	if (IfHasIndex == 1) {
		IndexName = indexname;
	}
	else {
		IndexName = "@@";
	}
}

void CheckCondition::AddCondition(Condition& c) {
	condition.push_back(c);
	return;
}

Condition::Condition(ConditionOperator o, int can, int cat, int ci, float cf, string cc) {
	co = o;
	CAttributeNum = can;
	CAttributeType = cat;
	Cint = ci;
	Cfloat = cf;
	Cchar = cc;
}

void Condition::ShowCondition() {
	cout << "Attribute No." << CAttributeNum << endl;
	cout << "Attribute Type:" << CAttributeType << endl;
	cout << "Operator is:" << co << endl;
	if (CAttributeType == -1) {
		cout << "is int:" << Cint << endl;
	}
	else if (CAttributeType == 0) {
		cout << "is float:" << Cfloat << endl;
	}
	else {
		cout << "is char*:" << Cchar << endl;
	}
	return;
}

void CheckCondition::ShowCheckCondition() {
	for (int i = 0; i < condition.size(); i++) {
		condition[i].ShowCondition();
	}
	return;
}