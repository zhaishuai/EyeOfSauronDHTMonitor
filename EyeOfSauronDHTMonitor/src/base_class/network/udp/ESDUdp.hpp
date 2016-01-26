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
        
        uv_loop_t*                       loop;
        uv_udp_t                         socket;
        uv_udp_send_t                    sendRequest;
        struct sockaddr_in               sendAddr;
        struct sockaddr_in               recvAddr;
        
    public:
        
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
        
        ~ESDUdp();
        
        
        /**
         * @brief 绑定socket的接收地址
         */
        virtual void bindReciveAddress(std::string ipv4, int port, uv_udp_flags flag = UV_UDP_PARTIAL) override;
        
        /**
         * @brief 接收UDP请求
         * ipv4 ip地址；port 端口；callback 回调。
         * 在调用之前需要先调用bindReciveAddress
         */
        virtual void receive(std::function<void(std::string)> callback) override;
        
        /**
         * @brief 接收UDP请求并且可以设置超时时间
         * ipv4 ip地址；port 端口；timeOut 超时时间；callback 回调。
         */
        virtual void receive(double timeOut, std::function<void(std::string)> callback) override;
        
        /**
         * @brief 接收UDP请求
         * ipv4 ip地址；port 端口；callback 回调。
         * send以后调用该方法接收服务端的响应
         */
        virtual void receiveResponse(std::function<void(std::string)> callback) override;
        
        /**
         * @brief 接收UDP请求并且可以设置超时时间
         * ipv4 ip地址；port 端口；timeOut 超时时间；callback 回调。
         * send以后调用该方法接收服务端的响应
         */
        virtual void receiveResponse(double timeOut, std::function<void(std::string)> callback) override;
        
        /**
         * @brief 绑定socket的发送地址
         */
        virtual void bindSendAddress(std::string ipv4, int port, uv_udp_flags flag = UV_UDP_PARTIAL) override;
        
        /**
         * @brief 发送UDP请求
         * ipv4 ip地址；port 端口；callback 回调。
         * 在调用之前需要先调用bindSendAddress
         */
        virtual void send(std::string, std::function<void(int status)> callback) override;
        
        /**
         * @brief 响应UDP请求
         * callback 回调。
         */
        virtual void response(std::function<void(int status)> callback) override;
        
        
        
    };
    
}


#endif /* ESDUdp_hpp */
