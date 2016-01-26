//
//  main.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/23/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include <iostream>
#include <uv.h>
#include "document.h"

#include "ESDDns.hpp"

#include "testUdp.hpp"


using namespace rapidjson;
using namespace esdht;

char *json = (char *)"{\"hello\": \"world\",\"t\": true ,\"f\": false,\"n\": null,\"i\": 123,\"pi\": 3.1416,\"a\": [1, 2, 3, 4]}";

int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    clock_t one, two;
//    one = clock();   // 开始计时
    //    Document document;
    //    document.Parse(json);
    
    //    printf("%s\n",document["hello"].GetString());
    
//    {
//        ESDDns dns2;
//    }
    
//    try {
//        
//        ESDDns dns;
//        
//        
//        for(int i = 0 ; i < 10000 ; i++){
//            dns.getIpOfURL("baidu.com","80", [&i](int state, std::string result){
//                printf("i:%d   %s", i, result.c_str());
//            });
//        }
//        two= clock();   // 结束计时
//        std::cout << "Runtime： " << (double)(two- one ) * 1000.0 / CLOCKS_PER_SEC << " ms!" << std::endl;
//    }catch (const ESDDnsError &error){
//        std::cerr << "error: " << error.what() << "\n";
//    }
    
    test_udp::start_test();
    
    return 0;
}
