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
    void uvCloseCallback(uv_handle_t* handle);
    void receive_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf,
                          const struct sockaddr *addr, unsigned flags);
    void response_callback(uv_udp_send_t *req, int status);
    
    ESDUdp::ESDUdp(){
        
//        loop = uv_loop_new();
        loop = uv_default_loop();
        
        uv_udp_init(loop, &sendSocket);
        sendRequest.data = this;
        responseRequest.data = this;
        sendSocket.data = this;
        receiveSocket.data = this;
        
        loop->data = this;
        
    }//ESDUdp
    
    ESDUdp::ESDUdp(uv_loop_t *loop){
        this->loop = loop;
        uv_udp_init(loop, &sendSocket);
        sendRequest.data = this;
        responseRequest.data = this;
        sendSocket.data = this;
        receiveSocket.data = this;
        
        loop->data = this;
        
    }//ESDUdp
    
    ESDUdp::~ESDUdp(){
//        uv_close((uv_handle_t*)&socket, uvCloseCallback);
//        uv_loop_close(loop);
        
    }//~EDUdp
    
    void uvCloseCallback(uv_handle_t* handle){
        
    }
    


    

    
    

    

    

    
    
    void ESDUdp::send(std::string ipv4, int port, std::string msg, std::function<void(int status)> sendcb, std::function<void(std::string)> revcb, double timeout, int flag){
        
        this->sendCallback = sendcb;
        this->receiveResponseCallback = revcb;
        uv_udp_init(loop, &sendSocket);
        uv_ip4_addr(ipv4.c_str(), port, &sendAddr);
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());
        int error = uv_udp_send(&sendRequest, &sendSocket, &buffer, 1, (const struct sockaddr *)&sendAddr, send_callback);
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        
        if(revcb == nullptr){
            uv_run(loop, UV_RUN_DEFAULT);
            return;
        }
        

        error = uv_udp_recv_start(&sendSocket, alloc_buffer_callback, receiveResponse_callback);
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        uv_run(loop, UV_RUN_DEFAULT);
        
        
    }//send
    
    void send_callback(uv_udp_send_t *req, int status){
        
        ESDUdp *udp = (ESDUdp *)req->data;
        if(udp != NULL && udp->sendCallback != nullptr){
            udp->sendCallback(status);
            udp->sendCallback = nullptr;
        }
        if(udp->receiveResponseCallback == nullptr){
            uv_close((uv_handle_t*)req->handle, NULL);
        }
        
    }//send_callback
    
    void receiveResponse_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf,
                                  const struct sockaddr *addr, unsigned flags){
        ESDUdp *udp = (ESDUdp *)req->data;
        
        if(nread == 0 || nread == -1){
            free(buf->base);
            if(udp != NULL && udp->receiveResponseCallback != nullptr){
                udp->receiveResponseCallback("");
                udp->receiveResponseCallback = nullptr;
            }
            return;
        }
        
        if(udp != NULL && udp->receiveResponseCallback != nullptr){
            udp->receiveResponseCallback(std::string{buf->base, static_cast<size_t>(nread)});
            udp->receiveResponseCallback = nullptr;
        }
        free(buf->base);
        uv_udp_recv_stop(req);
        uv_close((uv_handle_t*) req, nullptr);
        
    }//receiveResponse_callback
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //
    
    void ESDUdp::receive(std::string ipv4, int port, std::function<void(std::string)> revcb, double timeout, int flag){
        
        this->receiveCallback = revcb;
        uv_udp_init(loop, &receiveSocket);
        uv_ip4_addr(ipv4.c_str(), port, &recvAddr);
        uv_udp_bind(&receiveSocket, (const struct sockaddr*) &recvAddr, 0);
        int error = uv_udp_recv_start(&receiveSocket, alloc_buffer_callback, receive_callback);
        
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
        uv_run(loop, UV_RUN_ONCE);
        
    }
    
    
    void receive_callback(uv_udp_t *req, ssize_t nread, const uv_buf_t* buf,
                          const struct sockaddr *addr, unsigned flags){
        ESDUdp *udp = (ESDUdp *)req->data;
        
        if(nread == 0 || nread == -1){
            free(buf->base);
            if(udp != NULL && udp->receiveCallback != nullptr){
                udp->receiveCallback("");
            }
            return;
        }
        
        if(udp != NULL && udp->receiveCallback != nullptr){
            udp->responseAddr = (struct sockaddr *)addr;
            udp->receiveCallback(std::string{buf->base, static_cast<size_t>(nread)});
        }
        
        uv_buf_t buffer = uv_buf_init(buf->base, (unsigned int)nread);
        int error = uv_udp_send(&udp->responseRequest, &udp->receiveSocket, &buffer, 1, (const struct sockaddr *)addr, response_callback);
        uv_run(udp->loop, UV_RUN_ONCE);
        free(buf->base);

        
    }//receive_callback
    
    void ESDUdp::response(std::string msg, std::function<void(int status)> callback){
        
        this->responseCallback = callback;
        uv_buf_t buffer = uv_buf_init((char *)msg.c_str(), (unsigned int)msg.length());
        int error = uv_udp_send(&responseRequest, &receiveSocket, &buffer, 1, (const struct sockaddr *)responseAddr, response_callback);
        if(error < 0){
            throw ESDUdpError(uv_strerror(error));
        }
//        uv_run(loop, UV_RUN_ONCE);
        
    }//reponse
    
    void response_callback(uv_udp_send_t *req, int status){
        
        ESDUdp *udp = (ESDUdp *)req->data;
        if(udp != NULL && udp->responseCallback != nullptr){
            udp->responseCallback(status);
            udp->responseCallback = nullptr;
        }
//        if(udp->receiveResponseCallback == nullptr){
//            uv_close((uv_handle_t*)req->handle, NULL);
//        }
        
    }//response_callback
    

    

    
    
    
    void alloc_buffer_callback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf){
        
        (*buf) = uv_buf_init((char*) malloc(suggested_size), (unsigned int)suggested_size);
        
    }//alloc_buffer_callback
    
    
    
    
}