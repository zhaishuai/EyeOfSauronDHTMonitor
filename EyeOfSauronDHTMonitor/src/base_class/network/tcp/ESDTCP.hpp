//
//  ESDTCP.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/19/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDTCP_hpp
#define ESDTCP_hpp

#include <stdio.h>

namespace esdht {
    /**
     * @brief 当TCP出现错误的时候抛出异常
     */
    CREATE_ERROR_CLASS(ESDTcpError)
    
    class ESDTcp : public ESDObject{
    protected:
        
        uv_tcp_t clientSocket;
        uv_tcp_t serverSocket;
        
    public:
        
        uv_loop_t clinetLoop;
        uv_loop_t serverLoop;
        
        
        struct sockaddr_in               sendAddr;
        struct sockaddr_in               recvAddr;
        
        struct sockaddr*                 responseAddr;
        std::function<void(std::string msg, uv_stream_t* stream)> receiveCallback = nullptr;
        std::function<void(std::string)> receiveWithTimeOutCallback = nullptr;
        std::function<void(std::string)> receiveResponseCallback = nullptr;
        std::function<void(std::string)> receiveResponseWithTimeOutCallback = nullptr;
        std::function<void(int status)>  sendCallback = nullptr;
        std::function<void(int status)>  responseCallback = nullptr;
        
        
        ESDTcp();
        ESDTcp(uv_loop_t *loop);
        ~ESDTcp();
        
        /**
         * @brief 设置发送端口号
         * @param port 端口号
         */
        virtual void setSendPort(int port);
        
        /**
         * @brief 向给定的地址发送UDP请求
         * @param ipv4:ip地址 port:端口 sendcb:发送的回调 revcb:接收到响应的回调 timeout:超时时间 flag:udp的模式详情参见uv_udp_flags
         */
        virtual void send(std::string ipv4, int port, std::string msg, std::function<void(int status)> sendcb, std::function<void(std::string)> revcb, double timeout = 5000, int flag = UV_UDP_REUSEADDR);
        
        void licensingResponse(std::function<void (std::string response)> func);
        
        void sendAsync(std::string ipv4, int port, std::string msg, std::function<void(int status)> sendcb, int flag = UV_UDP_REUSEADDR);
        
        /**
         * @brief 给定地址监听UDP请求
         * @param ipv4:ip地址 port:端口 revcb:接收请求回调 timeout:超时时间 flag:udp的模式详情参见uv_udp_flags
         */
        virtual void receive(std::string ipv4, int port, std::function<void(std::string, uv_stream_t* stream)> revcb, int flag = 0);
        
        /**
         * @brief 停止receive
         */
        virtual void stopReceive() ;
        
        /**
         * @brief 响应UDP请求
         * callback 回调。
         */                                                                                       
        virtual void response(std::string msg, uv_stream_t* stream, std::function<void (int state)> callback) ;
        
    };
    
}





#endif /* ESDTCP_hpp */
