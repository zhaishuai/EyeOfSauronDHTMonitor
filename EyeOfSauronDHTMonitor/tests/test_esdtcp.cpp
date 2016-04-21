//
//  test_esdtcp.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/19/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esdtcp.hpp"
#include "ESDTCP.hpp"
#include "ThreadPool.hpp"
#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

namespace test_esdtcp {
    using namespace esdht;
    void main3();
    
    void test_esdtcp(){
        
        
        
//        threadPool::Thread thread;
//        thread.startThread([]{
//            esdht::ESDTcp tcp;
//            tcp.receive("10.12.112.55", 6882, [&tcp](std::string msg, uv_stream_t *stream){
//                printf("I am Server:%s\n", msg.c_str());
//                tcp.response("ni hao woshi wang da chui", stream, nullptr);
//            });
//        });
        
        try {
            esdht::ESDTcp sender;
            
            sender.connect("83.170.95.163", 80, [&sender](uv_stream_t *stream){
                for(int i = 0 ; i < 1 ; i++){
                    sender.send(stream, "GET /robots.txt HTTP/1.0\n\n", [&sender, &stream](std::string msg){
                    printf("read: %s\n", msg.c_str());
                });
                }
                sender.runDefult();
                sender.stopReceiveResponse(stream);
                
            });
        } catch (ESDTcpError error) {
            printf("%s\n", error.what());
        }
        
        

//         main3();
    }
    
    
    
}