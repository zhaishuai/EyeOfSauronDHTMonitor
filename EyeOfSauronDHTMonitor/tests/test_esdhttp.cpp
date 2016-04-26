//
//  test_esdhttp.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/25/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esdhttp.hpp"
#include <regex.h>


namespace test_esdhttp {
    
    void test_esdhttp(){
    
        char * source = (char *)"http://code.tutsplus.com:8080/tutorials/8-regular-expressions-you-should-know--net-6149";
        esdht::ESDHttpUtility u;
        const char* regexString = esdht::PORT.c_str();
        
        size_t maxMatches = strlen(source);
        size_t maxGroups = 1;
        
        regex_t regexCompiled;
        regmatch_t groupArray[maxGroups];
        unsigned int m;
        char * cursor;
        
        if(int error = regcomp(&regexCompiled, regexString, REG_EXTENDED))
        {
            char ebuf[256];
            printf("Could not compile regular expression.\n");
            regerror(error, &regexCompiled, ebuf, sizeof(ebuf));
            fprintf(stderr, "%s: regcom()\n", ebuf);
            return ;
        };
        
        m = 0;
        cursor = source;
        for (m = 0; m < maxMatches; m ++)
        {
            if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
                break;  // No more matches
            
            unsigned int g = 0;
            unsigned int offset = 0;
            for (g = 0; g < maxGroups; g++)
            {
                if (groupArray[g].rm_so == (size_t)-1)
                    break;  // No more groups
                
                if (g == 0)
                    offset = groupArray[g].rm_eo;
                
                char cursorCopy[strlen(cursor) + 1];
                strcpy(cursorCopy, cursor);
                cursorCopy[groupArray[g].rm_eo] = 0;
                printf("Match %u, Group %u: [%2u-%2u]: %s\n",
                       m, g, groupArray[g].rm_so, groupArray[g].rm_eo,
                       cursorCopy + groupArray[g].rm_so);
            }
            cursor += offset;
        }
        
        regfree(&regexCompiled);
        
    }
    
    
    
}