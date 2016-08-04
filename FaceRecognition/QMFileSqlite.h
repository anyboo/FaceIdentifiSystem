#ifndef QM_FILE_SQLITE_H
#define QM_FILE_SQLITE_H


#include "Poco/Tuple.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include <vector>
#include "DatabaseException.h"

using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Statement;

#define CREATE_USER_INFO_TABLE              "CREATE TABLE IF NOT EXISTS UserInfo(id INTEGER PRIMARY KEY, \
											name VARCHAR(50), age INTEGER, gender VARCHAR(10), birthday VARCHAR(30), \
											address VARCHAR(100), phone VARCHAR(20), certificate VARCHAR(30), audit BOOL, image BLOB)"
#define DROP_USER_INFO_TABLE				"DROP TABLE IF EXISTS UserInfo"
#define DELETE_USER_INFO_TABLE				"DELETE from UserInfo"
#define SELECT_ALL_USER_INFO				"select * from UserInfo"
#define SELECT_COUNT_USER_INFO				"select count(*) from UserInfo"
#define INSERT_USET_INFO					"INSERT INTO UserInfo (name, age, gender, birthday, address, phone, certificate, audit, image) \
											VALUES(:name, :age, :gender, :birthday, :address, :phone, :certificate, :audit, :image)"
typedef Poco::Tuple<int, std::string, int, std::string, std::string, std::string, std::string, std::string, bool, Poco::Data::CLOB> readUserInfo;
typedef Poco::Tuple<std::string, int, std::string, std::string, std::string, std::string, std::string, bool, Poco::Data::CLOB> writeUserInfo;

#define CREATE_COMPARE_INFO_TABLE           "CREATE TABLE IF NOT EXISTS CompareInfo(id INTEGER PRIMARY KEY, num INTEGER, time VARCHAR(30), similarity float, picture BLOB)"
#define DROP_COMPARE_INFO_TABLE				"DROP TABLE IF EXISTS CompareInfo"
#define DELETE_COMPARE_INFO					"DELETE from CompareInfo"
#define SELECT_ALL_COMPARE_INFO				"SELECT * FROM CompareInfo"
#define INSERT_COMPARE_INFO					"INSERT INTO CompareInfo (num, time, similarity, picture) VALUES(:num, :time, :similarity, :picture)"
typedef Poco::Tuple<int, int, std::string, float, Poco::Data::CLOB> readCompareInfo;
//typedef Poco::Tuple<int, std::string, float, Poco::Data::CLOB> writeCompareInfo;
typedef Poco::Tuple<bool, int> writeCompareInfo;

class QFileSqlite
{
public:

	static QFileSqlite *getInstance();
private:
	QFileSqlite();
	~QFileSqlite();
	QFileSqlite(QFileSqlite const& other);
	QFileSqlite& operator=(QFileSqlite const& other);


public:
	template<typename T>
	bool GetData(string sql, std::vector<T>& Record)
	{
		if (sql.empty())
			return false;
		Session sess = connectDb();
		if (!checkConnect(sess))
			return false;
		try
		{
			Statement select(sess);
			select << sql, Poco::Data::Keywords::into(Record), Poco::Data::Keywords::now;
			closeConnect(sess);
		}
		catch (Poco::Exception &ex)
		{
			throw DatabaseException(ex.displayText());
			closeConnect(sess);
			return false;
		}

		return true;
	}
	template<typename T>
	bool writeData(string sql, T searchrecode)
	{
		if (sql.empty())
			return false;
		Session sess = connectDb();
		if (!checkConnect(sess))
			return false;
		try
		{
			sess << sql, Poco::Data::Keywords::use(searchrecode), Poco::Data::Keywords::now;
			closeConnect(sess);
		}
		catch (Poco::Exception &ex)
		{
			throw DatabaseException(ex.displayText());
			closeConnect(sess);
			return false;
		}

		return true;
	}

	template<typename T>
	bool writeDataByVector(string sql, std::vector<T>& Record)
	{
		if (sql.empty() || Record.empty())
			return false;
		Session sess = connectDb();
		if (!checkConnect(sess))
			return false;
		try
		{
			Statement insert(sess);
			insert << sql, Poco::Data::Keywords::use(Record), Poco::Data::Keywords::now;
			closeConnect(sess);
		}
		catch (Poco::Exception &ex)
		{
			throw DatabaseException(ex.displayText());
			closeConnect(sess);
			return false;
		}
		return true;
	}
	bool cleanData(string sql);
	bool dropTable(string sql);
	bool createTable(string sql);
private:
	bool Initialize();
	bool creatSessionPool();
	void closeConnect(Session sess);
	bool checkConnect(Session sess);
	bool unInitialize();
	void closeSessionPool();
	bool execSql(string sql);
	Session connectDb();
	Poco::Data::SessionPool *m_pool;

};


#endif 