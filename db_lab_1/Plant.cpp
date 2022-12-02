#include "Plant.h"
#include "FilesFunctionality.h"
#include "GarbageCollector.h"
#include "Product.h"

#include <fstream>
#include <string>
#include <iostream>


plants::Plant::Plant()
{
	this->id = -1;
	strcpy_s(this->name, " ");
	strcpy_s(this->city, " ");
	this->number_of_workers = -1;
	this->number_of_products = 0;
	this->product_beg = -1;
	this->flag_deleted = false;
}
plants::Plant::Plant(int id, char* name, char* city, int number_of_workers)
{
	this->id = id;
	strcpy_s(this->name,name);
	strcpy_s(this->city, city);
	this->number_of_workers = number_of_workers;
	this->number_of_products = 0;
	this->product_beg = -1;
	this->flag_deleted = false;
}
void plants::Plant::setDeleted()
{
	this->flag_deleted = true;
}

void plants::Plant::printinfo()
{

	std::cout << this->name << " | ";
	std::cout << this->id << " | ";
	std::cout << this->city << " | ";
	std::cout << this->number_of_workers << " | ";
	std::cout << this->number_of_products << std::endl;
	
}


const int INDEX_TABLE_CAPACITY = 20;

void plants::merge(plants::PlantIndex array[], int left, int mid, int right)
{
	int left_array_length = mid - left + 1;
	int right_array_length = right - mid;

	// Create temp arrays
	auto* left_array = new plants::PlantIndex[left_array_length],
		* right_array = new plants::PlantIndex[right_array_length];

	for (int i = 0; i < left_array_length; i++)
		left_array[i] = array[left + i];
	for (int j = 0; j < right_array_length; j++)
		right_array[j] = array[mid + 1 + j];

	int index_left_array = 0,
		index_right_array = 0,
		index_merged_array = left;

	while (index_left_array < left_array_length && index_right_array < right_array_length)
	{
		if (left_array[index_left_array].id < right_array[index_right_array].id)
		{
			array[index_merged_array] = left_array[index_left_array];
			index_left_array++;
			index_merged_array++;
		}
		else
		{
			array[index_merged_array] = right_array[index_right_array];
			index_right_array++;
			index_merged_array++;
		}
	}

	while (index_left_array < left_array_length)
	{
		array[index_merged_array] = left_array[index_left_array];
		index_left_array++;
		index_merged_array++;
	}

	while (index_right_array < right_array_length)
	{
		array[index_merged_array] = right_array[index_right_array];
		index_right_array++;
		index_merged_array++;
	}

	delete[] left_array;
	delete[] right_array;
}
void plants::mergeSort(plants::PlantIndex array[], int begin, int end)
{
	if (begin >= end)
		return;

	int mid = begin + (end - begin) / 2;
	mergeSort(array, begin, mid);
	mergeSort(array, mid + 1, end);
	merge(array, begin, mid, end);
}

plants::PlantIndex::PlantIndex()
{
	this->id = -1;
	this->position = -1;
	this->flag_deleted = false;
}
plants::PlantIndex::PlantIndex(int id)
{
	this->id = id;
	this->position = -1;
	this->flag_deleted = false;
}
plants::PlantIndexTable::PlantIndexTable()
{
	for (int i = 0; i <= INDEX_TABLE_CAPACITY; i++)
	{
		this->idx_cells[i];
	}
	this->cur_pos = -1;
}


plants::PlantIndex plants::PlantIndexTable::getPositionOfPlantById(int id)
{
	for (int i = 0; i <= this->cur_pos; i++)
	{
		if (this->idx_cells[i].id == id && this->idx_cells[i].flag_deleted != true) return this->idx_cells[i];
	}
	return PlantIndex(-1);
}
int plants::PlantIndexTable::getIndexPositionOfPlantById(int id)
{
	for (int i = 0; i <= this->cur_pos; i++)
	{
		if (this->idx_cells[i].id == id) return i;
	}
	return -1;
}
void plants::PlantIndexTable::sortById()
{
	mergeSort(this->idx_cells, 0, this->cur_pos);
}
void plants::PlantIndexTable::posNext(int pos)
{
	plants::PlantIndex temp = this->idx_cells[pos];
	for (int i = pos; i < this->cur_pos; i++)
		this->idx_cells[i] = this->idx_cells[i + 1];
	this->idx_cells[this->cur_pos] = temp;
	this->idx_cells[this->cur_pos].flag_deleted = true;
}
bool plants::PlantIndexTable::add(int id)
{
	if (this->cur_pos == 19)
		return false;
	this->cur_pos++;
	this->idx_cells[this->cur_pos] = PlantIndex(id);
	this->idx_cells[this->cur_pos].position = this->cur_pos;
	this->sortById();
	return true;
}
int plants::PlantIndexTable::findFirstDeletedPlantIdx()
{
	for (int i = 0; i <= this->cur_pos; i++)
		if (this->idx_cells[i].flag_deleted == true) return i;
}


