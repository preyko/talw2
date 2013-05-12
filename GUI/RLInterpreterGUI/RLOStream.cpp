#include "RLOStream.h"

// Overload std::endl for RLOStream and remove flush after '\n' inserted into ostream
namespace std {
std::ostream& endl(RLOStream& os) {
	os << "\n";
	return os;
};
}

RLOStream::RLOStream(const char* filename, std::ios_base::openmode mode) :
	QObject(0),
	std::ofstream(),
	interceptor_(NULL) {
	openFile(filename,mode);
}

RLOStream::RLOStream(std::string filename, ios_base::openmode mode) :
	QObject(0),
	std::ofstream(),
	interceptor_(NULL) {
	openFile(filename,mode);
}

RLOStream::RLOStream(QString filename, ios_base::openmode mode) : 
	QObject(0),
	std::ofstream(),
	interceptor_(NULL) {
	openFile(filename,mode);
}

RLOStream::~RLOStream() {
	closeFile();
}

void RLOStream::openFile(const char* filename, std::ios_base::openmode mode) {
	closeFile();
	open(filename,mode);
	init_();
}

void RLOStream::openFile(std::string filename, std::ios_base::openmode mode) {
	openFile(filename.c_str(),mode);
}

void RLOStream::openFile(QString filename, std::ios_base::openmode mode) {
	openFile(filename.toStdString(),mode);
}

void RLOStream::closeFile() {
	close();
	if(interceptor_ != NULL) {
		delete interceptor_;
		//disconnect(interceptor_,SIGNAL(sendInterceptedData(QString)),this,SIGNAL(sendInterceptedData(QString)));
	}
}


void RLOStream::init_() {
	interceptor_ = new RLOStreamIntercept(*this);
	
	connect(interceptor_,SIGNAL(sendInterceptedData(QString)),SIGNAL(sendInterceptedData(QString)));
}


/*
 * RLOStream intercepter
 */
RLOStreamIntercept::RLOStreamIntercept(std::ostream& stream) : mStream_(stream), QObject(0) {
	mStreamOldBuffer_ = stream.rdbuf();
	stream.rdbuf(this);
}

RLOStreamIntercept::~RLOStreamIntercept() {
	// Output anything that is left
	if(!mString_.empty())
		sendString_(mString_);
	
	mStream_.rdbuf(mStreamOldBuffer_);
}


RLOStreamIntercept::int_type RLOStreamIntercept::overflow(RLOStreamIntercept::int_type v) {
	if(v == '\n') {
		sendString_(mString_);
		mString_.erase(mString_.begin(),mString_.end());
	} else {
		mString_ += v;
	}
	
	return v;
}

std::streamsize RLOStreamIntercept::xsputn(const char* p,std::streamsize n) {
	mString_.append(p,p+n);
	
	uint pos = 0;
	while(pos != std::string::npos) {
		pos = mString_.find('\n');
		if(pos != std::string::npos) {
			std::string tmp(mString_.begin(),mString_.begin() + pos);
			emit sendInterceptedData(QString::fromStdString(tmp));
			mString_.erase(mString_.begin(),mString_.begin() + pos + 1);
		}
	}
	
	return mStreamOldBuffer_->sputn(p,n);
}

void RLOStreamIntercept::sendString_(std::string &str) {
	emit sendInterceptedData(QString::fromStdString(mString_));
	mStreamOldBuffer_->sputn(str.c_str(),str.size()*sizeof(char));
}
