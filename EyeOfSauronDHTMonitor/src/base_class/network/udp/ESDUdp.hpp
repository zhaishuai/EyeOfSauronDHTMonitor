//
//  ESDUdp.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/24/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDUdp_hpp
#define ESDUdp_hpp

#include <stdio.h>
#include "ESDAbstractUdp.h"
namespace esdht {
    
    /**
     * @brief 当UDP出现错误的时候抛出异常
     */

    CREATE_ERROR_CLASS(ESDUdpError)
    
    /**
     * 实现ESDAbstractUdp中的方法
     */
    class ESDUdp : public ESDAbstractUdp{
    // 成员变量
    //
    private:
        
        uv_loop_t                        defaultLoop;
        
    protected:
        
        uv_udp_t                         socket;
        
    public:
        uv_timer_t                       timer;
        uv_loop_t*                       loop;
        uv_loop_t*                       receiveLoop;
        uv_udp_t                         sendSocket;
        uv_udp_t                         receiveSocket;
        uv_udp_send_t                    sendRequest;
        uv_udp_send_t                    responseRequest;
        struct sockaddr_in               sendAddr;
        struct sockaddr_in               recvAddr;
        
        struct sockaddr*                 responseAddr;
        std::function<void(std::string)> receiveCallback = nullptr;
        std::function<void(std::string)> receiveWithTimeOutCallback = nullptr;
        std::function<void(std::string)> receiveResponseCallback = nullptr;
        std::function<void(std::string)> receiveResponseWithTimeOutCallback = nullptr;
        std::function<void(int status)>  sendCallback = nullptr;
        std::function<void(int status)>  responseCallback = nullptr;
        
    // 成员函数
    //
    private:

    protected:
        
    public:
        
        ESDUdp();
        
        ESDUdp(uv_loop_t *loop);
        
        ~ESDUdp();
        
        /**
         * @brief 设置发送端口号
         * @param port 端口号
         */
        virtual void setSendPort(unsigned int port) override;
        
        /**
         * @brief 向给定的地址发送UDP请求
         * @param ipv4:ip地址 port:端口 sendcb:发送的回调 revcb:接收到响应的回调 timeout:超时时间 flag:udp的模式详情参见uv_udp_flags
         */
        virtual void send(std::string ipv4, unsigned int port, std::string msg, std::function<void(int status)> sendcb, std::function<void(std::string)> revcb, double timeout = 5000, int flag = UV_UDP_REUSEADDR) override;
        
        void licensingResponse(std::function<void (std::string response)> func);
        
        void sendAsync(std::string ipv4, unsigned int port, std::string msg, std::function<void(int status)> sendcb, int flag = UV_UDP_REUSEADDR);
        
        /**
         * @brief 给定地址监听UDP请求
         * @param ipv4:ip地址 port:端口 revcb:接收请求回调 timeout:超时时间 flag:udp的模式详情参见uv_udp_flags
         */
        virtual void receive(std::string ipv4, unsigned int port, std::function<void(std::string)> revcb, int flag = 0) override;
        
        /**
         * @brief 停止receive
         */
        virtual void stopReceive() override;
        
        /**
         * @brief 响应UDP请求
         * callback 回调。
         */
        virtual void response(std::string msg, std::function<void(int status)> callback) override;
    };
    
}


#endif /* ESDUdp_hpp */
