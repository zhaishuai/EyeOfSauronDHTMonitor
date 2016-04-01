//
//  ThreadPool.cpp
//  PhreadTest
//
//  Created by shuaizhai on 3/25/16.
//  Copyright © 2016 pcx. All rights reserved.
//

#include "ThreadPool.hpp"
#include <signal.h>

namespace threadPool{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    
    void *threadCallback(void *data){
        pthread_mutex_unlock(&mutex);
        ThreadData *threadData = ((ThreadData *)data);
        pthread_testcancel();
        threadData->callback();
        pthread_exit(0);
    }
    
    
    
    Thread::Thread(){
        threadInfo.mut  = PTHREAD_MUTEX_INITIALIZER;
        threadInfo.cond = PTHREAD_COND_INITIALIZER;
    }
    
    Thread::~Thread(){
        pthread_join(threadInfo.pthread, NULL);
    }
    
    void Thread::startThread( std::function<void ()> func){
        pthread_mutex_lock( &mutex);
        threadData          = std::unique_ptr<ThreadData>(new ThreadData{this, func});
        threadInfo.threadId = pthread_create(&threadInfo.pthread, NULL, threadCallback, (void *)threadData.get());
    }
    
    
#pragma mark - Timer
    
    Timer::Timer(int timeInterval){
        thread             = std::unique_ptr<Thread>(new Thread);
        this->delaytime    = 0;
        this->timeInterval = timeInterval;
        mut = PTHREAD_MUTEX_INITIALIZER;
        timeStarted = false;
    }
    
    Timer::Timer(int delay, int timeInterval){
        thread             = std::unique_ptr<Thread>(new Thread);
        this->delaytime    = delay;
        this->timeInterval = timeInterval;
        mut                = PTHREAD_MUTEX_INITIALIZER;
        quitMut            = PTHREAD_MUTEX_INITIALIZER;
        timeStarted = false;
        
    }
    
    void Timer::start(std::function<void ()> func){
        
        if(timeStarted){
            return;
        }
        pthread_mutex_lock(&mut);
        timeStarted = true;
        pthread_mutex_unlock(&mut);
        
        thread->startThread([func, this]{
            usleep(this->delaytime*1000);
            while (true) {
                if(func!=nullptr){
                    func();
                }
                usleep(this->timeInterval*1000);
                pthread_mutex_lock(&mut);
                if(!timeStarted){
                    pthread_mutex_unlock(&mut);
                    break;
                }
                pthread_mutex_unlock(&mut);
                
            }
            
        });
    }
    
    void Timer::stop(){
        if(!timeStarted)
            return;
        pthread_mutex_lock(&mut);
        timeStarted = false;
        pthread_mutex_unlock(&mut);
    }
    
    Timer::~Timer(){
        
        pthread_mutex_lock(&quitMut);
        stop();
        pthread_cancel(thread->threadInfo.pthread);
        pthread_mutex_unlock(&quitMut);
        
    }
    
#pragma mark - ThreadPool
    
    ThreadPool::ThreadPool(){
        idleQueue = std::unique_ptr<std::deque<std::shared_ptr<Thread>>>(new std::deque<std::shared_ptr<Thread>>);
        workQueue = std::unique_ptr<std::vector<std::shared_ptr<Thread>>>(new std::vector<std::shared_ptr<Thread>>);
        taskQueue = std::unique_ptr<std::deque<std::function<void()>>>(new std::deque<std::function<void ()>>);
        addThreadTimer    = std::unique_ptr<Timer>(new Timer(500,500));
        removeThreadTimer = std::unique_ptr<Timer>(new Timer(1000, 1000));
        
        threadQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        taskQueueMutex   = PTHREAD_MUTEX_INITIALIZER;
        for(int i = 0 ; i < minThreads ; i++){
            std::shared_ptr<Thread> reusedThread = std::shared_ptr<Thread>(new Thread());
            startThread(reusedThread);
        }
        
    }
    
    ThreadPool::ThreadPool(int increaseInterval, int decreaseInterval){
        idleQueue = std::unique_ptr<std::deque<std::shared_ptr<Thread>>>(new std::deque<std::shared_ptr<Thread>>);
        workQueue = std::unique_ptr<std::vector<std::shared_ptr<Thread>>>(new std::vector<std::shared_ptr<Thread>>);
        taskQueue = std::unique_ptr<std::deque<std::function<void()>>>(new std::deque<std::function<void ()>>);
        
        addThreadTimer    = std::unique_ptr<Timer>(new Timer(increaseInterval,increaseInterval));
        removeThreadTimer = std::unique_ptr<Timer>(new Timer(decreaseInterval, decreaseInterval));
        
        threadQueueMutex = PTHREAD_MUTEX_INITIALIZER;
        taskQueueMutex   = PTHREAD_MUTEX_INITIALIZER;
        for(int i = 0 ; i < minThreads ; i++){
            std::shared_ptr<Thread> reusedThread = std::shared_ptr<Thread>(new Thread());
            startThread(reusedThread);
        }
    }
    
