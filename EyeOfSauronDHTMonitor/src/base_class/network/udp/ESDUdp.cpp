//
//  ESDUdp.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/24/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDUdp.hpp"



namespace esdht {
    
    /**
     * @brief 构建UDP错误信息
     */
    ESDUdpError::ESDUdpError(const std::string &what):std::runtime_error(what){};
    
    void send_callback(uv_udp_send_t *req, int status);
    void alloc_buffer_callback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    void receiveResponse_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf, const struct sockaddr *addr, unsigned flags);
    void receiveResponse_callback_async(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf, const struct sockaddr *addr, unsigned flags);
    void uvCloseCallback(uv_handle_t* handle);
    void receive_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf, const struct sockaddr *addr, unsigned flags);
    void response_callback(uv_udp_send_t *req, int status);
    void receive_respond_timeout_cb(uv_timer_t* h);
    
    ESDUdp::ESDUdp(){
        
        loop = uv_default_loop();
        sendRequest.data = this;
        responseRequest.data = this;
        sendSocket.data = this;
        receiveSocket.data = this;
        
        uv_loop_init(&defaultLoop);
        receiveLoop = &defaultLoop;
        receiveLoop->data = this;
        loop->data = this;
        timer.data = this;
        uv_timer_init( loop, &timer );
        uv_udp_init(loop, &sendSocket);
        
        uv_udp_init(loop, &socket);
        
    }//ESDUdp
    
    ESDUdp::ESDUdp(uv_loop_t *loop){
        this->loop = loop;
        sendRequest.data = this;
        responseRequest.data = this;
        sendSocket.data = this;
        receiveSocket.data = this;
        
        uv_loop_init(&defaultLoop);
        receiveLoop = &defaultLoop;
        receiveLoop->data = this;
        loop->data = this;
        timer.data = this;
        uv_timer_init( loop, &timer );
        uv_udp_init(loop, &sendSocket);
        
        uv_udp_init(loop, &socket);
        
    }//ESDUdp
    
    ESDUdp::~ESDUdp(){
        
        stopLoop(loop);
        stopLoop(receiveLoop);

    }//~EDUdp
    
#pragma mark - 以下是客户端代码
    
    void ESDUdp::setSendPort(int port){
        struct sockaddr_in temp;
        uv_ip4_addr("", port, &temp);
        uv_udp_bind(&sendSocket, (const struct sockaddr*)&temp, UV_UDP_REUSEADDR);
        
    }//setSendPort
    
    void ESDUdp::sendAsync(std::string ipv4, int port, std::string msg, std::function<void (int)> sendcb, int flag){
        this->sendCallback = sendcb;
        uv_ip4_addr(ipv4.c_str(), port, &sendAddr);
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());
        int error = uv_udp_send(&sendRequest, &sendSocket, &buffer, 1, (const struct sockaddr *)&sendAddr, send_callback);
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        uv_run(loop, UV_RUN_NOWAIT);
    }
    
    void ESDUdp::licensingResponse(std::function<void (std::string response)> func){
        receiveResponseCallback = func;
        int error = uv_udp_recv_start(&sendSocket, alloc_buffer_callback, receiveResponse_callback_async);
        
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        
        uv_run(loop, UV_RUN_NOWAIT);
    }
    
    void ESDUdp::send(std::string ipv4, int port, std::string msg, std::function<void(int status)> sendcb, std::function<void(std::string)> revcb, double timeout, int flag){
        
        this->sendCallback = sendcb;
        this->receiveResponseCallback = revcb;
        uv_ip4_addr(ipv4.c_str(), port, &sendAddr);
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());

        int error = uv_udp_send(&sendRequest, &sendSocket, &buffer, 1, (const struct sockaddr *)&sendAddr, send_callback);
        
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        
        if(revcb == nullptr ){
            uv_run(loop, UV_RUN_DEFAULT);
            return;
        }
        
        error = uv_udp_recv_start(&sendSocket, alloc_buffer_callback, receiveResponse_callback);
        
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        uv_timer_start( &timer, receive_respond_timeout_cb, timeout, 0 );
        uv_run(loop, UV_RUN_DEFAULT);
        
    }//send
    
    void send_callback(uv_udp_send_t *req, int status){
        
        ESDUdp *udp = (ESDUdp *)req->data;
        if(udp != NULL && udp->sendCallback != nullptr){
            udp->sendCallback(status);
            udp->sendCallback = nullptr;
        }
        
    }//send_callback
    
    void receiveResponse_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf,
                                  const struct sockaddr *addr, unsigned flags){
        ESDUdp *udp = (ESDUdp *)req->data;
        uv_timer_stop(&udp->timer);
        if(nread == 0 || nread == -1){
            free(buf->base);
            return;
        }
        
        if(udp != NULL && udp->receiveResponseCallback != nullptr){
            udp->receiveResponseCallback(std::string{buf->base, static_cast<size_t>(nread)});
            udp->receiveResponseCallback = nullptr;
        }
        free(buf->base);
        uv_udp_recv_stop(req);
        if(!uv_is_closing((uv_handle_t*) req))
            uv_close((uv_handle_t*) req, nullptr);
        
    }//receiveResponse_callback
    
    void receiveResponse_callback_async(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf,
                                  const struct sockaddr *addr, unsigned flags){
        ESDUdp *udp = (ESDUdp *)req->data;
        uv_timer_stop(&udp->timer);
        if(nread == 0 || nread == -1){
            free(buf->base);
            return;
        }
        
        if(udp != NULL && udp->receiveResponseCallback != nullptr){
            udp->receiveResponseCallback(std::string{buf->base, static_cast<size_t>(nread)});
        }
        free(buf->base);
        
    }//receiveResponse_callback
    
    void receive_respond_timeout_cb(uv_timer_t* timer) {
        ESDUdp *udp = (ESDUdp *)timer->data;
        
        if(udp!=nullptr){
            uv_udp_recv_stop(&udp->sendSocket);

            if(!uv_is_closing((uv_handle_t*)&udp->sendSocket)){
                uv_close((uv_handle_t*)&udp->sendSocket, NULL);
            }
            uv_timer_stop(&udp->timer);
            throw ESDUdpError("request time out!\n");
        }
        
    }//receive_respond_timeout_cb
    
