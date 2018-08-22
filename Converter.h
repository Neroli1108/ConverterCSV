#pragma once
// To calculate the average performance data and Convert it to the XLS format file


#include <iostream>
//#include<filesystem>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <dirent.h>

#define TargetFrameTime 100
#define TargetTriganlesDrawn 18000000
#define TargetMeshDrawCalls 18000

using FrameId = int; 

//store the CSV file original data
struct OrignalData 
{	// for identify every frame data
	FrameId ID;

	float STAT_StaticMeshTriangles;
	float STAT_MeshDrawCalls;
	float Stat_GPU_Total;
	float GameThread;
	float STAT_FrameTime;
	float STAT_TotalSceneRenderingTime;
};

//store the data after being calculated
struct SummaryData 
{
	float ActualFrameTime;
	float ActualGameThread;
	float ActualRenderThread;
	float ActualGPU;
	float ActualTrianglesDrawn;
	float ActualMeshDrawCalls;
	float FrameTimeDelta;
	float TrianglesDrawnDelta;
	float MeshDrawCallsDelta;
	std::string FrameTiemBound;

	//record the date


};

// open the specific directory and iterate all CSV files
//
//class ExtractAddress
//{
//public:
//    ExtractAddress() = default;
//    ExtractAddress(std::string address);
//    ~ExtractAddress() = default;
//    std::map<FrameId, OrignalData> getStoreFiles();
//    std::vector<SummaryData> getStoreSummaryData();
//
//
//private:
//    std::filesystem::path* folderPath;
//    std::filesystem::directory_iterator* DirectoryIterator;
//    std::map<FrameId, OrignalData> storeFiles;
//    std::vector<SummaryData> storeSummaryData;
//};
//




//Operate the CSV files
class ConverterOperation
{
public:
	ConverterOperation()=default;
	~ConverterOperation()=default;
    void storeCSVData(std::string address,std::map<FrameId, OrignalData> &storeFiles);
    void calculateSummaryData(std::map<FrameId, OrignalData> &storeFiles,SummaryData& storeSummaryData);
    OrignalData retriveNthData(int n,std::map<FrameId, OrignalData> &storeFiles);
};


//
//
//class Converter
//{
//public:
//    Converter();
//    ~Converter();
//
//private:
//
//};
//
//Converter::Converter()
//{
//
//}
//
//Converter::~Converter()
//{
//}