    void ThreadPool::startThread(std::shared_ptr<Thread> reusedThread){
        
        reusedThread->startThread([reusedThread, this]{
            std::shared_ptr<Thread> thread = reusedThread;
            while(true){
                pthread_testcancel();
                pthread_mutex_lock(&taskQueueMutex);
                std::function<void()> func;
                if(taskQueue->size()){
                    func = taskQueue->front();
                    taskQueue->pop_front();
                }
                pthread_mutex_unlock(&taskQueueMutex);
                if(func!=nullptr)
                    func();
                
                pthread_mutex_lock(&taskQueueMutex);
                if(taskQueue->size()){
                    pthread_mutex_unlock(&taskQueueMutex);
                    continue;
                }
                pthread_mutex_unlock(&taskQueueMutex);
                
                pthread_mutex_lock(&threadQueueMutex);
                idleQueue->push_back(thread);
                workQueue->erase(std::remove(workQueue->begin(), workQueue->end(), thread), workQueue->end());
                // 判断线程池中的任务是否被全部完成
                //
                if(idleQueue->size() == currentThreads && taskQueue->size() == 0 ){
                    addThreadTimer->stop();
                    taskQueue->shrink_to_fit();
                    // 线程池中所有线程结束任务后开启移除大于minThreads部分线程。
                    //
                    removeThreadTimer->start([this]{
                        
                        if(currentThreads > minThreads){
                            pthread_mutex_lock(&threadQueueMutex);
                            idleQueue->pop_back();
                            currentThreads --;
                            pthread_mutex_unlock(&threadQueueMutex);
                        }else{
                            removeThreadTimer->stop();
                        }
                    });
                    
                    // 回调，线程池已完成全部任务
                    //
                    if(finishCallback != nullptr){
                        finishCallback();
                    }
                    
                }
                pthread_mutex_unlock(&threadQueueMutex);
                
                pthread_mutex_lock(&thread->threadInfo.mut);
                pthread_cond_wait(&thread->threadInfo.cond, &thread->threadInfo.mut);
                pthread_mutex_unlock(&thread->threadInfo.mut);
            }
        });
    }
    
    void ThreadPool::addThreadIntoPool(){
        std::shared_ptr<Thread> reusedThread = std::shared_ptr<Thread>(new Thread());
        pthread_mutex_lock(&threadQueueMutex);
        workQueue->push_back(reusedThread);
        currentThreads ++;
        pthread_mutex_unlock(&threadQueueMutex);
        startThread(reusedThread);
        
    }
    
    void ThreadPool::run(std::function<void()> func){
        removeThreadTimer->stop();
        pthread_mutex_lock(&threadQueueMutex);
        std::shared_ptr<Thread> thread = nullptr;
        if(idleQueue->size()){
            thread = idleQueue->front();
            idleQueue->pop_front();
            workQueue->push_back(thread);
        }
        pthread_mutex_unlock(&threadQueueMutex);
        
        
        pthread_mutex_lock(&taskQueueMutex);
        taskQueue->push_back(func);
        // 判断是否要添加新的线程
        //
        if(taskQueue->size()&&currentThreads < maxThreads){
            addThreadTimer->start([this]{
                addThreadIntoPool();
                if(!(taskQueue->size()&&currentThreads < maxThreads)){
                    addThreadTimer->stop();
                    
                }
            });
        }
        
        pthread_mutex_unlock(&taskQueueMutex);
        
        if(thread){
            pthread_mutex_lock(&thread->threadInfo.mut);
            pthread_cond_signal(&thread->threadInfo.cond);
            pthread_mutex_unlock(&thread->threadInfo.mut);
        }
        
    }
    
    void ThreadPool::allTaskFinished(std::function<void ()> func){
        finishCallback = func;
    }
    
    void ThreadPool::stop(){
        pthread_mutex_lock(&taskQueueMutex);
        taskQueue->clear();
        pthread_mutex_unlock(&taskQueueMutex);
        
        long size = 1;
        
        while (size != 0) {
            pthread_mutex_lock(&threadQueueMutex);
            size = workQueue->size();
            sched_yield();
            pthread_mutex_unlock(&threadQueueMutex);
        }
    }
    
    int ThreadPool::getCurrentThreads(){
        return currentThreads;
    }
    
    ThreadPool::~ThreadPool(){
        stop();
        pthread_mutex_lock(&threadQueueMutex);
        for(int i = 0 ; i < idleQueue->size() ; i ++){
            pthread_cancel(idleQueue->at(i)->threadInfo.pthread);
        }
        pthread_mutex_unlock(&threadQueueMutex);
    }
    
}