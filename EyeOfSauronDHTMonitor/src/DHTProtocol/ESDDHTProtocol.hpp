//
//  ESDDHTProtocol.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/4/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDDHTProtocol_hpp
#define ESDDHTProtocol_hpp

#include <stdio.h>
#include "bencoding.h"

namespace esdht {
    using namespace bencoding;
    class ESDDHTProtocol : public ESDObject{
    public:
        virtual void ping(std::string ip, int port) = 0;
        
    };
    
}

#endif /* ESDDHTProtocol_hpp */
