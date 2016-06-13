#include "Thread.h"
#include <exception>
#include <iostream>
#include "exceptions.h"
void Thread::start() {
	int c_result = pthread_create(&thread, NULL, Thread::runner, this);
	if(c_result != 0)
		throw CException("Error al abrir el thread");
}

void* Thread::runner(void *data) {
	try{
		Thread *self = (Thread*)data;
		self->run();
	}catch(const std::exception& e){
		std::cerr<<"Error: "<< e.what()<<std::endl;
	}catch(...){
		std::cerr<<"Error desconocido."<<std::endl;
	}
	return NULL;
}
        
void Thread::join() {
	std::cout<<"iniciando join en Thread"<<std::endl;
	this->end();
	int j_result = pthread_join(thread, NULL);
	if(j_result != 0){
		std::cerr<<"Error al joinear el thread."<<std::endl;
		throw CException("Error al joinear el thread");
		
	}
	std::cout<<"terminando join en Thread"<<std::endl;
}

Thread::~Thread(){}
