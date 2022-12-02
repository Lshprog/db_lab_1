#pragma once
#include <fstream>
#include <string>
#include <iostream>

template <class T>
void writeToFileStructure(T structure, char* FILE_NAME, int position = -1)
{
    //position:
    //-1 in the end

    if (position == -1)
    {
        std::ofstream file(FILE_NAME, std::ofstream::binary | std::ofstream::app);
        file.write((char*)&structure, sizeof(structure));
        file.close();
        return;
    }
    std::fstream in(FILE_NAME, std::ifstream::in | std::ios::out | std::ios::binary);
    int pos = sizeof(structure) * position;
    in.clear();
    in.seekp(pos, std::ofstream::beg);
    in.write((char*)&structure, sizeof(structure));
    in.close();
    return;
}

template <class T>
T readFromFileStructure(char* FILE_NAME, int position = -1)
{
    //position:
    //-1 in the end

    std::ifstream file(FILE_NAME, std::ifstream::in | std::ifstream::binary);
    T buffer;
    file.clear();
    if (file.peek() == std::ifstream::traits_type::eof()) return buffer;
    int pos = sizeof(buffer) * position;
    file.clear();
    if (position != -1)
        file.seekg(pos, std::ofstream::beg);
    file.read((char*)&buffer, sizeof(buffer));
    file.close();
    return buffer;
}

template <class T>
int getLastPos(char* FILE_NAME, T structure)
{
    std::ifstream file(FILE_NAME, std::ifstream::in | std::ifstream::binary);
    if (file)
    {
        file.clear();
        file.seekg(0, file.end);
        int length = file.tellg();
        int length = length / sizeof(structure);
        return length;
    }
    return 0;
}