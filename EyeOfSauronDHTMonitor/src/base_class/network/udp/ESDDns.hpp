//
//  ESDDns.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/24/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDDns_hpp
#define ESDDns_hpp

#include <stdio.h>

namespace esdht {
    
    /**
     * @brief 当DNS解析出现错误的时候抛出异常
     */
    CREATE_ERROR_CLASS(ESDDnsError)
    
    /**
     * @brief DNS解析，获取给定域名的ip地址
     */
    class ESDDns : ESDObject{
    // 成员变量
    //
    private:
        
    protected:
        
        uv_loop_t*       loop;
        struct addrinfo  hints;
        uv_getaddrinfo_t resolver;
        
    public:
        
        std::function<void(int status, std::string address)> handleIPCallback;
        
    // 成员函数
    //
    private:
        
    protected:
        
    public:
        /**
         * @brief 初始化ESDDns
         */
        ESDDns();
        
        ~ESDDns();
        
        /**
         * @brief 获取给定地址的ip
         */
        void getIpOfURL(std::string url,std::string port, std::function<void(int status, std::string address)> callback);
    };
    
}

#endif /* ESDDns_hpp */
