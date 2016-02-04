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
    class ESDAbstractUdp : public ESDObject{
    public:
        
        /**
         * @brief 设置发送端口号
         * @param port 端口号
         */
        virtual void setSendPort(int port) = 0;
        
        /**
         * @brief 向给定的地址发送UDP请求
         * @param ipv4:ip地址 port:端口 msg:要发送的消息 sendcb:发送的回调 revcb:接收到响应的回调 timeout:超时时间 flag:udp的模式详情参见uv_udp_flags
         */
        virtual void send(std::string ipv4, int port, std::string msg, std::function<void(int status)> sendcb, std::function<void(std::string)> revcb, double timeout = 0, int flag = 0) = 0;
        
        
        /**
         * @brief 给定地址监听UDP请求
         * @param ipv4:ip地址 port:端口 revcb:接收请求回调 timeout:超时时间 flag:udp的模式详情参见uv_udp_flags
         */
        virtual void receive(std::string ipv4, int port, std::function<void(std::string)> revcb, int flag = 0) = 0;
        
        /**
         * @brief 停止receive
         */
        virtual void stopReceive() = 0;
        
        /**
         * @brief 响应UDP请求
         * callback 回调。
         */
        virtual void response(std::string msg, std::function<void(int status)> callback) = 0;
    };
    
}

#endif /* ESDAbstractUdp_hpp */
