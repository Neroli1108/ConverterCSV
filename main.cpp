//
//  main.cpp
//  Converter
//
//  Created by Yunding Li on 8/20/18.
//  Copyright © 2018 Yunding Li. All rights reserved.
//
//
//#include <iostream>
//
//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}
#include "Converter.h"
int main(){
    std::map<FrameId, OrignalData> test;
    std::string address = "test.csv";
    
    std::fstream file(address);
    if(file.is_open()){
        std::cout<<"the file is open!"<<std::endl;
    }else{
        std::cout<<"the file is not open!"<<std::endl;
    }
    ConverterOperation cv;
    SummaryData sd;
    cv.storeCSVData(address, test);
    cv.calculateSummaryData(test, sd);
    
    return 0;
}
