#ifndef DBCONN_H
#define DBCONN_H

class DbConn
{
private:
	Connection* connection;

public:
	DbConn();
	~DbConn();
	
	PreparedStatement* prepareStatement(string query);
};

#endif