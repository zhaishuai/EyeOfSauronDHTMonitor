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
    
}//namespace esobject