// 2021/06/22/11:09

#include "Interpreter.h"



//==========================================
// GetSql used to get input sql statement
//==========================================
void SqlStatement::GetSql(API& APIOnly) {
	string input;   //input string
	int InputFinish = -1;
	int FindTwoBlank = -1;
	int FindTxt = -1;
	cout << ">>>";
	getline(cin, input);
	InputStatement = InputStatement + input;
	InputFinish = InputStatement.find(";");
	FindTxt = InputStatement.find(".txt");
	while (InputFinish < 0) {
		if (FindTxt >= 0) {
			//cout << FindTxt << endl;
			// read from .txt file
			break;
		}
		cout << ">>>";
		getline(cin, input);
		InputStatement = InputStatement + " " + input;
		InputFinish = InputStatement.find(";");
		FindTxt = InputStatement.find(".txt");
	}
	if (FindTxt >= 0) {
		int FindTxtEnd = -1;
		string txttemp;
		// read from .txt file
		string filename = InputStatement.substr(9, InputFinish - 9);
		InputStatement.clear();
		ifstream readfile(filename);
		while (getline(readfile, InputStatement)) {
			FindTxtEnd = InputStatement.find(";");
			while (FindTxtEnd < 0) {
				getline(readfile, txttemp);
				InputStatement = InputStatement + " " + txttemp;
				FindTxtEnd = InputStatement.find(";");
			}
			//救命 大小写怎么处理啊:( 可以直接这样的吗
			for (int i = 0; i < InputStatement.length(); i++) {
				if (InputStatement[i] >= 'A' && InputStatement[i] <= 'Z') {
					InputStatement[i] = InputStatement[i] + 32;
				}
			}
			//cout << InputStatement << endl;
			while (InputStatement.substr(0, 1) == " ") {
				InputStatement.erase(0, 1);
			}
			while (InputStatement.substr(InputStatement.length() - 1, 1) == " ") {
				InputStatement.erase(InputStatement.length() - 1, 1);
			}
			FindTwoBlank = InputStatement.find("  ");
			while (FindTwoBlank >= 0) {
				InputStatement.erase(FindTwoBlank, 1);
				FindTwoBlank = InputStatement.find("  ");
			}
			while (InputStatement.substr(InputStatement.length() - 2, 1) == " ") {
				InputStatement.erase(InputStatement.length() - 2, 1);
			}
			try {
				cout << InputStatement << endl;
				FindTxt = InputStatement.find(".txt");
				if (FindTxt >= 0) {
					ReadTxt(APIOnly);
				}
				else {
					DoOneStatement(APIOnly);
				}
			}
			catch (const char* message) {
				if (strcmp(message, "quit") == 0) {
					throw("quit");
				}
				cout << message << endl;
				cout << endl;
				cout << endl;
				cout << "-----------------------------------------------------------------" << endl;
			}
			InputStatement.clear();
		}
		readfile.close();
	}
	else {
		//救命 大小写怎么处理啊:( 可以直接这样的吗
		for (int i = 0; i < InputStatement.length(); i++) {
			if (InputStatement[i] >= 'A' && InputStatement[i] <= 'Z') {
				InputStatement[i] = InputStatement[i] + 32;
			}
		}
		//cout << InputStatement << endl;
		while (InputStatement.substr(0, 1) == " ") {
			InputStatement.erase(0, 1);
		}
		while (InputStatement.substr(InputStatement.length() - 1, 1) == " ") {
			InputStatement.erase(InputStatement.length() - 1, 1);
		}
		FindTwoBlank = InputStatement.find("  ");
		while (FindTwoBlank >= 0) {
			InputStatement.erase(FindTwoBlank, 1);
			FindTwoBlank = InputStatement.find("  ");
		}
		while (InputStatement.substr(InputStatement.length() - 2, 1) == " ") {
			InputStatement.erase(InputStatement.length() - 2, 1);
		}
		//cout << InputStatement << endl;
		//input statement complete
		DoOneStatement(APIOnly);
	}
}

void SqlStatement::ReadTxt(API& APIOnly) {
	int FindTxtEnd = -1;
	string txttemp;
	int InputFinish = InputStatement.find(";");
	int FindTwoBlank = -1;
	// read from .txt file
	string filename = InputStatement.substr(9, InputFinish - 9);
	InputStatement.clear();
	ifstream readfile(filename);
	while (getline(readfile, InputStatement)) {
		FindTxtEnd = InputStatement.find(";");
		while (FindTxtEnd < 0) {
			getline(readfile, txttemp);
			InputStatement = InputStatement + " " + txttemp;
			FindTxtEnd = InputStatement.find(";");
		}
		//救命 大小写怎么处理啊:( 可以直接这样的吗
		for (int i = 0; i < InputStatement.length(); i++) {
			if (InputStatement[i] >= 'A' && InputStatement[i] <= 'Z') {
				InputStatement[i] = InputStatement[i] + 32;
			}
		}
		//cout << InputStatement << endl;
		while (InputStatement.substr(0, 1) == " ") {
			InputStatement.erase(0, 1);
		}
		while (InputStatement.substr(InputStatement.length() - 1, 1) == " ") {
			InputStatement.erase(InputStatement.length() - 1, 1);
		}
		FindTwoBlank = InputStatement.find("  ");
		while (FindTwoBlank >= 0) {
			InputStatement.erase(FindTwoBlank, 1);
			FindTwoBlank = InputStatement.find("  ");
		}
		while (InputStatement.substr(InputStatement.length() - 2, 1) == " ") {
			InputStatement.erase(InputStatement.length() - 2, 1);
		}
		try {
			cout << InputStatement << endl;
			/*
			if (this->InputStatement.find(".txt") >= 0) {
				ReadTxt(APIOnly);
			}
			else {
				DoOneStatement(APIOnly);
			}
			*/
			DoOneStatement(APIOnly);
		}
		catch (const char* message) {
			if (strcmp(message, "quit") == 0) {
				throw("quit");
			}
			cout << message << endl;
			cout << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------" << endl;
		}
		InputStatement.clear();
	}
	readfile.close();
}

