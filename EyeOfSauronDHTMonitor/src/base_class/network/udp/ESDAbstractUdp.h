//
//  ESDAbstractUdp.h
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/24/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDAbstractUdp_hpp
#define ESDAbstractUdp_hpp

#include <stdio.h>

namespace esdht {
    
    /**
     * @brief 规定ESDUdp的接口
     * 在编写UDP服务端时，只需要使用receive()和response()方法。
     * 在编写UDP客户端时，只需要使用send()和receive()方法。
     */
    class ESDAbstractUdp : ESDObject{
    public:
        /**
         * @brief 绑定socket的接收地址
         */
        virtual void bindReciveAddress(std::string ipv4, int port, uv_udp_flags flag = UV_UDP_PARTIAL) = 0;
        
        /**
         * @brief 接收UDP请求
         * ipv4 ip地址；port 端口；callback 回调。
         * 在调用之前需要先调用bindReciveAddress
         */
        virtual void receive(std::function<void(std::string)> callback) = 0;
        
        /**
         * @brief 接收UDP请求并且可以设置超时时间
         * ipv4 ip地址；port 端口；timeOut 超时时间；callback 回调。
         */
        virtual void receive(double timeOut, std::function<void(std::string)> callback) = 0;
        
        /**
         * @brief 接收UDP请求
         * ipv4 ip地址；port 端口；callback 回调。
         * send以后调用该方法接收服务端的响应
         */
        virtual void receiveResponse(std::function<void(std::string)> callback) = 0;
        
        /**
         * @brief 接收UDP请求并且可以设置超时时间
         * ipv4 ip地址；port 端口；timeOut 超时时间；callback 回调。
         * send以后调用该方法接收服务端的响应
         */
        virtual void receiveResponse(double timeOut, std::function<void(std::string)> callback) = 0;
        
        /**
         * @brief 绑定socket的发送地址
         */
        virtual void bindSendAddress(std::string ipv4, int port, uv_udp_flags flag = UV_UDP_PARTIAL) = 0;
        
        /**
         * @brief 发送UDP请求
         * ipv4 ip地址；port 端口；callback 回调。
         * 在调用之前需要先调用bindSendAddress
         */
        virtual void send(std::string, std::function<void(int status)> callback) = 0;
        
        /**
         * @brief 响应UDP请求
         * callback 回调。
         */
        virtual void response(std::function<void(int status)> callback) = 0;
        
    };
    
}

#endif /* ESDAbstractUdp_hpp */
