#pragma once

#include <iostream>
#include <fstream>

#include <streambuf>
#include <string>

#include <QDebug>
#include <QObject>


class RLOStreamIntercept;
class RLOStream : public QObject, public std::ofstream {
	Q_OBJECT
public:
	explicit RLOStream(const char* filename,ios_base::openmode mode = ios_base::out);
	explicit RLOStream(std::string filename,ios_base::openmode mode = ios_base::out);
	explicit RLOStream(QString filename,ios_base::openmode mode = ios_base::out);
	
	~RLOStream();
	
	void openFile(const char* filename,ios_base::openmode mode = ios_base::out);
	void openFile(std::string filename,ios_base::openmode mode = ios_base::out);
	void openFile(QString filename,ios_base::openmode mode = ios_base::out);
	void closeFile();
	
signals:
	void sendInterceptedData(QString data);
	
private:
	void init_();
	
	RLOStreamIntercept* interceptor_;
	
};

class RLOStreamIntercept : public QObject, public std::basic_streambuf<char> {
	Q_OBJECT
public:
	explicit RLOStreamIntercept(std::ostream& stream);
	~RLOStreamIntercept();
	
protected:
	virtual int_type overflow(int_type c);
	virtual std::streamsize xsputn(const char* p, std::streamsize n);
	
signals:
	void sendInterceptedData(QString data);
	
private:
	void sendString_(std::string& str);
	
	std::ostream& mStream_;
	std::string mString_;
	std::basic_streambuf<char>* mStreamOldBuffer_;
	
};

namespace std {
std::ostream& endl(RLOStream& os);
}
