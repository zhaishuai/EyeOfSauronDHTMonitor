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
    class ESDUdpError: public std::runtime_error {
    public:
        explicit ESDUdpError(const std:: string &what);
    };
    
    /**
     * 实现ESDAbstractUdp中的方法
     */
    class ESDUdp : ESDAbstractUdp{
    // 成员变量
    //
    private:
        
    protected:
        
        
        
    public:
        
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
        
        
//        /**
//         * @brief 绑定socket的接收地址
//         */
//        virtual void bindReciveAddress(std::string ipv4, int port, uv_udp_flags flag = UV_UDP_PARTIAL) override;
//        
//        /**
//         * @brief 接收UDP请求
//         * ipv4 ip地址；port 端口；callback 回调。
//         * 在调用之前需要先调用bindReciveAddress
//         */
//        virtual void receive(std::function<void(std::string)> callback) override;
//        
//        /**
//         * @brief 接收UDP请求并且可以设置超时时间
//         * ipv4 ip地址；port 端口；timeOut 超时时间；callback 回调。
//         */
//        virtual void receive(double timeOut, std::function<void(std::string)> callback) override;
//        
//        /**
//         * @brief 接收UDP请求
//         * ipv4 ip地址；port 端口；callback 回调。
//         * send以后调用该方法接收服务端的响应
//         */
//        virtual void receiveResponse(std::function<void(std::string)> callback) override;
//        
//        /**
//         * @brief 接收UDP请求并且可以设置超时时间
//         * ipv4 ip地址；port 端口；timeOut 超时时间；callback 回调。
//         * send以后调用该方法接收服务端的响应
//         */
//        virtual void receiveResponse(double timeOut, std::function<void(std::string)> callback) override;
//        
//        /**
//         * @brief 绑定socket的发送地址
//         */
//        virtual void bindSendAddress(std::string ipv4, int port, uv_udp_flags flag = UV_UDP_PARTIAL) override;
//        
//        /**
//         * @brief 发送UDP请求
//         * ipv4 ip地址；port 端口；callback 回调。
//         * 在调用之前需要先调用bindSendAddress
//         */
//        virtual void send(std::string, std::function<void(int status)> callback) override;
        
        
        
        
        //重新定义API
        /**
         * @brief 向给定的地址发送UDP请求
         * @param ipv4:ip地址 port:端口 sendcb:发送的回调 revcb:接收到响应的回调 timeout:超时时间 flag:udp的模式详情参见uv_udp_flags
         */
        virtual void send(std::string ipv4, int port, std::string msg, std::function<void(int status)> sendcb, std::function<void(std::string)> revcb, double timeout = 0, int flag = 0) override;
        /**
         * @brief 给定地址监听UDP请求
         * @param ipv4:ip地址 port:端口 revcb:接收请求回调 timeout:超时时间 flag:udp的模式详情参见uv_udp_flags
         */
        virtual void receive(std::string ipv4, int port, std::function<void(std::string)> revcb, double timeout = 0, int flag = 0) override;
        
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
