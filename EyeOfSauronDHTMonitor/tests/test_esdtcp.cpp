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
#include "ESDHttpProtocol.hpp"


namespace test_esdtcp {
    using namespace esdht;
    void main3();
    
    void test_esdtcp(){
        
        threadPool::Thread thread;
        thread.startThread([]{
            esdht::ESDTcp tcp;
            tcp.receive("10.12.112.55", 8809, [&tcp](std::string msg, uv_stream_t *stream){
                ESDHttp http;
//                printf("I am Server:%s\n", msg.c_str());
                http.handleRequest(msg);
                fprintf(stderr, "%s", http.requestBody.c_str());
                
                tcp.response("ni hao woshi wang da chui", stream, nullptr);
            });
        });
        
//        try {
//            ESDDns dns;
//            dns.getIpOfURL("www.w3.org", 80, [](std::string address, unsigned int port){
//                esdht::ESDTcp sender;
//                sender.connect(address, port, [&sender](uv_stream_t *stream){
//                    std::string httpContent;
//                    sender.send(stream, "GET /html/ HTTP/1.1\r\nHost: www.w3.org\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n", [&sender, &stream, &httpContent](std::string msg){
//                        
//                        httpContent += msg;
//                        
//                    });
//                    sender.runDefult();
//                    sender.stopReceiveResponse(stream);
//                    ESDHttp response;
//                    response.handleResponse(httpContent);
//                    fprintf(stderr, "%s", response.responseBody.c_str());
//                });
//            });
//            
//        } catch (ESDTcpError error) {
//            printf("%s\n", error.what());
//        } catch (ESDDnsError error){
//            printf("%s\n", error.what());
//        }
        
    }
    
    
    
}