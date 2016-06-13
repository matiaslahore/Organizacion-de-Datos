#include "exceptions.h"
#include <errno.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <string>
using std::string;
const char* CException::what() const throw(){
	return constructed_message.c_str();
}
CException::CException(){}
CException::CException(const std::string& custom_message){
	std::string os_message = osMessage();
	
	constructed_message = "Os message: "+os_message;
	constructed_message += "\ncustom message: "+custom_message;
}
CException::~CException() throw(){}

std::string CException::osMessage(){
	return std::string(strerror(errno));
}

GaiException::GaiException(const std::string& custom_message, int code){
	std::string os_message(gai_strerror(code));
	
	constructed_message = "Os message: "+os_message;
	constructed_message += "\ncustom message: "+custom_message;
}
GaiException::~GaiException() throw() {}

const char* CustomException::what() const throw(){
	return message.c_str();
}
CustomException::CustomException(const std::string& custom_message){
	this->message = custom_message;
}
CustomException::~CustomException() throw() {}
