/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef MYSQLDBCONNECTION_H
#define MYSQLDBCONNECTION_H

#include "base/array.h"
#include "base/dynamictype.h"
#include "base/timer.h"
#include "ido/dbconnection.h"
#include <mysql/mysql.h>

namespace icinga
{

/**
 * A MySQL database connection.
 *
 * @ingroup ido
 */
class MysqlDbConnection : public DbConnection
{
public:
	typedef shared_ptr<MysqlDbConnection> Ptr;
	typedef weak_ptr<MysqlDbConnection> WeakPtr;

	MysqlDbConnection(const Dictionary::Ptr& serializedUpdate);
	virtual void Stop(void);

	virtual void UpdateObject(const DbObject::Ptr& dbobj, DbUpdateType kind);

private:
	Attribute<String> m_Host;
	Attribute<long> m_Port;
	Attribute<String> m_User;
	Attribute<String> m_Password;
	Attribute<String> m_Database;
	Attribute<String> m_InstanceName;
	Attribute<String> m_InstanceDescription;

	DbReference m_InstanceID;

	boost::mutex m_ConnectionMutex;
	bool m_Connected;
	MYSQL m_Connection;

	Timer::Ptr m_ReconnectTimer;
	Timer::Ptr m_TxTimer;

	Array::Ptr Query(const String& query);
	DbReference GetInsertID(void);
	String Escape(const String& s);
	Dictionary::Ptr FetchRow(MYSQL_RES *result);

	void TxTimerHandler(void);
	void ReconnectTimerHandler(void);
};

}

#endif /* MYSQLDBCONNECTION_H */