void SqlStatement::DoOneStatement(API& APIOnly) {
	int flag = 1;
	int startclock = clock();
	int endclock;
	string head;    //create or delete or insert or drop or select or exit
	string head2;   //tablename or indexname
	head = InputStatement.substr(0, 6);
	if (head == "create") {
		//to create
		head2 = InputStatement.substr(7, 5);
		if (head2 == "table") {
			//to create table
			//cout << "create table" << endl;
			IsCreateTable(APIOnly);
			flag = 0;
		}
		else if (head2 == "index") {
			//to create index
			//cout << "create index" << endl;
			IsCreateIndex(APIOnly);
			flag = 0;
		}
	}
	else if (head == "select") {
		//to select
		IsSelect(APIOnly);
		flag = 0;
	}
	else if (head == "insert") {
		//to insert
		IsInsert(APIOnly);
		flag = 0;
	}
	else if (head == "delete") {
		//to delete
		IsDelete(APIOnly);
		flag = 0;
	}
	//cout << InputStatement << endl;
	head = InputStatement.substr(0, 4);
	if (head == "drop") {
		//to drop
		head2 = InputStatement.substr(5, 5);
		if (head2 == "table") {
			//to drop table
			IsDropTable(APIOnly);
			flag = 0;
		}
		else if (head2 == "index") {
			//to drop index
			IsDropIndex(APIOnly);
			flag = 0;
		}
	}
	else if (head == "quit") {
		//to exit
		IsExit();
		flag = 0;
	}
	if (flag == 1) {
		throw("SYNTAX ERROR");
	}
	endclock = clock();
	double wholetime = (double)(endclock - startclock) / CLOCKS_PER_SEC;
	cout << "Total time:" << wholetime * 1000 << "ms" << endl;
	cout << endl;
	cout << endl;
	cout << "-----------------------------------------------------------------" << endl;
	return;
}

