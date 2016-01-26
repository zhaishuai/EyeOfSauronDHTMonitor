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
    void receiveResponse_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf,
                          const struct sockaddr *addr, unsigned flags);
    
    ESDUdp::ESDUdp(){
        
//        loop = uv_loop_new();
        loop = uv_default_loop();
        uv_loop_init(loop);
        uv_udp_init(loop, &socket);
        sendRequest.data = this;
        socket.data = this;
        loop->data = this;
        
    }//ESDUdp
    
    ESDUdp::~ESDUdp(){
        
//        uv_loop_close(loop);
        
    }//~EDUdp
    
    void ESDUdp::bindReciveAddress(std::string ipv4, int port, uv_udp_flags flag){
        
        
    }//bindReciveAddress
    
    void ESDUdp::receive(std::function<void(std::string)> callback){
        
        
        
    };//receive
    
    void ESDUdp::receive(double timeOut, std::function<void(std::string)> callback){
    
    }//receive with timeout
    
    void ESDUdp::receiveResponse(std::function<void(std::string)> callback){
        
        this->receiveResponseCallback = callback;
        int error = uv_udp_recv_start(&socket, alloc_buffer_callback, receiveResponse_callback);
        if(error < 0){
            throw ESDUdpError("call uv_udp_recv_start failure\n");
        }
        uv_run(loop, UV_RUN_DEFAULT);
        
    };//receiveResponse
    
    void ESDUdp::receiveResponse(double timeOut, std::function<void(std::string)> callback){
        
    }//receiveResponse with timeout
    
    void ESDUdp::bindSendAddress(std::string ipv4, int port, uv_udp_flags flag){
        
        uv_ip4_addr(ipv4.c_str(), port, &sendAddr);
        uv_udp_bind(&socket, (const struct sockaddr *)&sendAddr, flag);
        
    }//bindSendAddress
    
    void ESDUdp::send(std::string msg, std::function<void (int status)> callback){
        
        this->sendCallback = callback;
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());
        int error = uv_udp_send(&sendRequest, &socket, &buffer, 1, (const struct sockaddr *)&sendAddr, send_callback);
        if(error < 0){
            throw ESDUdpError("message can’t be sent immediately\n");
        }
        uv_run(loop, UV_RUN_DEFAULT);
        
    }//send
    
    void ESDUdp::response(std::function<void (int)> callback){
    
    }//reponse
    
    
    
    void send_callback(uv_udp_send_t *req, int status){
        
        ESDUdp *udp = (ESDUdp *)req->data;
        if(udp != NULL && udp->sendCallback != nullptr){
            udp->sendCallback(status);
        }
        
    }//send_callback
    
    void receiveResponse_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf,
                          const struct sockaddr *addr, unsigned flags){
        ESDUdp *udp = (ESDUdp *)req->data;
        uv_udp_recv_stop(req);
        if(nread == 0 || nread == -1){
            free(buf->base);
            if(udp != NULL && udp->receiveResponseCallback != nullptr){
                udp->receiveResponseCallback("");
            }
            return;
        }
        
        if(udp != NULL && udp->receiveResponseCallback != nullptr){
            udp->receiveResponseCallback(std::string{buf->base, static_cast<size_t>(nread)});
        }
        free(buf->base);
    }
    
    void alloc_buffer_callback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf){
        
        (*buf) = uv_buf_init((char*) malloc(suggested_size), (unsigned int)suggested_size);
        
    }//alloc_buffer_callback
    
}