#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>

#define MB 1000000
#define RAMTIME 0.00033333333333333
#define HDDTIME 0.011
#define SSDTIME 0.05

using namespace std;
using namespace std::chrono;

int sizeString (string str) {
    string temp = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '|') {
            break;
        }
        temp += str[i];
    }
    return stoi(temp);
}

void fileWriting(int *arr, size_t arrSize, string filePath) {
    ofstream out;
    out.open(filePath);
    if (!out.is_open()) {
        cout << "open error" << endl;
        return;
    }
    for (size_t i = 0; i < arrSize; i++) {
        out << arr[i] << " ";
    }
    out.close();
}

int ramTimeWrite(int launchCount, int sum, int arrSize, int* arr, int blockSize) {
    int j;
    j = launchCount;
    double * launchDuration = new double[launchCount];
    auto startOutLoop = high_resolution_clock::now();
    while (j > 0) {
        auto launchStart = high_resolution_clock::now();
        for (int i = 0; i < arrSize; i++) {
            arr[i] = rand() % arrSize + 1;
        }
        auto launchStop = high_resolution_clock::now();
        duration<double> launchDuration_t = duration_cast<microseconds>(launchStop - launchStart);
        launchDuration[launchCount - j] = launchDuration_t.count();
        sum += launchDuration_t.count();
        j--;
    }
    auto stopOutLoop = high_resolution_clock::now();
    duration<double> duration_t = duration_cast<microseconds>(stopOutLoop - startOutLoop);
    for (int i = 0; i < launchCount; i++) {
            cout << "RAM;"
            << blockSize << ";"
            << "int;" 
            << arrSize << ";" 
            <<  i + 1 << ";" 
            << "now(), lib chrono;" 
            << launchDuration[i] << ";" 
            <<  sum / launchCount << ";" 
            << (blockSize / (sum / launchCount)) * MB << ";" 
            << abs(RAMTIME - duration_t.count()) << ";" 
            << ((duration_t.count() / launchCount > RAMTIME / launchCount) ? (duration_t.count() / launchCount) / (RAMTIME / launchCount) : (RAMTIME / launchCount) / (duration_t.count() / launchCount)) << "%;"
            << endl;
        }
    return 0;
}

