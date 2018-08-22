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
int main(int argc, char *argv[]){
    std::map<FrameId, OrignalData> test;
    //std::string address = "test.csv";
    //
    //std::fstream file(address);
    //if(file.is_open()){
    //    std::cout<<"the file is open!"<<std::endl;
    //}else{
    //    std::cout<<"the file is not open!"<<std::endl;
    //}
    ConverterOperation cv;
    SummaryData sd;
    //cv.storeCSVData(address, test);
    //cv.calculateSummaryData(test, sd);
    

	//FileManager *FM = new FileManager("D:/CSV/","D:/Result/");

	//for (auto it : *(FM->getDirectoryIterator()))
	//{
	//	SummaryData sd;
	//	std::cout<<it<<std::endl;
	//	auto p = it.path();
	//	cv.storeCSVData(p.string(), test);
	//	cv.calculateSummaryData(test, sd, p.filename().string().substr(0, p.filename().string().size()-4));
	//	FM->setStoreSummaryData(sd);
	//	FM->recordAllSummaryData();
	//}
	std::cout << argc << std::endl;
	for (int i = 0; i < argc;i++) {
		std::cout << argv[i] << std::endl;
	}
	Converter* con = new Converter(argv[1], argv[2]);
	con->storeData();
	con->writeSummary();

	

    return 0;
}
