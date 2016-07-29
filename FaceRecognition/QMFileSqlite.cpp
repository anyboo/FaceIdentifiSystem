#include "stdafx.h"
#include "QMFileSqlite.h"
#include <stdio.h>
#include <iostream>


using namespace Poco::Data::Keywords;
using namespace Poco::Data;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Statement;


QFileSqlite::QFileSqlite() :m_pool(NULL)
{
	Initialize();
	creatSessionPool();
}


QFileSqlite::~QFileSqlite()
{
	closeSessionPool();
	unInitialize();
}


QFileSqlite* QFileSqlite::getInstance()
{
	static QFileSqlite fileSqlite;
	return &fileSqlite;
}



bool QFileSqlite::Initialize()
{
	//setting session type
	try
	{
		SQLite::Connector::registerConnector();
	}
	catch (Poco::Exception &ex)
	{
		throw DatabaseException(ex.displayText());
		return false;
	}


	return true;
}


bool QFileSqlite::creatSessionPool()
{
	try
	{
		//create pool
		m_pool = new SessionPool(SQLite::Connector::KEY, "facerecog.db", 1, 100, 10);
		Session sess = connectDb();
		sess << "PRAGMA synchronous = OFF; ", now;
		if (!m_pool->isActive())
			throw DatabaseException("new session fail");
	}
	catch (Poco::Exception &ex)
	{
		throw DatabaseException(ex.displayText());
		return false;
	}

	return true;
}


Session QFileSqlite::connectDb()
{
	//get session
	Session sess(m_pool->get());
	if (!sess.isConnected())
		throw DatabaseException("session get error");

	return sess;
}


bool QFileSqlite::createTable(string sql)
{
	return execSql(sql);
}


bool QFileSqlite::cleanData(string sql)
{
	return execSql(sql);
}


bool QFileSqlite::dropTable(string sql)
{
	return execSql(sql);
}

void QFileSqlite::closeConnect(Session sess)
{
	if (sess.isConnected())
		sess.close();
}

bool QFileSqlite::checkConnect(Session sess)
{
	if (!sess.isConnected())
		return false;

	return true;
}

bool QFileSqlite::unInitialize()
{
	try
	{
		SQLite::Connector::unregisterConnector();
	}
	catch (Poco::Exception &ex)
	{
		throw DatabaseException(ex.displayText());
		return false;
	}

	return true;
}

void QFileSqlite::closeSessionPool()
{
	delete m_pool;
}

bool QFileSqlite::execSql(string sql)
{
	Session sess = connectDb();
	if (!checkConnect(sess))
		return false;
	try
	{
		if (sql.empty())
			return false;
		sess << sql , now;
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