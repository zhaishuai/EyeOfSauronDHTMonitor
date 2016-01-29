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
        uv_loop_t *loop = uv_default_loop();
        uv_loop_init(loop);
        ESDUdp udp(loop);
//        for(int i = 0 ; i < 100000 ; i++){
            try {
                
//                udp.bindSendAddress("127.0.0.1", 6882);
//                udp.send("hello word ni hao shi jie\n", [i](int status){
//                    
//                    if(status == -1){
//                        fprintf(stderr, "Send error!\n");
//                    }
//                    printf("sucess %d\n", i);
//                });
//                
//                udp.receiveResponse([i](std::string msg){
//                    printf("msg:%s i:%d", msg.c_str(), i);
//                });
                
//                udp.send("127.0.0.1", 6882, "hola hello nihao", [i](int status){
//                    if(status == -1){
//                        fprintf(stderr, "Send error!\n");
//                    }
//                    printf("sucess %d\n", i);
//
//                }, [i](std::string msg){
//                    printf("msg:%s i:%d", msg.c_str(), i);
//                });
                string str;
                int i = 0;
                udp.receive("0.0.0.0", 6882, [&udp,&str,&i](std::string msg){
                    printf("%s\n",msg.c_str());
                    str = msg;
                    udp.response(str, [&i](int status){
                        printf("%d\n",i++);
                    });
                });
                
                
            } catch (const ESDUdpError &error) {
                printf("\n%s\n",error.what());
//                break;
//                sleep (1);
                
            }

            
//        }//for
        
        
        printf("dddd");
    }
}