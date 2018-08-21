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
            int counter = 0;
            while(std::getline(file, value)){
                
                std::stringstream ss(value);
                std::string subvalue;
                
                while (std::getline(ss, subvalue, ',')) {
                    if(std::find(temporaryStore.begin(), temporaryStore.end(), subvalue)==temporaryStore.end())
                        temporaryStore.push_back(subvalue);
                    counter++;
                    
                    if(counter == 18){
                        counter = 0;
                        fileData.ID = std::stoi(temporaryStore[0]);
                        for (int i = 1;i<temporaryStore.size();i++) {
                            if (temporaryStore[i] == "STAT_StaticMeshTriangles") {
                                fileData.STAT_StaticMeshTriangles = std::stof(temporaryStore[++i]);
                            }else if (temporaryStore[i] == "STAT_MeshDrawCalls"){
                                fileData.STAT_MeshDrawCalls = std::stof(temporaryStore[++i]);
                            }else if (temporaryStore[i] == "Stat_GPU_Total"){
                                fileData.Stat_GPU_Total =std::stof(temporaryStore[++i]);
                            }else if (temporaryStore[i] == "GameThread"){
                                fileData.GameThread = std::stof(temporaryStore[++i]);
                            }else if (temporaryStore[i] == "STAT_FrameTime"){
                                fileData.STAT_FrameTime = std::stoi(temporaryStore[++i]);
                            }else if(temporaryStore[i] == "STAT_TotalSceneRenderingTime"){
                                fileData.STAT_TotalSceneRenderingTime = std::stof(temporaryStore[++i]);
                            }
                        }
                        storeFiles[fileData.ID] = fileData;
                        temporaryStore.clear();
                    }
                }
                
            }

            }
        }
    }


void ConverterOperation::calculateSummaryData(std::map<FrameId, OrignalData> &storeFiles,std::vector<SummaryData>&storeSummaryData){
    
    
    
}




