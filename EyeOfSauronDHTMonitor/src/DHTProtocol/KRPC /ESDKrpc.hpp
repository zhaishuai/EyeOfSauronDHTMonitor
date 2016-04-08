//
//  ESDKrpc.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/8/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDKrpc_hpp
#define ESDKrpc_hpp

#include <stdio.h>

namespace esdht {
    
    class ESDKrpc : public ESDObject{
    
    protected:
        
    public:
        std::string ping(std::string transactionID, std::string id);
    };
    
}

#endif /* ESDKrpc_hpp */