void SqlStatement::IsCreateTable(API& APIOnly) {
	string TableName;
	string OneAttribute;
	int FindLeftBracket = -1;
	int FindRightBracket = -1;
	int FindBlankSpace = -1;
	int FindComma = -1;
	int FindComma2 = -1;
	int FindSemicolon = -1;
	int RowBegin;
	int RowEnd;
	int FindPrimaryKey = -1;
	string PrimaryKey = "";
	FindSemicolon = InputStatement.find(";");
	FindPrimaryKey = InputStatement.find("primary key");
	if (FindPrimaryKey >= 0) {
		PrimaryKey = InputStatement.substr(FindPrimaryKey + 11, FindSemicolon - FindPrimaryKey - 11);
		FindBlankSpace = PrimaryKey.find(" ");
		while (FindBlankSpace >= 0) {
			PrimaryKey = PrimaryKey.erase(FindBlankSpace, 1);
			FindBlankSpace = PrimaryKey.find(" ");
		}
		FindLeftBracket = PrimaryKey.find("(");
		while (FindLeftBracket >= 0) {
			PrimaryKey = PrimaryKey.erase(FindLeftBracket, 1);
			FindLeftBracket = PrimaryKey.find("(");
		}
		FindRightBracket = PrimaryKey.find(")");
		while (FindRightBracket >= 0) {
			PrimaryKey = PrimaryKey.erase(FindRightBracket, 1);
			FindRightBracket = PrimaryKey.find(")");
		}
	}
	//cout << PrimaryKey << endl;
	FindLeftBracket = InputStatement.find("(");
	TableName = InputStatement.substr(13, FindLeftBracket - 13);
	FindBlankSpace = TableName.find(" ");
	while (FindBlankSpace >= 0) {
		TableName = TableName.erase(FindBlankSpace, 1);
		FindBlankSpace = TableName.find(" ");
	}
	Table TableA(TableName);
	//cout << TableName << endl;
	FindComma = InputStatement.find(",");
	if (FindComma < 0) {
		FindComma = FindSemicolon - 1;
	}
	FindComma2 = FindComma;
	OneAttribute = InputStatement.substr(FindLeftBracket + 1, FindComma - FindLeftBracket - 1);
	if (FindComma == FindSemicolon - 1) {
		istringstream CutString(OneAttribute);
		string Cut;
		int CountCut = 0;
		string AttriName;
		string CharNum;
		int AttriType = -2;
		int AttriUnique = 0;
		int AttriPrimaryKey = 0;
		int AttriIndex = 0;
		while (CutString >> Cut) {
			if (CountCut == 0) {
				if (Cut == "primary") {
					break;
				}
				AttriName = Cut;
				CountCut++;
				if (AttriName == PrimaryKey) {
					AttriPrimaryKey = 1;
					AttriIndex = 1;
					AttriUnique = 1;
				}
				else {
					AttriPrimaryKey = 0;
				}
				//cout << AttriPrimaryKey << endl;
				//cout << AttriName << endl;
			}
			else if (CountCut == 1) {
				if (Cut == "int") {
					AttriType = -1;
				}
				else if (Cut == "float") {
					AttriType = 0;
				}
				else if (Cut.find("char") >= 0) {
					if (Cut == "char") {
						CutString >> Cut;
						CharNum = Cut.substr(1, Cut.length() - 2);
						AttriType = atoi(CharNum.c_str());
					}
					else {
						CharNum = Cut.substr(5, Cut.length() - 6);
						AttriType = atoi(CharNum.c_str());
					}
				}
				else {
					throw("invalid type");
					return;
				}
				CountCut++;
				//cout << AttriType << endl;
			}
			else if (Cut == "unique") {
				AttriUnique = 1;
			}
			//cout << Cut << endl;
		}
		Attribute TempA(AttriName);
		string TempIndexName;
		if (AttriIndex == 1) {
			TempIndexName = TableName + "_" + AttriName;
		}
		else {
			TempIndexName = "@@";
		}
		TempA.SetAttribute(AttriType, AttriUnique, AttriPrimaryKey, AttriIndex, TempIndexName);
		if (TempA.AttributeType != -2) {
			TableA.AddAttribute(TempA);
		}
		OneAttribute = InputStatement.substr(FindComma + 1, FindComma2 - FindComma - 1);
	}
	while (FindComma != FindSemicolon - 1) {
		istringstream CutString(OneAttribute);
		string Cut;
		int CountCut = 0;
		string AttriName;
		string CharNum;
		int AttriType = -2;
		int AttriUnique = 0;
		int AttriPrimaryKey = 0;
		int AttriIndex = 0;
		while (CutString >> Cut) {
			if (CountCut == 0) {
				if (Cut == "primary") {
					break;
				}
				AttriName = Cut;
				CountCut++;
				if (AttriName == PrimaryKey) {
					AttriPrimaryKey = 1;
					AttriIndex = 1;
					AttriUnique = 1;
				}
				else {
					AttriPrimaryKey = 0;
				}
				//cout << AttriPrimaryKey << endl;
				//cout << AttriName << endl;
			}
			else if (CountCut == 1) {
				if (Cut == "int") {
					AttriType = -1;
				}
				else if (Cut == "float") {
					AttriType = 0;
				}
				else if (Cut.find("char") >= 0) {
					if (Cut == "char") {
						CutString >> Cut;
						CharNum = Cut.substr(1, Cut.length() - 2);
						AttriType = atoi(CharNum.c_str());
						if (AttriType <= 0) {
							throw("invalid type");
						}
					}
					else {
						CharNum = Cut.substr(5, Cut.length() - 6);
						AttriType = atoi(CharNum.c_str());
						if (AttriType <= 0) {
							throw("invalid type");
						}
					}
				}
				else {
					throw("invalid type");
					return;
				}
				CountCut++;
				//cout << AttriType << endl;
			}
			else if (Cut == "unique") {
				AttriUnique = 1;
			}
			//cout << Cut << endl;
		}
		Attribute TempA(AttriName);
		string TempIndexName;
		if (AttriIndex == 1) {
			TempIndexName = TableName + "_" + AttriName;
		}
		else {
			TempIndexName = "@@";
		}
		TempA.SetAttribute(AttriType, AttriUnique, AttriPrimaryKey, AttriIndex, TempIndexName);
		if (TempA.AttributeType != -2) {
			TableA.AddAttribute(TempA);
		}
		if (TableA.AttributeInTable.size() > 32) {
			throw("The number of attributes in one table can not greater than 32");
		}
		//cout << OneAttribute << endl;
		FindComma = FindComma2;
		FindComma2 = InputStatement.find(",", FindComma + 1);
		if (FindComma2 < 0) {
			FindComma2 = FindSemicolon - 1;
		}
		OneAttribute = InputStatement.substr(FindComma + 1, FindComma2 - FindComma - 1);
	}
	APIOnly.APICreateTable(TableA);
	cout << "create table " << TableA.TableName << " successfully!" << endl;
	return;
}

void SqlStatement::IsCreateIndex(API& APIOnly) {
	int FindOn;
	int FindLeftBracket;
	int FindRightBracket;
	string IndexName;
	string IndexTable;
	string IndexAttribute;
	int IfUnique;
	FindOn = InputStatement.find(" on ");
	FindLeftBracket = InputStatement.find("(");
	FindRightBracket = InputStatement.find(")");
	IndexName = InputStatement.substr(13, FindOn - 13);
	//cout << IndexName << endl;
	IndexTable = InputStatement.substr(FindOn + 4, FindLeftBracket - FindOn - 4);
	//cout << IndexTable << endl;
	IndexAttribute = InputStatement.substr(FindLeftBracket + 1, FindRightBracket - FindLeftBracket - 1);
	//cout << IndexAttribute << endl;
	Index IndexA(IndexTable, IndexName, IndexAttribute);
	APIOnly.APICreateIndex(IndexA);
	cout << "create index " << IndexA.IndexName << " sucessfully!" << endl;
	return;
}

