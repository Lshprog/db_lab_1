#pragma once

#include <fstream>
#include <iostream>

template<class T>
void writeToFileStruct(T structure, char* FILE_NAME, int position);

namespace garbcoll
{

    struct GarbageCollector
    {
        int record_numbers[20];
        int pos;
        GarbageCollector();
        void add(int to_add);
        int getLastRecordNumber();
    };
    void addRecordNumberToGarbage(GarbageCollector& collector, int record_number, char* FILE_NAME, int option);
    void garbageCollectorInitilialization(char* FILE_NAME);
    int getGarbageCollectorLastRecord(char* GARBAGE_FILE_NAME, int option);
}