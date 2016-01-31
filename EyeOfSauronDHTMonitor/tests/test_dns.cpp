//
//  test_dns.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/31/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_dns.hpp"

using namespace esdht;

namespace test_dns {
    void run_test_dns(){
        
        clock_t one, two;
        one = clock();   // 开始计时
        
        {
            ESDDns dns2;
        }
        
        try {
            
            ESDDns dns;
            
            for(int i = 0 ; i < 10000 ; i++){
                dns.getIpOfURL("baidu.com","80", [&i](int state, std::string result){
                    printf("i:%d   %s", i, result.c_str());
                });
            }
            two= clock();   // 结束计时
            std::cout << "Runtime： " << (double)(two- one ) * 1000.0 / CLOCKS_PER_SEC << " ms!" << std::endl;
        }catch (const ESDDnsError &error){
            std::cerr << "error: " << error.what() << "\n";
        }
        
    }
}