void SqlStatement::IsSelect(API& APIOnly) {
	int SelectAttriNumber = 0; //-1 means all
	string SelectAttri[50];
	string AttriNameString; //store the string from "select" to "from"
	string SelectTableName;
	int FindFrom = -1;
	int FindComma = -1;
	int FindWhere = -1;
	int FindAnd = -1;
	int SelectNumber[40] = { 0 };

	FindFrom = InputStatement.find("from");
	AttriNameString = InputStatement.substr(7, FindFrom - 8);
	//cout << AttriNameString << endl;
	FindComma = AttriNameString.find(",");
	while (FindComma >= 0) {
		AttriNameString.replace(FindComma, 1, " ");
		FindComma = AttriNameString.find(",");
	}
	if (AttriNameString == "*") {
		SelectAttriNumber = -1;
		//cout << AttriNameString << endl;
		for (int i = 0; i < 40; i++) {
			SelectNumber[i] = 1;
		}
	}
	else {
		istringstream CutString(AttriNameString);
		string Cut;
		while (CutString >> Cut) {
			SelectAttri[SelectAttriNumber] = Cut;
			SelectAttriNumber++;
			//cout << Cut << endl;
		}
	}
	FindWhere = InputStatement.find("where");
	if (FindWhere == -1) {
		FindWhere = InputStatement.find(";") + 1;
	}
	SelectTableName = InputStatement.substr(FindFrom + 5, FindWhere - FindFrom - 6);
	//cout << SelectTableName << endl;
	Table TableS(SelectTableName);
	CatalogManager TempC;
	TempC.FindTableByName(SelectTableName, TableS);
	FindWhere = InputStatement.find("where") + 5; //+5用于跳过where
	CheckCondition allcondition(TableS);
	// ↓这里>=5是因为上面用+5跳过where了
	if (FindWhere >= 5) {
		int FindEqual = -1;
		int FindNotEqual = -1;
		int FindGreater = -1;
		int FindLess = -1;
		int FindGreaterEqual = -1;
		int FindLessEqual = -1;
		int FindOperator = -1;
		int FindOperatorEnd = -1;
		FindAnd = InputStatement.find("and");

		while (FindAnd >= 0) {
			string conditionstring;
			ConditionOperator COTemp = Oequal;
			conditionstring = InputStatement.substr(FindWhere, FindAnd - FindWhere);
			//以下是处理操作符
			FindEqual = conditionstring.find("=");
			FindNotEqual = conditionstring.find("!=");
			FindGreater = conditionstring.find(">");
			FindLess = conditionstring.find("<");
			FindGreaterEqual = conditionstring.find(">=");
			FindLessEqual = conditionstring.find("<=");
			if (FindNotEqual >= 0) {
				COTemp = Onotequal;
				FindOperator = FindNotEqual;
				FindOperatorEnd = FindOperator + 2;
			}
			else if (FindGreaterEqual >= 0) {
				COTemp = Ogreater_equal;
				FindOperator = FindGreaterEqual;
				FindOperatorEnd = FindOperator + 2;
			}
			else if (FindLessEqual >= 0) {
				COTemp = Oless_equal;
				FindOperator = FindLessEqual;
				FindOperatorEnd = FindOperator + 2;
			}
			else if (FindEqual >= 0) {
				COTemp = Oequal;
				FindOperator = FindEqual;
				FindOperatorEnd = FindOperator + 1;
			}
			else if (FindGreater >= 0) {
				COTemp = Ogreater;
				FindOperator = FindGreater;
				FindOperatorEnd = FindOperator + 1;
			}
			else if (FindLess >= 0) {
				COTemp = Oless;
				FindOperator = FindLess;
				FindOperatorEnd = FindOperator + 1;
			}
			else {
				throw("invalid operator");
			}
			//以下得到属性名和选择的值
			string Aname;
			string BeWhat; //attribute = ?
			Aname = conditionstring.substr(0, FindOperator);
			BeWhat = conditionstring.substr(FindOperatorEnd);
			int FindBlank = -1;
			FindBlank = Aname.find(" ");
			while (FindBlank >= 0) {
				Aname = Aname.erase(FindBlank, 1);
				FindBlank = Aname.find(" ");
			}
			FindBlank = BeWhat.find(" ");
			while (FindBlank == 0) {
				BeWhat = BeWhat.erase(FindBlank, 1);
				FindBlank = BeWhat.find(" ");
			}
			FindBlank = BeWhat.find(" ");
			while (FindBlank == BeWhat.size() - 1) {
				BeWhat = BeWhat.erase(FindBlank, 1);
				FindBlank = BeWhat.find(" ");
			}
			int can = 0;
			int cat = -2;
			int ci = 0;
			float cf = 0;
			string cc = "";
			//TableS.ShowTable();
			//cout << "Aname" << Aname << endl;
			for (int i = 0; i < TableS.AttributeNumber; i++) {
				//cout << TableS.AttributeInTable[i].AttributeName << endl;
				if (TableS.AttributeInTable[i].AttributeName == Aname) {
					can = i;
					cat = TableS.AttributeInTable[i].AttributeType;
					SelectNumber[i] = 1;
					break;
				}
				if (i == TableS.AttributeNumber - 1) {
					throw("No such Attribute in this table");
				}
			}
			istringstream tochange(BeWhat);
			if (cat == -1) {
				tochange >> ci;
			}
			else if (cat == 0) {
				tochange >> cf;
			}
			else {
				tochange >> cc;
			}
			Condition onecon(COTemp, can, cat, ci, cf, cc);
			allcondition.AddCondition(onecon);
			FindWhere = FindWhere + conditionstring.size();
			FindAnd = InputStatement.find("and", FindAnd + 3);
		}
		//处理最后一条条件语句(and 之后的/无and的第一条)
		if (FindAnd < 0) {
			FindAnd = InputStatement.rfind("and");
		}
		if (FindAnd < 0) {
			// 只有一条(无 and)
			FindAnd = InputStatement.find("where");
			FindAnd = FindAnd + 5;
		}
		else {
			// 有and(最后一个and之后的)
			FindAnd = FindAnd + 3;
		}
		string conditionstring;
		ConditionOperator COTemp;
		conditionstring = InputStatement.substr(FindAnd, InputStatement.find(";") - FindAnd);
		//以下是处理操作符
		FindEqual = conditionstring.find("=");
		FindNotEqual = conditionstring.find("!=");
		FindGreater = conditionstring.find(">");
		FindLess = conditionstring.find("<");
		FindGreaterEqual = conditionstring.find(">=");
		FindLessEqual = conditionstring.find("<=");
		if (FindNotEqual >= 0) {
			COTemp = Onotequal;
			FindOperator = FindNotEqual;
			FindOperatorEnd = FindOperator + 2;
		}
		else if (FindGreaterEqual >= 0) {
			COTemp = Ogreater_equal;
			FindOperator = FindGreaterEqual;
			FindOperatorEnd = FindOperator + 2;
		}
		else if (FindLessEqual >= 0) {
			COTemp = Oless_equal;
			FindOperator = FindLessEqual;
			FindOperatorEnd = FindOperator + 2;
		}
		else if (FindEqual >= 0) {
			COTemp = Oequal;
			FindOperator = FindEqual;
			FindOperatorEnd = FindOperator + 1;
		}
		else if (FindGreater >= 0) {
			COTemp = Ogreater;
			FindOperator = FindGreater;
			FindOperatorEnd = FindOperator + 1;
		}
		else if (FindLess >= 0) {
			COTemp = Oless;
			FindOperator = FindLess;
			FindOperatorEnd = FindOperator + 1;
		}
		else {
			throw("invalid operator");
		}
		//以下得到属性名和选择的值
		string Aname = "";
		string BeWhat = ""; //attribute = ? 的 ?
		Aname = conditionstring.substr(0, FindOperator);
		BeWhat = conditionstring.substr(FindOperatorEnd);
		int FindBlank = -1;
		FindBlank = Aname.find(" ");
		while (FindBlank >= 0) {
			Aname = Aname.erase(FindBlank, 1);
			FindBlank = Aname.find(" ");
		}
		FindBlank = BeWhat.find(" ");
		while (FindBlank == 0) {
			BeWhat = BeWhat.erase(FindBlank, 1);
			FindBlank = BeWhat.find(" ");
		}
		FindBlank = BeWhat.find(" ");
		while (FindBlank == BeWhat.size() - 1) {
			BeWhat = BeWhat.erase(FindBlank, 1);
			FindBlank = BeWhat.find(" ");
		}
		int can = 0;
		int cat = -2;
		int ci = 0;
		float cf = 0;
		string cc = "";
		for (int i = 0; i < TableS.AttributeNumber; i++) {
			if (TableS.AttributeInTable[i].AttributeName == Aname) {
				can = i;
				cat = TableS.AttributeInTable[i].AttributeType;
				break;
			}
			if (i == TableS.AttributeNumber - 1) {
				throw("No such Attribute");
			}
		}
		for (int i = 0; i < TableS.AttributeNumber; i++) {
			for (int j = 0; j < SelectAttriNumber; j++) {
				if (TableS.AttributeInTable[i].AttributeName == SelectAttri[j]) {
					SelectNumber[i] = 1;
				}
			}
		}
		istringstream tochange(BeWhat);
		if (cat == -1) {
			tochange >> ci;
		}
		else if (cat == 0) {
			tochange >> cf;
		}
		else {
			tochange >> cc;
		}
		Condition onecon(COTemp, can, cat, ci, cf, cc);
		allcondition.AddCondition(onecon);
		FindWhere = FindWhere + conditionstring.size();
		FindAnd = InputStatement.find("and", FindAnd + 3);
	}
	else {
		int equaltimes = 0;
		for (int i = 0; i < TableS.AttributeNumber; i++) {
			for (int j = 0; j < SelectAttriNumber; j++) {
				if (TableS.AttributeInTable[i].AttributeName == SelectAttri[j]) {
					SelectNumber[i] = 1;
					equaltimes++;
				}
			}
		}
		if (equaltimes < SelectAttriNumber) {
			throw("no such attribute exist in this table");
		}
		//findwhere<0的情况
		//cout << "nowhere" << endl;
		allcondition.SetAll();
	}
	//allcondition.ShowCheckCondition();
	/*
	for (int i = 0; i < TableS.AttributeNumber; i++) {
		cout << SelectNumber[i] << " ";
	}
	cout << endl;
	*/
	APIOnly.APISelect(TableS, allcondition, SelectNumber);
	return;
}

