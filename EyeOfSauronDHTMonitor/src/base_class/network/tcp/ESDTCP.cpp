//
//  ESDTCP.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/19/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDTCP.hpp"

namespace esdht {
    const int backlog = 128;
    
    
    void receive_callback(uv_stream_t* server, int status);
    void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
    void on_server_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
    void uv_close_cb(uv_handle_t* handle);
    void on_client_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
    void on_connect(uv_connect_t* connection, int status);
    void on_write(uv_write_t* req, int status);
    
    ESDTcp::ESDTcp(){
        uv_loop_init(&clinetLoop);
        uv_loop_init(&serverLoop);
        uv_tcp_init(&serverLoop, &serverSocket);
        uv_tcp_init(&clinetLoop, &clientSocket);
        serverSocket.data = this;
        clientSocket.data = this;
        
    }
    
    ESDTcp::ESDTcp(uv_loop_t *loop){
        
    }
    
    ESDTcp::~ESDTcp(){
        stopLoop(&clinetLoop);
        stopLoop(&serverLoop);
    }
    
    void ESDTcp::setSendPort(int port){
    
    }
    
    void ESDTcp::send(std::string ipv4, int port, std::string msg, std::function<void(int status)> sendcb, std::function<void(std::string)> revcb, double timeout, int flag){
        uv_ip4_addr(ipv4.c_str(), port, &sendAddr);

        uv_connect_t connect;
        connect.data = this;
        // TCP是面向连接的协议，在此进行connect的原因是进行三次握手协议从而建立连接。
        //
        
        uv_tcp_connect(&connect, &clientSocket, (const struct sockaddr *)&sendAddr, on_connect);
        uv_run(&clinetLoop, UV_RUN_DEFAULT);
    }
    
    void ESDTcp::licensingResponse(std::function<void (std::string)> func){
    
    }
    
    void ESDTcp::sendAsync(std::string ipv4, int port, std::string msg, std::function<void (int)> sendcb, int flag){
    
    }
    
    void ESDTcp::receive(std::string ipv4, int port, std::function<void (std::string, uv_stream_t* stream)> revcb, int flag){
        this->receiveCallback = revcb;
        uv_ip4_addr(ipv4.c_str(), port, &recvAddr);
        uv_tcp_bind(&serverSocket,(const struct sockaddr*)&recvAddr, 0);
        if(int error = uv_listen((uv_stream_t*)&serverSocket, backlog, receive_callback)){
            throw ESDTcpError(uv_strerror(error));
        }
        uv_run(&serverLoop, UV_RUN_DEFAULT);
    }
    
    void ESDTcp::stopReceive(){
    
    }
    
    void ESDTcp::response(std::string msg, uv_stream_t* stream, std::function<void (int state)> callback){
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());
        uv_write_t request;
        int r = uv_write(&request, stream, &buffer, 1, nullptr);
        if(r){
            throw ESDTcpError(uv_strerror(r));
        }
        uv_run(&serverLoop, UV_RUN_NOWAIT);
        uv_close((uv_handle_t *)stream, uv_close_cb);
    }
    
    void receive_callback(uv_stream_t* server, int status){
        
        if (status < 0) {
            throw ESDTcpError(uv_strerror(status));
            return;
        }
        ESDTcp *tcp = (ESDTcp *)server->data;
        uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
        client->data = tcp;
        uv_tcp_init(&tcp->serverLoop, client);
        
        // accept
        int result = uv_accept(server, (uv_stream_t*) client);
        
        if (result == 0) {
            // success
            uv_read_start((uv_stream_t*) client, alloc_buffer, on_server_read);
        } else {
            // error
            uv_close((uv_handle_t*) client, uv_close_cb);
        }
        
    }//response_callback
    

    void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
        (*buf) = uv_buf_init((char*) malloc(suggested_size), (unsigned int)suggested_size);
    }
    
    void on_server_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
        if (nread <= 0) {
            return;
        }
        ESDTcp *tcp = (ESDTcp *)stream->data;
        std::string msg = std::string(buf->base);
        delete buf->base;
        tcp->receiveCallback(msg, stream);
    }
    
    void uv_close_cb(uv_handle_t* handle){
        free(handle);
    }
    
    // client
    //
    void on_connect(uv_connect_t* connection, int status)
    {
        printf("connected.\n");
        
        ESDTcp *tcp = (ESDTcp *)connection->data;
        
        uv_stream_t* stream = connection->handle;
        
        uv_buf_t buffer[] = {
            {.base = "hello", .len = 5},
            {.base = "world", .len = 5}
        };
        
            uv_write_t request;

            uv_write(&request, stream, buffer, 2, on_write);
            
        
//        uv_run(&tcp->clinetLoop, UV_RUN_DEFAULT);
        uv_read_start(stream, alloc_buffer, on_client_read);
        
        
    }
    
    void on_client_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
        if(nread >= 0) {
            //printf("read: %s\n", tcp->data);
            printf("read: %s\n", buf->base);
        }
        else {
            //we got an EOF
            uv_close((uv_handle_t*)stream, nullptr);
            
        }
        
        //cargo-culted
        free(buf->base);
    }
    
    void on_write(uv_write_t* req, int status)
    {
        if (status) {
//            uv_err_t err = uv_last_error(loop);
//            fprintf(stderr, "uv_write error: %s\n", uv_strerror(err));
            return;
        }
//        printf("wrote.\n");
//        uv_close((uv_handle_t*)req->handle, nullptr);
    }
    
}