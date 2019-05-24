#include <iostream>
#include <string>
#include <fstream> 
#include <sqlite3.h>
#include <cstring>
#include <unistd.h>
#include <mysql/mysql.h>
using namespace std;

typedef struct Sign
{
	int n_cpu;
	int n_tem;
	int n_hum;
	int n_fun1;
	int n_fun2;
	int time;
	int state;
	int data;
	int nfs;
}STM32;

typedef struct Path
{
	string n_cpu;
	string n_tem;
	string n_hum;
	string n_fun1;
	string n_fun2;
	string time;
	string state;
	bool sgin;
	string mate;
}PATH;



class HandleData
{
public:
		HandleData();
		~HandleData();
		void GitData();
		PATH Handle(string strPath, string &time);
		int create_database(sqlite3 **db, const char *path);
		bool HandleSqlite3(string dbname,string name, string data, int &id, string state, string time);
		bool HandleMysql(string dbname,string name1, string data, int &id_cpu, int &id_tem, int &id_hum, string state, string time);
private:
		string name;
		string cpu;
};