//get-m
plants::Plant plants::getPlantById(int id, char* FILE_NAME, PlantIndexTable idx_table)
{
	int pos = idx_table.getPositionOfPlantById(id).position;
	Plant buffer;
	if (pos == -1)
		return buffer;
	return readFromFileStructure<Plant>(FILE_NAME, pos);
}
//insert-m
void plants::insertPlant(int id, char* name, int number_of_workers, char* city,
	char* FILE_NAME, char* GARBAGE_FILE_NAME, PlantIndexTable& idx_table)
{
	plants::Plant plant(id, name,city,number_of_workers);
	int garbage_idx_pos = garbcoll::getGarbageCollectorLastRecord(GARBAGE_FILE_NAME, 0);
	if (garbage_idx_pos != -1)
	{
		int pos = idx_table.idx_cells[garbage_idx_pos].position;
		plants::PlantIndex temp(id);
		temp.position = pos;
		idx_table.idx_cells[garbage_idx_pos] = temp;
		writeToFileStructure<plants::Plant>(plant, FILE_NAME, pos);
		return;
	}
	if (!idx_table.add(id))
	{
		std::cout << "Index table overflow!";
		return;
	}
	writeToFileStructure<plants::Plant>(plant, FILE_NAME, -1);

}
//update-m
void plants::updatePlant(int id, int number_of_workers,
	char* FILE_NAME, PlantIndexTable& idx_table)
{
	int pos = idx_table.getPositionOfPlantById(id).position;
	if (pos == -1)
	{
		std::cout << "There is no plant with such an ID.\n\n";
		return;
	}

	PlantIndex idx = idx_table.getPositionOfPlantById(id);
	Plant plant = readFromFileStructure<plants::Plant>(FILE_NAME, idx.position);
		
	plant.number_of_workers = number_of_workers;

	writeToFileStructure<plants::Plant>(plant, FILE_NAME, idx.position);
	return;
}
//del-m
void plants::deletePlantById(int id, char* FILE_NAME, char* PUBLICATIONS_FILE_NAME, char* GARBAGE_FILE_NAME, PlantIndexTable& idx_table)
{
	plants::Plant plant = plants::getPlantById(id, FILE_NAME, idx_table);
	if (plant.flag_deleted == true || plant.number_of_workers == -1)
	{
		std::cout << "There is no such a plant.\n\n";
		return;
	}
	int idx_pos = idx_table.getIndexPositionOfPlantById(id);
	int pos = idx_table.getPositionOfPlantById(id).position;
	idx_table.idx_cells[idx_pos].flag_deleted = true;
	plant.flag_deleted = true;
	idx_table.posNext(idx_pos);
	writeToFileStructure<plants::Plant>(plant, FILE_NAME, pos);
	garbcoll::GarbageCollector garb_col = readFromFileStructure<garbcoll::GarbageCollector>(GARBAGE_FILE_NAME, 0);
	garb_col.add(idx_pos);
	writeToFileStructure<garbcoll::GarbageCollector>(garb_col, GARBAGE_FILE_NAME, 0);


	int cur_pos = plant.product_beg;
	products::Product buffer;
	std::fstream file(PUBLICATIONS_FILE_NAME, std::fstream::in | std::fstream::out | std::fstream::binary);
	garbcoll::GarbageCollector garb_prod = readFromFileStructure<garbcoll::GarbageCollector>(GARBAGE_FILE_NAME, 1);
	for (int i = 0; i < plant.number_of_products; i++)
	{
		file.clear();
		file.seekg(sizeof(buffer) * cur_pos, std::ifstream::beg);
		file.read((char*)&buffer, sizeof(buffer));
		if (buffer.flag_deleted == false)
		{
			garb_prod.add(cur_pos);
			buffer.flag_deleted = true;
			writeToFileStructure<products::Product>(buffer, PUBLICATIONS_FILE_NAME, cur_pos);
		}
		else if (buffer.flag_deleted == true)
			i--;
		cur_pos = buffer.pos_next_prod;
	}
	file.close();
	writeToFileStructure<garbcoll::GarbageCollector>(garb_prod, GARBAGE_FILE_NAME, 1);
	return;
}

void plants::printPlantById(int id, char* FILE_NAME, PlantIndexTable idx_table)
{
	plants::Plant plant = plants::getPlantById(id, FILE_NAME, idx_table);
	if (plant.id != -1)
	{
		std::cout << "NAME | ";
		std::cout << "ID | ";
		std::cout << "CITY | ";
		std::cout << "NUMBER OF WORKERS | ";
		std::cout << "NUMBER OF PRODUCTS\n ";
		plant.printinfo();
	}
	else std::cout << "There is no plants with such an ID.\n\n";
	return;
}
void plants::printAllPlants(char* FILE_NAME)
{
	std::ifstream index_file(FILE_NAME, std::ifstream::in | std::ifstream::binary);
	Plant buffer;
	if (index_file.peek() == std::ifstream::traits_type::eof())
	{
		std::cout << "There are no plants.\n\n";
		return;
	}
	std::cout << "NAME | ";
	std::cout << "ID | ";
	std::cout << "CITY | ";
	std::cout << "NUMBER OF WORKERS | ";
	std::cout << "NUMBER OF PRODUCTS\n ";

	while (!index_file.eof())
	{
		index_file.read((char*)&buffer, sizeof(buffer));
		if (!index_file.good())
			break;
		if (buffer.flag_deleted != false)
			continue;
		buffer.printinfo();
	}
	index_file.close();
}


