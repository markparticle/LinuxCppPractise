/**
 * server class
 * 
 * @author mark
 * @date 2020-5-16
 * @copyleft GPL 2.0
 * */

#ifndef _SERVER_H_
#define _SERVER_H_

namespace rtc{

class Server{
    public:
        Server();
        ~Server();
    public:
        void run();
};

} //namespace 

#endif //_SERVER_H_