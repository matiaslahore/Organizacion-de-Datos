#ifndef _THREAD_
#define _THREAD_
#include <pthread.h>
/**
 * Representa un hilo genérico
 * */
class Thread {
    private:
        pthread_t thread;
        /**
         * Corre el método run en otro thread y atrapa cualquier excepción que lance
         * */
        static void *runner(void *data);
	protected:
        virtual void run() = 0;
		virtual void end() = 0;
        
    public:
    /**
     * inicia el thread
     * */
        void start();
        /**
         * Llama al método end() y espera que run() termine.
         * */
        void join();
		virtual ~Thread();
};
#endif