//insert into tablename values(),();
void SqlStatement::IsInsert(API& APIOnly) {
	int FindInsert = -1;
	int FindEnd = -1;
	int FindValue = -1;
	int FindInto = -1;
	int FindComma = -1;

	FindInsert = InputStatement.find("insert");
	FindEnd = InputStatement.find(";");
	FindValue = InputStatement.find("values");
	FindInto = InputStatement.find("into");

	if (FindValue < 0) {
		throw("syntax error");
	}

	if (FindInto < 0) {
		throw("syntax error");
	}

	string TableName = InputStatement.substr(FindInto + 4, FindValue - FindInto - 4);
	//cout << TableName << endl;

	int FindBlank = -1;
	FindBlank = TableName.find(" ");
	while (FindBlank >= 0) {
		TableName = TableName.erase(FindBlank, 1);
		FindBlank = TableName.find(" ");
	}
	Table InsertTable(TableName);
	CatalogManager TEMPC;
	TEMPC.FindTableByName(TableName, InsertTable);

	int FindLeft = -1;
	int FindRight = -1;
	string OneOfInsertValue;
	string InsertValue;
	FindLeft = InputStatement.find("(");
	FindRight = InputStatement.find(")");
	InsertValue = InputStatement.substr(FindLeft + 1, FindRight - FindLeft - 1);
	if (FindLeft < 0 || FindRight < 0) {
		throw("Syntax Error");
	}
	int ValueLeft = -1;
	int ValueRight = -1;
	vector<AllInsertValue> allinsert;
	AllInsertValue oneinsert(InsertTable);
	int TempLeft = 0;
	while (FindLeft >= 0 && FindRight >= 0) {
		//cout << InsertValue << endl;
		FindComma = InsertValue.find(",");
		if (FindComma < 0) {
			FindComma = FindRight - FindLeft - 1;
		}
		ValueLeft = 0;
		ValueRight = FindComma;
		oneinsert.clear();
		for (int i = 0; i < InsertTable.AttributeNumber; i++) {
			//cout << ValueLeft << " " << ValueRight << endl;
			OneOfInsertValue = InsertValue.substr(ValueLeft, ValueRight - ValueLeft);
			if (InsertTable.AttributeInTable[i].AttributeType == -1) {
				//int
				istringstream isint(OneOfInsertValue);
				int inttemp;
				int ifnull = 0;
				if (OneOfInsertValue == "" || OneOfInsertValue == "null") {
					ifnull = 1;
				}
				isint >> inttemp;
				OneInsertValue tempvalue;
				tempvalue.SetValue(-1, inttemp, ifnull);
				oneinsert.AddValue(tempvalue);
			}
			else if (InsertTable.AttributeInTable[i].AttributeType == 0) {
				//float
				istringstream isfloat(OneOfInsertValue);
				float floattemp;
				int ifnull = 0;
				if (OneOfInsertValue == "" || OneOfInsertValue == "null") {
					ifnull = 1;
				}
				isfloat >> floattemp;
				OneInsertValue tempvalue;
				tempvalue.SetValue(0, floattemp, ifnull);
				oneinsert.AddValue(tempvalue);
			}
			else {
				//string
				string stringtemp;
				int ifnull = 0;
				int FindLeftTemp = -1;
				int FindRightTemp = -1;
				FindLeftTemp = OneOfInsertValue.find("'") + 1;
				FindRightTemp = OneOfInsertValue.size() - 1;
				stringtemp = OneOfInsertValue.substr(FindLeftTemp, FindRightTemp - FindLeftTemp);
				//cout << stringtemp << endl;
				if (stringtemp == "" || stringtemp == "null") {
					ifnull = 1;
				}
				OneInsertValue tempvalue;
				tempvalue.SetValue(stringtemp.length(), stringtemp, ifnull);
				oneinsert.AddValue(tempvalue);
			}
			FindComma = InsertValue.find(",", ValueRight + 1);
			if (FindComma < 0) {
				FindComma = FindRight - FindLeft - 1;
			}
			ValueLeft = ValueRight + 1;
			ValueRight = FindComma;
		}
		allinsert.push_back(oneinsert);
		FindLeft = InputStatement.find("(", FindLeft + 1);
		FindRight = InputStatement.find(")", FindRight + 1);
		InsertValue = InputStatement.substr(FindLeft + 1, FindRight - FindLeft - 1);
	}
	for (int i = 0; i < allinsert.size(); i++) {
		// do API insert

		int mallocsize = 0;
		int bitnum = 0;
		void* data;
		void* bit;

		bitnum = InsertTable.AttributeNumber;
		if (bitnum % 8 == 0) {
			bitnum = bitnum / 8;
		}
		else {
			bitnum = bitnum / 8 + 1;
		}
		bit = malloc(bitnum);

		for (int i = 0; i < bitnum; i++) {
			*((char*)bit + i) = 0;
		}
		for (int i = 0; i < InsertTable.AttributeNumber; i++) {
			if (InsertTable.AttributeInTable[i].AttributeType == -1 || InsertTable.AttributeInTable[i].AttributeType == 0) {
				mallocsize = mallocsize + 4;
			}
			else {
				mallocsize = mallocsize + 256;
			}
		}

		data = malloc(mallocsize);
		if (data == NULL) {
			throw("no space");
		}

		int* datasize = &mallocsize;
		int* bitsize = &bitnum;

		(*datasize) = mallocsize;
		(*bitsize) = bitnum;

		CheckCondition cc(InsertTable);


		for (int j = 0; j < InsertTable.AttributeNumber; j++) {
			if (InsertTable.AttributeInTable[j].IsUnique == 1) {
				int at = InsertTable.AttributeInTable[j].AttributeType;
				int ci = 0;
				float cf = 0;
				string cs = "";
				if (at == -1) {
					//int
					ci = allinsert[i].InsertValue[j].intvalue;
				}
				else if (at == 0) {
					//float
					cf = allinsert[i].InsertValue[j].floatvalue;
				}
				else {
					cs = allinsert[i].InsertValue[j].charvalue;
				}
				if (allinsert[i].InsertValue[j].ifnull == 1) {
					continue;
				}
				Condition ct(Oequal, j, at, ci, cf, cs);
				cc.AddCondition(ct);
				int flag = 0;

				flag = APIOnly.APIInsertSelect(InsertTable, cc);
				if (flag > 0) {
					string uniquename = InsertTable.AttributeInTable[cc.condition[0].CAttributeNum].AttributeName;
					uniquename = "attribute " + uniquename + " should be unique";
					cout << uniquename << endl;
					throw("");
				}
				if (cc.condition.size() > 0) {
					cc.condition.pop_back();
				}
			}
		}
		allinsert[i].ValueToRecord(APIOnly.buf, bit, data, datasize, bitsize);
		APIOnly.APIInsert(InsertTable, data, bit, (*datasize), (*bitsize));
		cout << "Insert Values into " << InsertTable.TableName << " Successfully!" << endl;

		free(data);		// modified by cdh 6/27
	}
	if ((FindLeft < 0 && FindRight >= 0) || (FindRight < 0 && FindLeft >= 0)) {
		throw("Syntax Error");
	}
}