void writeTable() {
    cout << "MemoryType;" 
    << "BlockSize;" 
    << "ElementType;" 
    << "BufferSize;" 
    << "LaunchNum;" 
    << "Timer;" 
    << "WriteTime;" 
    << "AverageWriteTime;" 
    << "WriteBandwidth;" 
    << "AbsError(write);"
    << "RelError(write);" 
    << "ReadTime;" 
    << "AverageReadTime;" 
    << "ReadBandwidthAbsError(read);" 
    << "RelError(read);" 
    << endl;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    string memoryType = "";
    int blockSize;
    string elementType = "";
    int bufferSize;
    int launchNum;
    string timer = "now()";
    double writeTime = 0.0;
    double avgWriteTime = 0.0;
    //WriteBandwidth – пропускная способность памяти (BLOCK_SIZE/AverageWriteTime) * 10^6
    double absErrorWrite = 0.0;
    double relErrorWrite = 0.0;
    double readTime = 0.0;
    double avgReadTime = 0.0;
    //ReadBandwidth – пропускная способность памяти (BLOCK_SIZE/AverageReadTime) * 10^6
    double absErrorRead = 0.0;
    double relErrorRead = 0.0;
    
    string tempBlockSize = "";
    int launchCount;
    double sum = 0.0;

    cout.precision(12);
	// auto startOutLoop = high_resolution_clock::now();
    // auto stopOutLoop = high_resolution_clock::now();
    // auto launchStart = high_resolution_clock::now();
    // auto launchStop = high_resolution_clock::now();

    if (argc > 1) {
        memoryType = argv[2];
    }
    if (argc > 3) {
        tempBlockSize = argv[4];
        blockSize = sizeString(tempBlockSize);
    }
    if (argc > 5) {
        launchCount = atoi(argv[6]);
    }
    int arrSize = blockSize / sizeof(int);
    int * arr = new int [arrSize];

    for (int i = 0; i < arrSize; i++) {
        arr[i] = rand() % arrSize + 1;
    }
    //duration<double> duration_t = duration_cast<microseconds>(stopOutLoop - startOutLoop);
    //duration<double> launchDuration_t = duration_cast<microseconds>(stopOutLoop - startOutLoop);

    string filePath = "";

    writeTable();

    if (memoryType == "RAM") {
        memoryType = "RAM";
        elementType = "int";
        bufferSize = sizeof(int);
        int j;
        j = launchCount;
        double * launchDuration = new double[launchCount];
        auto startOutLoop = high_resolution_clock::now();
        while (j > 0) {
            auto launchStart = high_resolution_clock::now();
            for (int i = 0; i < arrSize; i++) {
                arr[i] = rand() % arrSize + 1;
            }
            auto launchStop = high_resolution_clock::now();
            duration<double> launchDuration_t = duration_cast<microseconds>(launchStop - launchStart);
            launchDuration[launchCount - j] = launchDuration_t.count();
            sum += launchDuration_t.count();
            j--;
        }
        auto stopOutLoop = high_resolution_clock::now();
        duration<double> duration_t = duration_cast<microseconds>(stopOutLoop - startOutLoop);
        for (int i = 0; i < launchCount; i++) {
                launchNum = i + 1;
                cout << memoryType << ";"
                << blockSize << ";"
                << elementType << ";"
                << bufferSize << ";" 
                <<  launchNum << ";" 
                << timer << ";" 
                << launchDuration[i] << ";" 
                <<  sum / launchCount << ";" 
                << (blockSize / (sum / launchCount)) * MB << ";" 
                << abs(RAMTIME - duration_t.count()) << ";" 
                << ((duration_t.count() / launchCount > RAMTIME / launchCount) ? (duration_t.count() / launchCount) / (RAMTIME / launchCount) : (RAMTIME / launchCount) / (duration_t.count() / launchCount)) << "%;"
                << endl;
            }
            delete[] launchDuration;
    } else if(memoryType == "HDD") {
        filePath = "/mnt/d/Over/memorytest.txt";
        memoryType = "HDD";
        elementType = "int";
        bufferSize = sizeof(int);
        int j = launchCount;
        double * launchDuration = new double[launchCount];
        auto startOutLoop = high_resolution_clock::now();
        while (j > 0) {
            auto launchStart = high_resolution_clock::now();
            fileWriting(arr, arrSize, filePath);
            auto launchStop = high_resolution_clock::now();
            duration<double> launchDuration_t = duration_cast<microseconds>(launchStop - launchStart);
            launchDuration[launchCount - j] = launchDuration_t.count();
            sum += launchDuration_t.count();
            j--;
        }
        auto stopOutLoop = high_resolution_clock::now();
        duration<double> duration_t = duration_cast<microseconds>(stopOutLoop - startOutLoop);
        for (int i = 0; i < launchCount; i++) {
                launchNum = i + 1;
                cout << memoryType << ";"
                << blockSize << ";"
                << elementType << ";"
                << bufferSize << ";" 
                << launchNum << ";" 
                << timer << ";" 
                << launchDuration[i] << ";" 
                <<  sum / launchCount << ";" << (blockSize / (sum / launchCount)) * MB << ";" << abs(HDDTIME - duration_t.count()) << ";" 
                << ((duration_t.count() / launchCount > HDDTIME / launchCount) ? (duration_t.count() / launchCount) / (HDDTIME / launchCount * 1024) :
                 (HDDTIME / launchCount) / (duration_t.count() / launchCount * 1024)) << "%;" << endl;
        }
        delete [] launchDuration;
    } else if(memoryType == "SSD") {
        filePath = "/mnt/c/Over/memorytest.txt";
        memoryType = "SSD";
        elementType = "int";
        bufferSize = sizeof(int);
        int j = launchCount;
        double * launchDuration = new double[launchCount];
        auto startOutLoop = high_resolution_clock::now();
        while (j > 0) {
            auto launchStart = high_resolution_clock::now();
            fileWriting(arr, arrSize, filePath);
            auto launchStop = high_resolution_clock::now();
            duration<double> launchDuration_t = duration_cast<microseconds>(launchStop - launchStart);
            launchDuration[launchCount - j] = launchDuration_t.count();
            sum += launchDuration_t.count();
            j--;
        }
        auto stopOutLoop = high_resolution_clock::now();
        duration<double> duration_t = duration_cast<microseconds>(stopOutLoop - startOutLoop);
        for (int i = 0; i < launchCount; i++) {
                launchNum = i + 1;
                cout << memoryType << ";"
                << blockSize << ";"
                << elementType << ";"
                << bufferSize << ";" 
                << launchNum << ";" 
                << timer << ";" 
                << launchDuration[i] << ";" 
                <<  sum / launchCount << ";" << (blockSize / (sum / launchCount)) * MB << ";" << abs(HDDTIME - duration_t.count()) << ";" 
                << ((duration_t.count() / launchCount > HDDTIME / launchCount) ? (duration_t.count() / launchCount) / (HDDTIME / launchCount * 1024) :
                 (HDDTIME / launchCount) / (duration_t.count() / launchCount * 1024)) << "%;" << endl;
        }
        delete [] launchDuration;
    } else if(memoryType == "FLASH") {

    } else {
        return -1;
    }
    delete [] arr;
    return 0;
}