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

//login infomation table
#define CREATE_LOGIN_DEVICE_INFO_TABLE      "CREATE TABLE LoginDeviceInfo(type INTEGER, ip VARCHAR(30), port INTEGER, username VARCHAR(50), password VARCHAR(50), maxchannel INTEGER)"
#define DROP_LOGIN_DEVICE_INFO_TABLE		"DROP TABLE IF EXISTS LoginDeviceInfo"
#define DELETE_ALL_LOGIN_DEVICE_INFO		"DELETE from LoginDeviceInfo"
#define DELETE_IP_LOGIN_DEVICE				"DELETE from LoginDeviceInfo where ip="
#define SELECT_ALL_LOGIN_DEVICE_INFO		"SELECT * FROM LoginDeviceInfo"
#define SELECT_IP_LOGIN_VIDEO				"SELECT * FROM LoginDeviceInfo where ip="
#define INSERT_LOGIN_DEVICE_INFO			"INSERT INTO LoginDeviceInfo VALUES(:type, :ip, :port, :username, :password, :maxchannel)"
typedef Poco::Tuple<int, std::string, int, std::string, std::string, int> LoginDeviceInfo;

#define CREATE_LOGIN_DEVICE_INFO_TABLE

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