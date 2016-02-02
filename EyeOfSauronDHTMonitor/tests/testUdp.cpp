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
    
    void run_send_and_receive(){
        ESDUdp udp;
        for(int i = 0; i < 5000; i++){
            try {
                
                udp.send("127.0.0.1", 6882, "hola hello nihao", [i](int status){
                    if(status == -1){
                        fprintf(stderr, "Send error!\n");
                    }
                    printf("send sucess %d\n", i);
                    
                }, [i](std::string msg){
                    printf("receive msg:%s i:%d\n", msg.c_str(), i);
                });
                
                
            
            } catch (const ESDUdpError &error) {
                printf("\n%s\n",error.what());
            }
            
        }//for
        
        
//        for(int i = 0; i < 50000; i++){
//            try {
//                ESDUdp udp;
//                udp.send("127.0.0.1", 6882, "hola hello nihao", [i](int status){
//                    if(status == -1){
//                        fprintf(stderr, "Send error!\n");
//                    }
//                    printf("send sucess %d\n", i);
//                    
//                }, nullptr);
//
//            } catch (const ESDUdpError &error) {
//                printf("\n%s\n",error.what());
//            }
//            
//        }//for
        
    }//
    
    
    void run_receive_and_response(){
        
        for(int i = 0; i < 10000; i++){
            try {
                uv_loop_t *loop = uv_loop_new();
                ESDUdp udp(loop);
                string str;
                int i = 0;
                udp.receive("0.0.0.0", 6882, [&udp,&str,&i](std::string msg){
                    printf("%s\n",msg.c_str());
                    str = msg;
                    udp.response(str, [&i,&udp](int status){
                        printf("%d\n",i++);
                        udp.stopReceive();
                    });
                });
                free(loop);
            } catch (const ESDUdpError &error) {
                printf("\n%s\n",error.what());
            }
            
        }//for
        
        uv_loop_t *loop = uv_loop_new();
        ESDUdp udp(loop);
        for(int i = 0 ; i < 10000; i++){
            try {
                string str;
                int i = 0;
                udp.receive("0.0.0.0", 6882, [&udp,&str,&i](std::string msg){
                    printf("%s\n",msg.c_str());
                    str = msg;
//                    if(i == 1){
//                        udp.stopReceive();
//                    }
                    udp.response(str, [&i,&udp](int status){
                     if(i == 1){
                          udp.stopReceive();
                     }
                        printf("%d\n",i++);
                    });
                });
                
            } catch (const ESDUdpError &error) {
                printf("\n%s\n",error.what());
            }
        }
        free(loop);
    }
    
    
    void run_test_udp(){
        
        run_send_and_receive();
        
//        run_receive_and_response();
        
    }
    
}