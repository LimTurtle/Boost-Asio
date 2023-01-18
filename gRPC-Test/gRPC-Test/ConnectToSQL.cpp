#include <iostream>
#include "ConnectToSQL.h"

using namespace std;

ConnectToSQL::ConnectToSQL()
{
	ConnPtr = nullptr;
	SQLInit();
}

ConnectToSQL::~ConnectToSQL()
{
	mysql_close(ConnPtr);
}

int ConnectToSQL::SQLInit()
{
	mysql_init(&Conn);
	ConnPtr = mysql_real_connect(&Conn, "127.0.0.1", "root", "Z797944z!", "sakila", 3306, (char*)NULL, 0);

	if (ConnPtr == NULL)
	{
		cout << "Error : " << mysql_error(&Conn) << endl;
		return 0;
	}

	return 1;
}

int ConnectToSQL::SQLQuery(const char* query)
{
	Stat = mysql_query(ConnPtr, query);
	if (Stat != 0)
	{
		cout << "Error : " << mysql_error(&Conn) << endl;
		return 0;
	}

	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL)
	{
		cout << Row[0] << " / " << Row[1] << " / " << Row[2] << " / " << Row[3] << endl;
	}
	mysql_free_result(Result);

	return 1;
}