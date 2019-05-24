#include "HandleData.h"



HandleData::HandleData()
{
	name = "log.txt";
	cpu = "";
}

HandleData::~HandleData()
{
	
}
void HandleData::GitData()
{
	ifstream in(name);
	string line;
	string time = "";
	if(in) // 有该文件
	{
		int id_cpu = 0, id_tem = 0, id_hum = 0;
		while (getline (in, line)) // line中不包括每行的换行符
		{ 
			//处理数据
			PATH path = Handle(line,time);
			
			if(path.sgin)
			{
				if(path.mate == "cpu")
				{
					HandleMysql("t1","cpu", path.n_cpu ,id_cpu, id_tem,id_hum ,path.state, time);
				}
				else if(path.mate == "tem")
				{
					HandleMysql("t1","tem", path.n_tem ,id_cpu, id_tem,id_hum ,path.state, time);
				}
				else if(path.mate == "hum")
				{
					HandleMysql("t1","hum", path.n_hum ,id_cpu, id_tem,id_hum,path.state, time);
				}
				else if(path.mate == "fun1")
				{
					HandleMysql("t1","fun1", path.n_fun1 ,id_cpu, id_tem,id_hum ,path.state, time);
				}
				else if(path.mate == "fun2")
				{
					HandleMysql("t1","fun2", path.n_fun2 ,id_cpu, id_tem,id_hum ,path.state, time);
				}
				
			}
		}
	}
	else // 没有该文件
	{
		cout <<"no such file" << endl;
	}
}

PATH HandleData::Handle(string strPath, string &time)
{
	STM32 stm32 = {0,0,0,0,0,0,0,0,0};
	PATH path = {"", "" , "" ,"" ,"" ,"","yes" ,false ,time};
	//判断标志
	stm32.nfs = strPath.rfind("##");
	//匹配cpu
	stm32.n_cpu = strPath.rfind("cpu=", stm32.nfs - 1);
	//匹配温度
	stm32.n_tem = strPath.rfind("tem=", stm32.nfs  - 1);
	//匹配湿度
	stm32.n_hum = strPath.rfind("hum=", stm32.nfs  - 1);
	//匹配功能1
	stm32.n_fun1 = strPath.rfind("fun1=", stm32.nfs  - 1);
	//匹配功能2
	stm32.n_fun2 = strPath.rfind("fun2=", stm32.nfs  - 1);
	//匹配时间
	stm32.time = strPath.rfind("time=", stm32.nfs  - 1);
	//匹配状态
	stm32.state = strPath.rfind("state=", stm32.nfs  - 1);
	
	if(stm32.nfs  != -1)
	{
		if(stm32.n_cpu != -1)
		{
			path.n_cpu = strPath.substr(stm32.n_cpu + 4, stm32.nfs  - stm32.n_cpu - 4);
			path.mate = "cpu";
			
		}
		else if(stm32.n_tem != -1)
		{
			path.n_tem = strPath.substr(stm32.n_tem + 4, stm32.nfs  - stm32.n_tem - 4);
			path.mate = "tem";
			
		}
		else if(stm32.n_hum != -1)
		{
			path.n_hum = strPath.substr(stm32.n_hum + 4, stm32.nfs  - stm32.n_hum - 4);
			path.mate = "hum";
			
		}
		else if(stm32.n_fun1 != -1)
		{
			path.n_fun1 = strPath.substr(stm32.n_fun1 + 5, stm32.nfs  - stm32.n_fun1 - 5);
			path.mate = "fun1";
			
		}
		else if(stm32.n_fun2 != -1)
		{
			path.n_fun2 = strPath.substr(stm32.n_fun2 + 5, stm32.nfs  - stm32.n_fun2 - 5);
			path.mate = "fun2";
			
		}
		else if(stm32.time != -1)
		{
			path.time = strPath.substr(stm32.time + 5, stm32.nfs  - stm32.time - 5);
			path.mate = "time";
			time = path.time;
			
		}
		else if(stm32.state != -1)
		{
			path.state = strPath.substr(stm32.state + 6, stm32.nfs  - stm32.state - 6);
			path.mate = "state";
			
		}
		path.sgin = true;
		return path;
	}
	path.sgin = false;
	return path;
}