//drop table tablename;
void SqlStatement::IsDropTable(API& APIOnly) {
	//drop data in table
	int FindDrop = -1;
	int FindEnd = -1;
	int FindTable = -1;

	FindDrop = InputStatement.find("drop");
	FindEnd = InputStatement.find(";");
	FindTable = InputStatement.find("table") + 5;

	string TableName = InputStatement.substr(FindTable, FindEnd - FindTable);
	while (TableName.substr(0, 1) == " ") {
		TableName.erase(0, 1);
	}
	while (TableName.substr(TableName.length() - 1, 1) == " ") {
		TableName.erase(TableName.length() - 1, 1);
	}
	CatalogManager CT;
	Table droptable(TableName);
	CT.FindTableByName(TableName, droptable);
	APIOnly.APIDropTable(droptable);
	cout << "drop table " << TableName << " successfully" << endl;
	return;
}

//drop index indexname;
void SqlStatement::IsDropIndex(API& APIOnly) {
	//drop b+ tree
	int FindDrop = -1;
	int FindEnd = -1;
	int FindIndex = -1;

	FindDrop = InputStatement.find("drop");
	FindEnd = InputStatement.find(";");
	FindIndex = InputStatement.find("index") + 5;

	string IndexName = InputStatement.substr(FindIndex, FindEnd - FindIndex);
	while (IndexName.substr(0, 1) == " ") {
		IndexName.erase(0, 1);
	}
	while (IndexName.substr(IndexName.length() - 1, 1) == " ") {
		IndexName.erase(IndexName.length() - 1, 1);
	}
	Index indextemp("x", "x", "x");
	CatalogManager TempCata;
	TempCata.FindIndexByName(IndexName, indextemp);
	APIOnly.APIDropIndex(indextemp);
	cout << "drop index " << IndexName << " successfully" << endl;
	return;
}

