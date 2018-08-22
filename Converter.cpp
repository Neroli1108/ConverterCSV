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


void ConverterOperation::calculateSummaryData(std::map<FrameId, OrignalData> &storeFiles,SummaryData& storeSummaryData){
    float num = storeFiles.size();
    float totalSTAT_StaticMeshTriangles = 0.00;
    float totalSTAT_MeshDrawCalls = 0.00;
    float totalStat_GPU_Total = 0.00;
    float totalGameThread = 0.00;
    float totalSTAT_FrameTime= 0.00;
    float totalSTAT_TotalSceneRenderingTime = 0.00;
    for (std::map<FrameId, OrignalData>::iterator it = storeFiles.begin(); it!=storeFiles.end(); it++) {
        totalSTAT_FrameTime += it->second.STAT_FrameTime;
        totalGameThread += it->second.GameThread;
        totalStat_GPU_Total += it->second.Stat_GPU_Total;
        totalSTAT_MeshDrawCalls += it->second.STAT_MeshDrawCalls;
        totalSTAT_TotalSceneRenderingTime += it->second.STAT_TotalSceneRenderingTime;
        totalSTAT_StaticMeshTriangles += it->second.STAT_StaticMeshTriangles;
    }
    
    storeSummaryData.ActualFrameTime = totalSTAT_FrameTime/num;
    storeSummaryData.ActualGPU = totalStat_GPU_Total/num;
    storeSummaryData.ActualGameThread = totalGameThread/num;
    storeSummaryData.ActualRenderThread = totalSTAT_TotalSceneRenderingTime/num;
    storeSummaryData.ActualMeshDrawCalls = totalSTAT_MeshDrawCalls/num;
    storeSummaryData.ActualTrianglesDrawn = totalSTAT_StaticMeshTriangles/num;
    
    storeSummaryData.MeshDrawCallsDelta = (storeSummaryData.ActualMeshDrawCalls - TargetMeshDrawCalls)/TargetMeshDrawCalls;
    storeSummaryData.FrameTimeDelta = (storeSummaryData.ActualFrameTime - TargetFrameTime)/TargetFrameTime;
    storeSummaryData.TrianglesDrawnDelta = (storeSummaryData.ActualTrianglesDrawn - TargetTriganlesDrawn)/TargetTriganlesDrawn;
    
    if (storeSummaryData.ActualGPU >= storeSummaryData.ActualRenderThread) {
        if (storeSummaryData.ActualGPU>=storeSummaryData.ActualGameThread) {
            storeSummaryData.FrameTiemBound = "GPU Bound";
        }else if(storeSummaryData.ActualRenderThread>=storeSummaryData.ActualGameThread){
            storeSummaryData.FrameTiemBound = "Render Thread Bound";
        }else{
            storeSummaryData.FrameTiemBound = "Game Thread Bound";
        }
    }else{
        if (storeSummaryData.ActualRenderThread>=storeSummaryData.ActualGameThread) {
            storeSummaryData.FrameTiemBound = "Render Thread Bound";
        }else{
            storeSummaryData.FrameTiemBound = "Game Thread Bound";
        }
    }
    
}

OrignalData ConverterOperation::retriveNthData(int n, std::map<FrameId, OrignalData> &storeFiles){
    std::map<FrameId, OrignalData>::iterator it = storeFiles.begin();
    while (n>1) {
        it++;
        n--;
    }
    
    return it->second;
}