#pragma mark - 以下是服务端代码
    
    void ESDUdp::stopReceive(){
        uv_stop(receiveLoop);
        uv_udp_recv_stop(&receiveSocket);
        if(!uv_is_closing((uv_handle_t*)&receiveSocket)){
            uv_close((uv_handle_t*)&receiveSocket, NULL);
        }

    }//stopReceive
    
    void ESDUdp::receive(std::string ipv4, int port, std::function<void(std::string)> revcb, int flag){
        
        this->receiveCallback = revcb;
        uv_udp_init(receiveLoop, &receiveSocket);
        uv_ip4_addr(ipv4.c_str(), port, &recvAddr);
        uv_udp_bind(&receiveSocket, (const struct sockaddr*) &recvAddr, 0);
        int error = uv_udp_recv_start(&receiveSocket, alloc_buffer_callback, receive_callback);
        
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        uv_run(receiveLoop, UV_RUN_DEFAULT);

    }//receive
    
    
    void receive_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf,
                          const struct sockaddr *addr, unsigned flags){
        ESDUdp *udp = (ESDUdp *)req->data;
        
        if(nread == 0 || nread == -1){
            free(buf->base);
            return;
        }
        
        if(udp != NULL && udp->receiveCallback != nullptr){
            udp->responseAddr = (struct sockaddr *)addr;
            udp->receiveCallback(std::string{buf->base, static_cast<size_t>(nread)});
        }
        
        free(buf->base);

        
    }//receive_callback
    
    void ESDUdp::response(std::string msg, std::function<void(int status)> callback){
        
        this->responseCallback = callback;
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());
        int error = uv_udp_send(&responseRequest, &socket, &buffer, 1, (const struct sockaddr *)responseAddr, response_callback);
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        uv_run(loop, UV_RUN_NOWAIT);
        
        
    }//reponse
    
    void response_callback(uv_udp_send_t *req, int status){
        
        ESDUdp *udp = (ESDUdp *)req->data;
        if(udp != NULL && udp->responseCallback != nullptr){
            udp->responseCallback(status);
            udp->responseCallback = nullptr;
        }

        uv_close((uv_handle_t*)req->handle, NULL);
        
    }//response_callback
    
    void alloc_buffer_callback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf){
        
        (*buf) = uv_buf_init((char*) malloc(suggested_size), (unsigned int)suggested_size);
        
    }//alloc_buffer_callback
    
    
    
    
}