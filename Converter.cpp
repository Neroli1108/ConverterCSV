#include "Converter.h"


FileManager::FileManager(std::string address, std::string DestAddress) {
    this->folderPath = new std::filesystem::path(address);
    this->DirectoryIterator = new std::filesystem::directory_iterator(*folderPath);
	this->destFolderPath = new std::filesystem::path(DestAddress);
}

FileManager::FileManager(std::string destfolder) {
	this->destFolderPath = new std::filesystem::path(destfolder);
}


std::filesystem::directory_iterator* FileManager::getDirectoryIterator() {
	return this->DirectoryIterator;
}


std::map<FrameId, OrignalData>& FileManager::getStoreFiles() {
	return this->storeFiles;
}

std::vector<SummaryData>& FileManager::getStoreSummaryData() {
	return this->storeSummaryData;
}

void FileManager::setStoreFiles(OrignalData& od) {
	this->storeFiles[od.ID] = od;
}
void FileManager::setStoreSummaryData(SummaryData& sd) {
	this->storeSummaryData.push_back(sd);
}

void FileManager::recordAllSummaryData() {
	for ( auto sd : this->storeSummaryData)
	{
		std::time_t t = std::time(0);
		std::tm now;

		localtime_s(&now,&t);
		std::ofstream file;
		std::string address = destFolderPath->string()+sd.dataName+".csv";
		file.open(address);
		file << "Date " << std::to_string(now.tm_mon+1) << "/" << std::to_string(now.tm_mday) << "/" << std::to_string(now.tm_year+1900) << std::endl;
		file << "Stat Name,Target,Actual,Delta(%)" << std::endl;
		file << "Frame Time(ms),"+ std::to_string(TargetFrameTime)+"," + std::to_string(sd.ActualFrameTime) + "," + std::to_string((double)(sd.FrameTimeDelta*100))+"%" << std::endl;
		if (sd.FrameTimeBound == "GPU Bound") {
			file << " -Game Thread, ," + std::to_string(sd.ActualGameThread) + ", ,"  << std::endl;
			file << " -Render Thread, ," + std::to_string(sd.ActualRenderThread) + ", ," << std::endl;
			file << " -GPU, ," + std::to_string(sd.ActualGPU) + "," + sd.FrameTimeBound<< std::endl;
		}
		else if (sd.FrameTimeBound == "Game Thread Bound") {

			file << " -Game Thread, ," + std::to_string(sd.ActualGameThread) + ","+sd.FrameTimeBound << std::endl;
			file << " -Render Thread, ," + std::to_string(sd.ActualRenderThread) + ", ," << std::endl;
			file << " -GPU, ," + std::to_string(sd.ActualGPU) + ", ," << std::endl;
		}
		else
		{
			file << " -Game Thread, ," + std::to_string(sd.ActualGameThread) + ", ," << std::endl;
			file << " -Render Thread, ," + std::to_string(sd.ActualRenderThread) + ","+ sd.FrameTimeBound << std::endl;
			file << " -GPU, ," + std::to_string(sd.ActualGPU) + ", ," << std::endl;
		}
		file << "Triangles Drawn,"+ std::to_string(TargetTriganlesDrawn)+ ","+ std::to_string(sd.ActualTrianglesDrawn) + "," + std::to_string((double)(sd.TrianglesDrawnDelta*100)) +"%"<< std::endl;
		file << "Mesh Drawn Calls," + std::to_string(TargetMeshDrawCalls) + "," + std::to_string(sd.ActualMeshDrawCalls) + "," + std::to_string((double)(sd.MeshDrawCallsDelta * 100)) + "%" << std::endl;
		file.close();

	}

}



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


void ConverterOperation::calculateSummaryData(std::map<FrameId, OrignalData> &storeFiles,SummaryData& storeSummaryData,std::string name){
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
	storeSummaryData.dataName = name;
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
            storeSummaryData.FrameTimeBound = "GPU Bound";
        }else if(storeSummaryData.ActualRenderThread>=storeSummaryData.ActualGameThread){
            storeSummaryData.FrameTimeBound = "Render Thread Bound";
        }else{
            storeSummaryData.FrameTimeBound = "Game Thread Bound";
        }
    }else{
        if (storeSummaryData.ActualRenderThread>=storeSummaryData.ActualGameThread) {
            storeSummaryData.FrameTimeBound = "Render Thread Bound";
        }else{
            storeSummaryData.FrameTimeBound = "Game Thread Bound";
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



Converter::Converter(std::string src = "D:/CSV/", std::string dst = "D:/Result/") {
	this->fm = new FileManager(src, dst);
	this->co = new ConverterOperation();
}

void Converter::storeData() {

	for (auto it : *(this->fm->getDirectoryIterator()))
	{
		SummaryData sd;
		auto p = it.path();
		this->co->storeCSVData(p.string(), this->fm->getStoreFiles());
		this->co->calculateSummaryData(this->fm->getStoreFiles(), sd, p.filename().string().substr(0, p.filename().string().size() - 4));
		this->fm->setStoreSummaryData(sd);
		this->fm->getStoreFiles().clear();
	}
}


void Converter::storeSingleFileData(std::string srcfile) {
	this->co->storeCSVData(srcfile, this->fm->getStoreFiles());
}


void Converter::writeSingleFileSummary(std::string srcfile, std::string dstfolder) {
	SummaryData sd;
	std::string name = srcfile.substr(srcfile.find_last_of("/\\")+1);
	this->fm = new FileManager(dstfolder);
	this->storeSingleFileData(srcfile);
	this->co->calculateSummaryData(this->fm->getStoreFiles(),sd,name);
	this->fm->setStoreSummaryData(sd);
	this->fm->recordAllSummaryData();

}

void Converter::writeSummary() {
	this->fm->recordAllSummaryData();
}

void Converter::showSpecificdata(std::string src, int n) {
	this->storeSingleFileData(src);
	OrignalData od = this->co->retriveNthData(n,this->fm->getStoreFiles());
	std::cout << "Frame ID: " << od.ID << std::endl;
	std::cout << "STAT_StaticMeshTriangles: " << od.STAT_StaticMeshTriangles << std::endl;
	std::cout << "STAT_MeshDrawCalls: " << od.STAT_MeshDrawCalls << std::endl;
	std::cout << "Stat_GPU_Total: " << od.Stat_GPU_Total << std::endl;
	std::cout << "GameThread: " << od.GameThread << std::endl;
	std::cout << "STAT_FrameTime: " << od.STAT_FrameTime << std::endl;
	std::cout << "STAT_TotalSceneRenderingTime: " << od.STAT_TotalSceneRenderingTime << std::endl;

}

Converter::Converter()
{
	fm = new FileManager();
	co = new ConverterOperation();
}

Converter::~Converter()
{
}