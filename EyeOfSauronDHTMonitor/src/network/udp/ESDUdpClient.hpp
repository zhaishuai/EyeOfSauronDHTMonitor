//
//  ESDUdpClient.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/23/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDUdpClient_hpp
#define ESDUdpClient_hpp

#include <stdio.h>

namespace esdht {
    /**
     * @brief 用来存储DHT网络节点ip和port的数据接口。
     */
    struct BootstrapNode{
        // 节点的ip地址
        //
        char*        ip;
        // 节点的端口地址
        //
        unsigned int port;
    };
    
    
    /**
     * @brief DHTClient的职责是负责发送DHT请求（UDP），接收DHT网络的回复（UPD）。
     */
    class ESDUdpClient : public ESDObject{
    private:
        /**
         * @brief 用来存放DHT网络节点
         */
        std::shared_ptr<std::vector<BootstrapNode>> nodes = std::shared_ptr<std::vector<BootstrapNode>>(new std::vector<BootstrapNode>);
    public:
        /**
         * @brief 给出DHT节点json文件的路径并并解析json
         */
        int  readBootstrapNodesFromFile(char *path);
        
        /**
         * @brief 加入DHT网络
         */
        void joinDHT();
        
        /**
         * @brief 处理DHT网络对于FindNode请求的回复
         * @param length代表buf的长度，buf代表回复的内容
         */
        void processFindNodeResponse(ssize_t length, char *buf);
    };
    
}

#endif /* ESDUdpClient_hpp */
