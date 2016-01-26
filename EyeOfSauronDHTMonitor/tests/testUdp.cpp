//
//  testUdp.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/26/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "testUdp.hpp"

#include "ESDUdp.hpp"

using namespace esdht;
using namespace std;

namespace test_udp{
    
    void start_test(){
        
        ESDUdp udp;
        udp.bindSendAddress("127.0.0.1", 6882);
//            printf("sucess %d\n", i);
        for(int i = 0 ; i < 100000 ; i++){
            udp.send("hello word ni hao shi jie\n", [i](int status){
                
                if(status == -1){
                    fprintf(stderr, "Send error!\n");
                }
                printf("sucess %d\n", i);
            });
            
            udp.receiveResponse([i](std::string msg){
                printf("msg:%s i:%d", msg.c_str(), i);
            });
            
        }//for
        
        printf("dddd");
    }
}