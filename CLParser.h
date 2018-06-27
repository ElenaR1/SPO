//
//  CLParser.h
//  V3SlowPiCalc
//
//  Created by Мирослав Николов on 12.06.18 г..
//  Copyright © 2018 г. Мирослав Николов. All rights reserved.
//

#ifndef CLParser_h
#define CLParser_h

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdlib.h>
//using namespace std;

class CLParser
{
public:
    
    CLParser(int argc_, const char * argv_[],bool switches_on_=false);
    ~CLParser(){}
    
    std::string get_arg(int i);
    std::string get_arg(std::string s);
    int get_arg_int(std::string s);
    
private:
    
    int argc;
    std::vector<std::string> argv;
    
    bool switches_on;
    std::map<std::string,std::string> switch_map;
};

CLParser::CLParser(int argc_, const char * argv_[],bool switches_on_)
{
    argc=argc_;
    argv.resize(argc);
    copy(argv_,argv_+argc,argv.begin());
    switches_on=switches_on_;
    
    //map the switches to the actual
    //arguments if necessary
    if (switches_on)
    {
        std::vector<std::string>::iterator it1,it2;
        it1=argv.begin();
        it2=it1+1;
        
        while (true)
        {
            if (it1==argv.end()) break;
            if (it2==argv.end()) {
                switch_map[*it1] = "";
                break;
            }
            
            if ((*it1)[0]=='-')
                switch_map[*it1]=*(it2);
            
            it1++;
            it2++;
        }
    }
}

std::string CLParser::get_arg(int i)
{
    if (i>=0&&i<argc)
        return argv[i];
    
    return "";
}

std::string CLParser::get_arg(std::string s)
{
    if (!switches_on) return "";
    
    if (switch_map.find(s)!=switch_map.end())
        return switch_map[s];
    
    return "";
}

int CLParser::get_arg_int(std::string s)
{
    if (!switches_on) return 0;
    
    if (switch_map.find(s)!=switch_map.end()) {
        if(switch_map[s].size() == 0) return 0;
        return stoi(switch_map[s]);
    }
    
    
    return -1;
}

#endif /* CLParser_h */
