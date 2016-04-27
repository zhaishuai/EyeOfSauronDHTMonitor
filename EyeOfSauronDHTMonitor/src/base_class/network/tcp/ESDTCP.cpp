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
    /**
     * 客户端程序建立连接
     */
    void ESDTcp::connect(std::string ipv4, int port, std::function<void (uv_stream_t* stream)> concb){
        connectCallback = concb;
        uv_ip4_addr(ipv4.c_str(), port, &sendAddr);
        uv_connect_t connect;
        connect.data = this;
        uv_tcp_keepalive(&clientSocket,1,2);
        uv_tcp_connect(&connect, &clientSocket, (const struct sockaddr *)&sendAddr, on_connect);
        uv_run(&clinetLoop, UV_RUN_DEFAULT);
    }
    /**
     * 客户端程序发送数据
     */
    void ESDTcp::send(uv_stream_t *stream, std::string msg, std::function<void (std::string msg)> sendRevcb){
        this->sendReceiveCallback = sendRevcb;
        uv_write_t req;
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());
        int r = uv_write(&req, stream, &buffer, 1, on_write);
        if(r){
            throw ESDTcpError(uv_strerror(r));
        }
        uv_run(&clinetLoop, UV_RUN_NOWAIT);

    }
    /**
     *  客户端发送请求后接受服务端的响应
     */
    void ESDTcp::stopReceiveResponse(uv_stream_t *stream){
        uv_read_stop(stream);
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
//        uv_shutdown_t req;
//        uv_shutdown(&req, uv_stream_t* handle, uv_shutdown_cb cb)¶
    }
    
    void ESDTcp::response(std::string msg, uv_stream_t* stream, std::function<void (int state)> callback){
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());
        uv_write_t request;
        request.data = stream;
        int r = uv_write(&request, stream, &buffer, 1, on_write);
        if(r){
            throw ESDTcpError(uv_strerror(r));
        }
        uv_run(&serverLoop, UV_RUN_DEFAULT);
        
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
        free(buf->base);
        tcp->receiveCallback(msg, stream);
    }
    
    void uv_close_cb(uv_handle_t* handle){
        free(handle);
    }
    
    // client
    //
    /**
     * 进行三次握手协议，建立连接。
     */
    void on_connect(uv_connect_t* connection, int status)
    {
        if(status == -1){
            uv_close((uv_handle_t *)connection->handle, nullptr);
            throw ESDTcpError(uv_strerror(status));
        }
        
        ESDTcp *tcp = (ESDTcp *)connection->data;
        
        uv_stream_t* stream = connection->handle;
        stream->data = tcp;
        uv_stream_set_blocking(stream, 0);
        uv_read_start(stream, alloc_buffer, on_client_read);
        if(tcp->connectCallback){
            tcp->connectCallback(stream);
        }

    }
    
    /**
     * send之后接收到的server的响应
     */
    void on_client_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
        if(nread >= 0) {
            ESDTcp *tcp = (ESDTcp *)stream->data;
            if(tcp->sendReceiveCallback){
                tcp->sendReceiveCallback(buf->base);
            }
        }
        else {
            uv_read_stop(stream);
            uv_close((uv_handle_t*)stream, nullptr);
        }
        free(buf->base);
    }
    
    void on_write(uv_write_t* req, int status){
        if (status) {
            throw ESDTcpError(uv_strerror(status));
        }
        uv_stream_t* stream = (uv_stream_t *)req->data;
        
//        uv_stop(&tcp->serverLoop);
        uv_close((uv_handle_t *)stream, uv_close_cb);
//        uv_run(&serverLoop, UV_RUN_DEFAULT);
        
    }
    
}