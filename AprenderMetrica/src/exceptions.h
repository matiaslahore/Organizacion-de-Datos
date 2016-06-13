#ifndef _exceptions_
#define _exceptions_
#include <exception>
#include <string>
/** 
 * Representa una excepción que modifica errno.
 * */
class CException : public std::exception {
	protected:
	std::string constructed_message;
	virtual std::string osMessage();
	public:
	virtual const char* what() const throw();
	explicit CException(const std::string& custom_message);
	~CException() throw();
	CException();
};
/**
 * Representa una excepción produceto de un error en 
 * getAddressInfo.
 * */
class GaiException : public CException {
	public:
	GaiException(const std::string& custom_message, int code);
	explicit GaiException(const std::string& custom_message);
	~GaiException() throw();
};
/**
 * Lanzada sólo cuando falla el accept() sobre un socket.
 * */
class AcceptException : public CException {
	public:
	AcceptException():CException("Error al aceptar una conexion"){}
};

/**
 * Lanzada sólo cuando receive() sobre un socket no obtiene nada
 * */
class RecvException : public CException {
	public:
	RecvException():CException("El buffer de lectura estaba vacio"){}
};
/**
 * Representa una excepción generada por los entes del modelo.
 * */
class CustomException : public std::exception {
	private:
	std::string message;
	public:
	virtual const char* what() const throw();
	explicit CustomException(const std::string& custom_message);
	~CustomException() throw();
};

class RecvTimeOutException: public CustomException{
	public: 
	RecvTimeOutException():CustomException("Timeout al hacer Recv"){};
};


#endif
