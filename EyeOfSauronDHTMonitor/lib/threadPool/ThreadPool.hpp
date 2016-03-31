//
//  ThreadPool.hpp
//  PhreadTest
//
//  Created by shuaizhai on 3/25/16.
//  Copyright © 2016 pcx. All rights reserved.
//

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <err.h>
#include <vector>
#include <functional>
#include <queue>

namespace threadPool{
    
    struct ThreadInfo{
        pthread_mutex_t mut;
        pthread_cond_t  cond;
        pthread_t       pthread;
        int             threadId;
    };
    
    struct ThreadData{
        void *data;
        std::function<void ()> callback;
    };

    
    class Thread{
    protected:
        std::unique_ptr<ThreadData> threadData;
        pthread_mutex_t quitMutex;
    public:
        bool shouldQuit = false;
        ThreadInfo threadInfo;
        
        Thread();
        ~Thread();
        void startThread(std::function<void ()> func);
    
    };
    
    class Timer{
    protected:
        std::unique_ptr<Thread> thread;
        int timeInterval = 0;
        int delaytime = 0;
        bool timeStarted = false;
        pthread_mutex_t mut, quitMut;
    public:
        Timer(int timeInterval);
        
        // 都是以毫秒为单位
        //
        Timer(int delay, int timeInterval);
        ~Timer();
        void start(std::function<void ()> func);
        void stop();
    };
    
    class ThreadPool{
    public:
        int minThreads = 150;
        int maxThreads  = 300;
        
        
        ThreadPool();
        ThreadPool(int increaseInterval, int decreaseInterval);
        ~ThreadPool();
        
        void run(std::function<void()> func);
        
        void stop();
        
        void allTaskFinished(std::function<void ()> func);
        
        int getCurrentThreads();
        
    protected:
        
        int currentThreads = minThreads;
        bool finishInit = false;
        std::unique_ptr<Timer> addThreadTimer;
        std::unique_ptr<Timer> removeThreadTimer;
        
        void addThreadIntoPool();
        void startThread(std::shared_ptr<Thread> thread);
        
        pthread_mutex_t threadQueueMutex;
        std::unique_ptr<std::deque<std::shared_ptr<Thread>>> idleQueue;
        std::unique_ptr<std::vector<std::shared_ptr<Thread>>> workQueue;
        
        pthread_mutex_t taskQueueMutex;
        std::unique_ptr<std::deque<std::function<void()>>> taskQueue;
        std::function<void ()> finishCallback;
        
    };
    
    
}

#endif /* ThreadPool_hpp */
