
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>
#include "Interpreter.h"
int main() {
	//_CrtSetBreakAlloc(1283);
	SetUpFile();
	API APIOnly;

	cout << "  Welcome to minisql" << endl;
	cout << "  The data type it supports:" << endl;
	cout << "    int / float / char(n) 1<=n<=255" << endl;
	cout << "  The syntax it supports:" << endl;
	cout << "    create table" << endl;
	cout << "    drop table" << endl;
	cout << "    create index" << endl;
	cout << "    drop index" << endl;
	cout << "    select" << endl;
	cout << "    insert" << endl;
	cout << "    delete" << endl;
	cout << "    quit" << endl;
	cout << "    filename" << endl;

	while (1) {
		try {
			SqlStatement sql;
			sql.GetSql(APIOnly);
		}
		catch (const char* message) {
			if (strcmp(message, "quit") == 0) {
				break;
			}
			cout << message << endl;
			cout << endl;
			cout << endl;
			cout << "-----------------------------------------------------------------" << endl;
		}
	}
	//_CrtDumpMemoryLeaks();
	system("pause");
	return 0;
}