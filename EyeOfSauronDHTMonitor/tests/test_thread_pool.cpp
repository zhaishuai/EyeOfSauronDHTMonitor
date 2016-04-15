//
//  test_thread_pool.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/15/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_thread_pool.hpp"
#include "ThreadPool.hpp"

namespace test_thread_pool {
    
    void test_thread_pool(){
        
        for (int i = 0 ; i < 10000000 ; i++){
            threadPool::WorkQueue work(3,3);
        }
    }
    
}