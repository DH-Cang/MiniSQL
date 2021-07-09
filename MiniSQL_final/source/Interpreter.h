// 2021/06/22/11:10


#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_DEPRECATE

#include "CatalogManager.h"
#include "API.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace std;

// class SqlStatement used to get the input sql statement
class SqlStatement {
private:
	// an input statement end with ";"
	string InputStatement;
public:
	SqlStatement() {
		InputStatement = "";
	}

	// "GetSql()" used to get input sql statement
	// and determine the operation that the input statement wants to do
	void GetSql(API& APIOnly);

	void ReadTxt(API& APIOnly);

	void DoOneStatement(API& APIOnly);

	// "IsCreateTable()" used to create a table
	void IsCreateTable(API& APIOnly);

	// "IsCreateIndex()" used to create a index
	void IsCreateIndex(API& APIOnly);

	// "IsSelect()" used to process statements for query operations
	void IsSelect(API& APIOnly);

	// "IsInsert()" used to process statement for the insert operation
	void IsInsert(API& APIOnly);

	// "IsDelete()" used to process statement for the delete operation
	void IsDelete(API& APIOnly);

	// "IsDropTable()" used to process statement for drop a table
	void IsDropTable(API& APIOnly);

	// "IsDropIndex()" used to process statement for drop an index
	void IsDropIndex(API& APIOnly);

	void IsUpdate(API& APIOnly);
	// "IsExit()" used to exit the minisql
	void IsExit();

};


#endif