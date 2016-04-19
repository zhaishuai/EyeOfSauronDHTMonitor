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
    
    int main1();
    
    void test_esdtcp(){
        
        
        
        threadPool::Thread thread;
//        thread.startThread([]{
            esdht::ESDTcp tcp;
            tcp.receive("10.12.112.55", 6882, [](std::string msg){
                printf("%s\n", msg.c_str());
            });
//        });
//        esdht::ESDTcp sender;
//        sender.send("10.12.112.55", 6882, "ss", nullptr, nullptr);
//         main1();
    }
    
    
    
    
    
    
    

    
    /**
     * Our tcp server object.
     */
    uv_tcp_t server;
    
    /**
     * Shared reference to our event loop.
     */
    uv_loop_t * loop;
    
    /**
     * Function declarations.
     */
    void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
    void connection_cb(uv_stream_t * server, int status);
    void read_cb(uv_stream_t* stream,
                 ssize_t nread,
                 const uv_buf_t* buf);
    
    int main1() {
        loop = uv_default_loop();
        
        /* convert a humanreadable ip address to a c struct */
        struct sockaddr_in addr;
        uv_ip4_addr("10.12.112.55", 6882, &addr);
        
        /* initialize the server */
        uv_tcp_init(loop, &server);
        /* bind the server to the address above */
        uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
        
        /* let the server listen on the address for new connections */
        int r = uv_listen((uv_stream_t *) &server, 128, connection_cb);
        
        if (r) {
            return fprintf(stderr, "Error on listening: %s.\n",
                           uv_strerror(r));
        }
        
        /* execute all tasks in queue */
        return uv_run(loop, UV_RUN_DEFAULT);
    }
    
    /**
     * Callback which is executed on each new connection.
     */
    void connection_cb(uv_stream_t * server, int status) {
        /* dynamically allocate a new client stream object on conn */
        uv_tcp_t * client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
        
        /* if status not zero there was an error */
        if (status == -1) {
            fprintf(stderr, "Error on listening: %s.\n",
                    uv_strerror(status));
        }
        
        /* initialize the new client */
        uv_tcp_init(loop, client);
        
        /* now let bind the client to the server to be used for incomings */
        if (uv_accept(server, (uv_stream_t *) client) == 0) {
            /* start reading from stream */
            int r = uv_read_start((uv_stream_t *) client, alloc_buffer, read_cb);
            
            if (r) {
                fprintf(stderr, "Error on reading client stream: %s.\n",
                        uv_strerror(r));
            }
        } else {
            /* close client stream on error */
            uv_close((uv_handle_t *) client, NULL);
        }
    }
    
    void uv_close_cb(uv_handle_t* handle){
        free(handle);
    }
    
    /**
     * Callback which is executed on each readable state.
     */
    void read_cb(uv_stream_t* stream,
                 ssize_t nread,
                 const uv_buf_t* buf) {
        if (nread == -1 || nread <= 0) {
            free(buf->base);
            uv_close((uv_handle_t *) stream, NULL);
            return;
        }
        

        fprintf(stderr, "server:%s", buf->base);
        /* free the remaining memory */
        free(buf->base);
        uv_close((uv_handle_t *) stream, uv_close_cb);
        
    }
    
    /**
     * Allocates a buffer which we can use for reading.
     */
    void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
        (*buf) = uv_buf_init((char*) malloc(suggested_size), (unsigned int)suggested_size);
    }
    
    
    
    
    
}