//update tablename set attributename = ? ,...
void SqlStatement::IsUpdate(API& APIOnly) {

}

//delete from tablename;
//delete from tablename where condition;
void SqlStatement::IsDelete(API& APIOnly) {
	int FindFrom = -1;
	int FindWhere = -1;
	FindFrom = InputStatement.find("from");
	FindWhere = InputStatement.find("where");
	if (FindWhere < 0) {
		//delete all
		//record manager debind
		//index manager drop
		//or drop table and create table
		FindWhere = InputStatement.find(";");
	}
	string TableName = InputStatement.substr(FindFrom + 4, FindWhere - FindFrom - 4);
	int FindBlank = -1;
	FindBlank = TableName.find(" ");
	while (FindBlank >= 0) {
		TableName = TableName.erase(FindBlank, 1);
		FindBlank = TableName.find(" ");
	}
	Table TableS(TableName);
	CatalogManager TempC;
	TempC.FindTableByName(TableName, TableS);

	FindWhere = InputStatement.find("where") + 5; //+5用于跳过where
	CheckCondition allcondition(TableS);
	// ↓这里>=5是因为上面用+5跳过where了
	if (FindWhere >= 5) {
		int FindEqual = -1;
		int FindNotEqual = -1;
		int FindGreater = -1;
		int FindLess = -1;
		int FindGreaterEqual = -1;
		int FindLessEqual = -1;
		int FindOperator = -1;
		int FindOperatorEnd = -1;
		int FindAnd = -1;
		FindAnd = InputStatement.find("and");
		while (FindAnd >= 0) {
			string conditionstring;
			ConditionOperator COTemp = Oequal;
			conditionstring = InputStatement.substr(FindWhere, FindAnd - FindWhere);
			//以下是处理操作符
			FindEqual = conditionstring.find("=");
			FindNotEqual = conditionstring.find("!=");
			FindGreater = conditionstring.find(">");
			FindLess = conditionstring.find("<");
			FindGreaterEqual = conditionstring.find(">=");
			FindLessEqual = conditionstring.find("<=");
			if (FindNotEqual >= 0) {
				COTemp = Onotequal;
				FindOperator = FindNotEqual;
				FindOperatorEnd = FindOperator + 2;
			}
			else if (FindGreaterEqual >= 0) {
				COTemp = Ogreater_equal;
				FindOperator = FindGreaterEqual;
				FindOperatorEnd = FindOperator + 2;
			}
			else if (FindLessEqual >= 0) {
				COTemp = Oless_equal;
				FindOperator = FindLessEqual;
				FindOperatorEnd = FindOperator + 2;
			}
			else if (FindEqual >= 0) {
				COTemp = Oequal;
				FindOperator = FindEqual;
				FindOperatorEnd = FindOperator + 1;
			}
			else if (FindGreater >= 0) {
				COTemp = Ogreater;
				FindOperator = FindGreater;
				FindOperatorEnd = FindOperator + 1;
			}
			else if (FindLess >= 0) {
				COTemp = Oless;
				FindOperator = FindLess;
				FindOperatorEnd = FindOperator + 1;
			}
			else {
				throw("invalid operator");
			}
			//以下得到属性名和选择的值
			string Aname;
			string BeWhat; //attribute = ?
			Aname = conditionstring.substr(0, FindOperator);
			BeWhat = conditionstring.substr(FindOperatorEnd);
			int FindBlank = -1;
			FindBlank = Aname.find(" ");
			while (FindBlank >= 0) {
				Aname = Aname.erase(FindBlank, 1);
				FindBlank = Aname.find(" ");
			}
			FindBlank = BeWhat.find(" ");
			while (FindBlank == 0) {
				BeWhat = BeWhat.erase(FindBlank, 1);
				FindBlank = BeWhat.find(" ");
			}
			FindBlank = BeWhat.find(" ");
			while (FindBlank == BeWhat.size() - 1) {
				BeWhat = BeWhat.erase(FindBlank, 1);
				FindBlank = BeWhat.find(" ");
			}
			int can = 0;
			int cat = -2;
			int ci = 0;
			float cf = 0;
			string cc = "";
			//TableS.ShowTable();
			for (int i = 0; i < TableS.AttributeNumber; i++) {
				if (TableS.AttributeInTable[i].AttributeName == Aname) {
					can = i;
					cat = TableS.AttributeInTable[i].AttributeType;
					break;
				}
				if (i == TableS.AttributeNumber - 1) {
					throw("No such Attribute in this table");
				}
			}
			istringstream tochange(BeWhat);
			if (cat == -1) {
				tochange >> ci;
			}
			else if (cat == 0) {
				tochange >> cf;
			}
			else {
				tochange >> cc;
			}
			Condition onecon(COTemp, can, cat, ci, cf, cc);
			allcondition.AddCondition(onecon);
			FindWhere = FindWhere + conditionstring.size();
			FindAnd = InputStatement.find("and", FindAnd + 3);
		}
		//处理最后一条条件语句(and 之后的/无and的第一条)
		if (FindAnd < 0) {
			FindAnd = InputStatement.rfind("and");
		}
		if (FindAnd < 0) {
			// 只有一条(无 and)
			FindAnd = InputStatement.find("where");
			FindAnd = FindAnd + 5;
		}
		else {
			// 有and(最后一个and之后的)
			FindAnd = FindAnd + 3;
		}
		string conditionstring;
		ConditionOperator COTemp;
		conditionstring = InputStatement.substr(FindAnd, InputStatement.find(";") - FindAnd);
		//以下是处理操作符
		FindEqual = conditionstring.find("=");
		FindNotEqual = conditionstring.find("!=");
		FindGreater = conditionstring.find(">");
		FindLess = conditionstring.find("<");
		FindGreaterEqual = conditionstring.find(">=");
		FindLessEqual = conditionstring.find("<=");
		if (FindNotEqual >= 0) {
			COTemp = Onotequal;
			FindOperator = FindNotEqual;
			FindOperatorEnd = FindOperator + 2;
		}
		else if (FindGreaterEqual >= 0) {
			COTemp = Ogreater_equal;
			FindOperator = FindGreaterEqual;
			FindOperatorEnd = FindOperator + 2;
		}
		else if (FindLessEqual >= 0) {
			COTemp = Oless_equal;
			FindOperator = FindLessEqual;
			FindOperatorEnd = FindOperator + 2;
		}
		else if (FindEqual >= 0) {
			COTemp = Oequal;
			FindOperator = FindEqual;
			FindOperatorEnd = FindOperator + 1;
		}
		else if (FindGreater >= 0) {
			COTemp = Ogreater;
			FindOperator = FindGreater;
			FindOperatorEnd = FindOperator + 1;
		}
		else if (FindLess >= 0) {
			COTemp = Oless;
			FindOperator = FindLess;
			FindOperatorEnd = FindOperator + 1;
		}
		else {
			throw("invalid operator");
		}
		//以下得到属性名和选择的值
		string Aname = "";
		string BeWhat = ""; //attribute = ? 的 ?
		Aname = conditionstring.substr(0, FindOperator);
		BeWhat = conditionstring.substr(FindOperatorEnd);
		int FindBlank = -1;
		FindBlank = Aname.find(" ");
		while (FindBlank >= 0) {
			Aname = Aname.erase(FindBlank, 1);
			FindBlank = Aname.find(" ");
		}
		FindBlank = BeWhat.find(" ");
		while (FindBlank == 0) {
			BeWhat = BeWhat.erase(FindBlank, 1);
			FindBlank = BeWhat.find(" ");
		}
		FindBlank = BeWhat.find(" ");
		while (FindBlank == BeWhat.size() - 1) {
			BeWhat = BeWhat.erase(FindBlank, 1);
			FindBlank = BeWhat.find(" ");
		}
		int can = 0;
		int cat = -2;
		int ci = 0;
		float cf = 0;
		string cc = "";
		for (int i = 0; i < TableS.AttributeNumber; i++) {
			if (TableS.AttributeInTable[i].AttributeName == Aname) {
				can = i;
				cat = TableS.AttributeInTable[i].AttributeType;
				break;
			}
			if (i == TableS.AttributeNumber - 1) {
				throw("No such Attribute");
			}
		}
		istringstream tochange(BeWhat);
		if (cat == -1) {
			tochange >> ci;
		}
		else if (cat == 0) {
			tochange >> cf;
		}
		else {
			tochange >> cc;
		}
		Condition onecon(COTemp, can, cat, ci, cf, cc);
		allcondition.AddCondition(onecon);
		FindWhere = FindWhere + conditionstring.size();
		FindAnd = InputStatement.find("and", FindAnd + 3);
	}
	else {
		//findwhere<0
		allcondition.SetAll();
	}
	APIOnly.APIDelete(TableS, allcondition);
	return;
}

void SqlStatement::IsExit() {
	throw("quit");
}