int HandleData::create_database(sqlite3 **db, const char *path)	//创建数据库
{
	int ret = 0;
	ret = sqlite3_open(path, db);
	if(ret != SQLITE_OK)
	{
		//kprintf(KRELEASE, E_DUMMY, "open database %s failed!!!\n", path);
		return -1;
	}
	ret = sqlite3_exec(*db, "PRAGMA cache_size = 8000", NULL, NULL, NULL);	//数据库缓存
	if(ret != SQLITE_OK)
	{
		//kprintf(KRELEASE, E_DUMMY, "PRAGMA cache_size to 8000 failed!!!\n");
		return -1;
	}
	ret = sqlite3_exec(*db, "PRAGMA auto_vacuum = 0", NULL, NULL, NULL);	//自动释放删除数据库后的空间，防止频繁操作NAND
	if(ret != SQLITE_OK)
	{
		//kprintf(KRELEASE, E_DUMMY, "PRAGMA auto_vacuum = 0 failed!!!\n");
		return -1;
	}
	ret = sqlite3_exec(*db, "PRAGMA temp_store = MEMORY", NULL, NULL, NULL);	//数据库索引等放在内存中，加快查询速度
	if(ret != SQLITE_OK)
	{
		//kprintf(KRELEASE, E_DUMMY, "PRAGMA temp_store = MEMORY failed!!!\n");
		return -1;
	}
	ret = sqlite3_exec(*db, "PRAGMA synchronous = FULL", NULL, NULL, NULL);		//同步模式，全速同步，确保异常断电后数据库文件不会损坏
	if(ret != SQLITE_OK)
	{
		//kprintf(KRELEASE, E_DUMMY, "PRAGMA synchronous = FULL failed!!!\n");
		return -1;
	}

	return 0;
}

bool HandleData::HandleSqlite3(string dbname,string name1, string data, int &id, string state, string time)
{
	int ret;
	sqlite3 *db;
	
	const char *name = dbname.c_str();
	//打开或者创建数据库
	ret = sqlite3_open(name, &db);
	if (ret)
	{
		cout << "sqlite3_open : "<< dbname << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	char sqq[100];
	memset(sqq, 0, sizeof(sqq));
	
	sprintf(sqq,"insert into chart_stm32 values(%d, '%s', '%s', '%s','%s');",id++,name1.c_str(),data.c_str(),state.c_str(),time.c_str());
	
	char sql[100];
	char *errmsg; //存储出错信息的指针
	memset(sql, 0, sizeof(sql));
	
	strcpy(sql,sqq);
	
	
	char **result;
	int nRow, nCol;
	#if 1
	ret = sqlite3_get_table(db, sql, &result, &nRow, &nCol, &errmsg);
	if (ret)
	{
		printf("get_table error: %s\n", errmsg);
		
		sqlite3_free(errmsg);
		sqlite3_close(db);
		return false;
	}
	#endif
	
	strcpy(sql, "select * from chart_stm32;");
	ret = sqlite3_get_table(db, sql, &result, &nRow, &nCol, &errmsg);
	if (ret)
	{
		printf("get_table error: %s\n", errmsg);
		
		sqlite3_free(errmsg);
		sqlite3_close(db);
		return false;
	}
	
	int i, j;
	int index  = 0;
	char buf[129];
	//STU s;
	printf("nRow: %d, nCol: %d\n", nRow, nCol);
	// result[0] ~ result[nCol-1]: 每一列的列名
	for (i = 0; i <= nRow; i++)
	{
		for (j = 0; j < nCol; j++)
		{
			printf("%s\t", result[index]);
			index++;
		}
		printf("\n");
		
		//memset(buf, 0, sizeof(buf));
	}
	
	sleep(1);
	sqlite3_close(db);
	
	return true;
}

bool HandleData::HandleMysql(string dbname,string name1, string data, int &id_cpu, int &id_tem, int &id_hum, string state, string time)
{
	MYSQL mysql;
    mysql_init(&mysql);
    MYSQL *ret = mysql_real_connect(&mysql, "129.204.249.80", "root", "eisoo.com123", "stm32", 3306, NULL, 0);
	if (!ret)  
	{  
		cout << "no" << endl; 
	}
	else
	{
		cout << "yes" << endl; 
	}
	char sqq[100];
	memset(sqq, 0, sizeof(sqq));
	if(name1 == "cpu")
	{
		sprintf(sqq,"insert into chart_cpu (id, name ,data ,time ,state) values(%d, '%s', '%s', '%s','%s');",id_cpu++,name1.c_str(),data.c_str(),time.c_str(),state.c_str());
	}
	else if(name1 == "tem")
	{
		sprintf(sqq,"insert into chart_tem (id, name ,data ,time ,state) values(%d, '%s', '%s', '%s','%s');",id_tem++,name1.c_str(),data.c_str(),time.c_str(),state.c_str());
	}
	else if(name1 == "hum")
	{
		sprintf(sqq,"insert into chart_hum (id, name ,data ,time ,state) values(%d, '%s', '%s', '%s','%s');",id_hum++,name1.c_str(),data.c_str(),time.c_str(),state.c_str());
	}
	mysql_query(&mysql, sqq);
	memset(sqq, 0, sizeof(sqq));
	
	sprintf(sqq,"SELECT * FROM cpu order by id DESC limit 1;");
	mysql_query(&mysql, sqq);
	mysql_close(&mysql);
	sleep(1);
}

/*
extern "C" 
{
	
	void GitData()
	{
		HandleData obj;
		obj.GitData();
	}
}
*/