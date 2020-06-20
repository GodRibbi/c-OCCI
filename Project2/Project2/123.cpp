#include<iostream>
#include <occi.h>

using namespace std;
using namespace oracle::occi;




#pragma region 时间转换函数
#include <ctime>
#include <stdlib.h>  
#pragma warning( disable : 4996 )
//获得现在的时间
string time_getnow();
//获得时间string
string time_getstr(time_t time);
//两个时间差几天,time1被减数，time2减数
int time_difftime(string time1, string time2);
//时间变为string
time_t StringToDatetime(const char* str);
//时间变为可被oracle识别的类型
string changeDateStr(string date);
//时间加上天数
string time_addday(string date, int day);
string changeDateStr(string date) {
	int year, month, day;
	string month1, day1;
	const char* p2 = date.c_str();
	sscanf(p2, "%d-%d-%d", &year, &month, &day);
	if (month < 10) {
		month1 ="0" + to_string(month);
	}
	else {
		month1 =to_string(month);
	}
	if (day < 10) {
		day1 = "0" + to_string(day);
	}
	else {
		day1 =to_string(day);
	}
	string p = to_string(year) + "-" + month1 + "-" + day1;
	string str = "to_date('" + p + "','yyyy-mm-dd')";
	return str;
}
string time_addday(string date, int day)
{
	const char* p1 = date.c_str();
	time_t tm1 = StringToDatetime(p1);
	tm1 += day * 24 * 60 * 60;
	return time_getstr(tm1);
}
string time_getnow() {
	string a;
	time_t now = time(0);
	struct tm* ltm = localtime(&now);
	a = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);
	return a;
}

string time_getstr(time_t time)
{
	string a;
	struct tm* ltm = localtime(&time);
	a = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);
	return a;
}

int time_difftime(string time1, string time2)
{
	const char* p1 = time1.c_str();
	const char* p2 = time2.c_str();
	time_t tm1 = StringToDatetime(p1);
	time_t tm2 = StringToDatetime(p2);
	double sed = 24 * 60 * 60;


	int a = difftime(tm1, tm2) / sed;
	//cout << time_getstr(tm1) << endl;
	//cout << difftime(tm1, tm2) << endl;

	return a;
}

time_t StringToDatetime(const char* str)
{
	tm tm_;
	int year, month, day;
	sscanf(str, "%d-%d-%d", &year, &month, &day);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = 0;
	tm_.tm_min = 0;
	tm_.tm_sec = 0;
	tm_.tm_isdst = 0;

	time_t t_ = mktime(&tm_); //已经减了8个时区
	return t_; //秒时间
}
#pragma endregion

void updateSql(string uri = "SELECT * FROM RECORDS") {
	Environment* env = Environment::createEnvironment();
	string name = "system";
	string pass = "123456";
	string srvName = "127.0.0.1:1521/xe";
	try
	{
		//创建数据库连接
		Connection* conn = env->createConnection(name, pass, srvName);//用户名，密码，数据库名
		Statement* pStmt = NULL;    // Statement对象
		pStmt = conn->createStatement();
		ResultSet* pRs = 
		pStmt->executeQuery(uri);
		while (pRs->next()) {
			string* str = new string(pRs->getDate(4).toText("YYYY-MM-DD"));
			cout << *str << endl;
		}
		conn->terminateStatement(pStmt);
		env->terminateConnection(conn);
		
	}
	catch (SQLException e)
	{
		cout << e.what() << endl;
		system("pause");
	}
}



int main()
{
	updateSql();
	string today = "2020-6-18";
	string should_return = "2020-6-28";
	cout << time_addday(today, 10);
	cout << changeDateStr(time_getnow());
	cout << time_difftime(today, should_return);
}



