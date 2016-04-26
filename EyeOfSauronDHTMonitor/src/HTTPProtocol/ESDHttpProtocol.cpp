//
//  ESDHttpProtocol.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/25/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "ESDHttpProtocol.hpp"
#include "ESDHttpUtility.hpp"

namespace esdht {
    
    enum State{RESPONSELINE, RESPONSEHEADER, RESPONSEBODY};
    
    void ESDHttpResponse::handleResponse(std::string &msg){
        std::istringstream stream(msg);
        int index = RESPONSELINE;
        for(std::string line; std::getline(stream, line) ; ){
            if(index == RESPONSELINE){
                std::deque<std::string> substrings = split(line, ' ');
                if(substrings.size() == 3){
                    httpVersion  = substrings[0];
                    status       = std::stoi(substrings[1]);
                    reasonPhrase = substrings[2];
                }
                index = RESPONSEHEADER;
                
            }else if(line != "\r" && index == RESPONSEHEADER){
                
                auto pos = line.find_first_of(": ");
                if(pos >= line.length())
                    continue;
                string key   = line.substr(0, pos);
                string value = line.substr(pos+2, line.length() - pos);
                header.insert(std::pair<std::string, std::string>(key, value));
                
            }else if(line == "\r" && index == RESPONSEHEADER){
                index = RESPONSEBODY;
            }else if(index == RESPONSEBODY){
                responseBody += line + "\n";
            }
            
            
        }
        
    }
    
}