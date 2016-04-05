//
//  ESDObject.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/23/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDObject.hpp"

namespace esdht {
    
    void walk_callback(uv_handle_t* handle, void* arg);
    
    /**
     * @brief构造ESDObject
     */
    ESDObject::ESDObject() = default;
    /**
     * @brief析构ESDObject
     */
    ESDObject::~ESDObject() = default;
    
    int ESDObject::stopLoop(uv_loop_t *loop){
        uv_stop(loop);
        int error = uv_loop_close(loop);
        while (error) {
            uv_run(loop, UV_RUN_DEFAULT);
            uv_stop(loop);
            uv_walk(loop, walk_callback, NULL);
            uv_run(loop, UV_RUN_DEFAULT);
            error = uv_loop_close(loop);
        }
        return error;
    }
    
    void walk_callback(uv_handle_t* handle, void* arg){
        if(!uv_is_closing(handle))
            uv_close(handle, NULL);
    }
    
    void async_cb(uv_async_t* handle){
        ESDAsync *async = (ESDAsync *)(handle->data);
        if(async->func!=nullptr){
            async->func(async->data);
        }
    }
    
    void ESDObject::addAsync(ESDAsync *async, uv_loop_t *loop, std::function<void (void *data)> func){
        async->func = func;
        uv_async_init(loop, &async->async, async_cb);
    
    }
    
#pragma mark - ESDAsync
    
    void ESDAsync::sendAsync(void *data){
        this->data = data;
        async.data = this;
        uv_async_send(&async);
    }
    
    ESDAsync::~ESDAsync(){
        
    }
    
}//namespace esobject