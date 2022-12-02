#include "GarbageCollector.h"
#include "FilesFunctionality.h"

#include <fstream>

garbcoll::GarbageCollector::GarbageCollector()
{
    this->pos = -1;
    for (int i = 0; i < 20; i++)
        this->record_numbers[i] = -1;
}
void garbcoll::GarbageCollector::add(int to_add)
{
    if (pos == 19)
    {
        std::cout << "Garbage collector overflow\n\n";
        return;
    }
    this->pos++;
    this->record_numbers[this->pos] = to_add;
}
int garbcoll::GarbageCollector::getLastRecordNumber()
{
    if (this->pos == -1) return -1;
    int result = this->record_numbers[this->pos];
    this->record_numbers[this->pos] = -1;
    this->pos--;
    return result;
}

void garbcoll::addRecordNumberToGarbage(GarbageCollector& collector, int record_number, char* GARBAGE_FILE_NAME, int option)
{
    // option - to which file add record number
    // 0: master-index
    // 1: slave
    collector.add(record_number);
    writeToFileStructure<GarbageCollector>(collector, GARBAGE_FILE_NAME, option);
    return;
}
void garbcoll::garbageCollectorInitilialization(char* FILE_NAME)
{
    GarbageCollector collector;
    std::ofstream in(FILE_NAME, std::ofstream::binary | std::ofstream::app);
    in.write((char*)&collector, sizeof(collector)); // 0: master-index (lecturer-index)
    in.write((char*)&collector, sizeof(collector)); // 1: slave (scientific publication)
    in.close();
    return;
}

int garbcoll::getGarbageCollectorLastRecord(char* GARBAGE_FILE_NAME, int option)
{
    // option - to which file add record number
    // 0: master-index
    // 1: slave
    garbcoll::GarbageCollector temp = readFromFileStructure<garbcoll::GarbageCollector>(GARBAGE_FILE_NAME, option);
    return temp.getLastRecordNumber();
}

