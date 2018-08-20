#include "Converter.h"
//
//ExtractAddress::ExtractAddress(std::string address) {
//    folderPath = new std::filesystem::path(address);
//    DirectoryIterator = new std::filesystem::directory_iterator(*folderPath);
//}


void ConverterOperation::storeCSVData(std::string address,std::map<FrameId, OrignalData> &storeFiles){
    std::fstream file(address);
    OrignalData fileData;
    std::string value;
    std::vector<std::string> temporaryStore;
    int firstLine = 1;
    while(file.good()){
        if(firstLine){
            std::getline(file, value);
            firstLine--;
        }else{
            int counter = 1;
            while(std::getline(file, value)){
                std::stringstream ss(value);
                std::string subvalue;
                while (std::getline(ss, subvalue, ',')) {
                    counter++;
                    if(counter == 18){
                        std::cout<<"wait"<<std::endl;
                    }
                    temporaryStore.push_back(value);
                    std::cout<<subvalue<<std::endl;
                }
                
            }

            }
//            if(fileData.ID == std::stoi(temporaryStore[0])){
//
//            }
            
        }
    }




