#include "Product.h"
#include "Plant.h"
#include "FilesFunctionality.h"
#include "GarbageCollector.h"

#include <fstream>
#include <string>
#include <iostream>

products::Product::Product()
{
	this->id = -1;
	this->id_plant = -1;
	strcpy_s(this->name, " ");
	strcpy_s(this->description, " ");
	
	this->pos_next_prod = -1;
	this->pos_prev_prod = -1;
	this->flag_deleted = false;
}
products::Product::Product(int id, int lecturer_id, char* name, char* description)
{
	this->id = id;
	this->id_plant = lecturer_id;
	strcpy_s(this->name, name);
	strcpy_s(this->description, description);
	
	this->pos_next_prod = -1;
	this->pos_prev_prod = -1;
	this->flag_deleted = false;
}
void products::Product::print()
{
	std::cout << this->name << " | ";
	std::cout << this->id << " | ";
	std::cout << this->id_plant << " | ";
	std::cout << this->description << "\n";
}

//get-s
products::Product products::getProductByIdAndPlantId(int id, int id_plant, char* PRODUCTS_FILE_NAME,
	char* PLANT_FILE_NAME, plants::PlantIndexTable idx_table)
{
	{
		plants::Plant plant = plants::getPlantById(id_plant, PLANT_FILE_NAME, idx_table);
		products::Product buffer;
		if (plant.id == -1)
			return buffer;
		int pos = plant.product_beg;
		int amount = plant.number_of_products;
		std::ifstream file(PRODUCTS_FILE_NAME, std::ifstream::in | std::ifstream::binary);
		file.seekg(sizeof(buffer) * pos, std::ifstream::beg);
		for (int i = 0; i <= amount; i++)
		{
			file.read((char*)&buffer, sizeof(buffer));
			if (buffer.flag_deleted == true)
			{
				i--;
				continue;
			}
			if (buffer.id == id)
				return buffer;
			file.seekg(sizeof(buffer) * buffer.pos_next_prod, std::ifstream::beg);

		}
		file.close();
		products::Product temp;
		return temp;
	}
}
//insert-s
void products::insertProduct(int id, int id_plant, char* name, char* description,
	char* FILE_NAME, char* PLANT_FILE_NAME, char* GARBAGE_FILE_NAME, plants::PlantIndexTable& idx_table)
{
	int garb = garbcoll::getGarbageCollectorLastRecord(GARBAGE_FILE_NAME, 1);
	plants::PlantIndex idx = idx_table.getPositionOfPlantById(id_plant);
	plants::Plant plant = readFromFileStructure<plants::Plant>(PLANT_FILE_NAME, idx.position);
	if (plant.id == -1)
	{
		std::cout << "There is no plant with such an ID\n\n";
		return;
	}
	products::Product prod(id, id_plant, name, description);
	products::Product buffer;
	std::fstream file(FILE_NAME, std::fstream::in | std::fstream::out | std::fstream::binary);
	file.seekg(0, file.end);
	int length = garb;

	if (garb == -1)
	{
		length = file.tellg();
		length = length / sizeof(prod);
	}

	int cur_pos = plant.product_beg;;
	if (cur_pos == -1)
	{
		plant.product_beg = length;
		plant.number_of_products++;
		file.write((char*)&prod, sizeof(prod));
		file.close();
		writeToFileStructure<plants::Plant>(plant, PLANT_FILE_NAME, idx.position);
		return;
	}

	for (int i = 0; i < plant.number_of_products; i++)
	{
		file.clear();
		file.seekg(sizeof(buffer) * cur_pos, std::ifstream::beg);
		file.read((char*)&buffer, sizeof(buffer));
		if (buffer.flag_deleted == true)
		{
			i--;
			continue;
		}
		cur_pos = buffer.pos_next_prod;
	}
	buffer.pos_next_prod = length;

	int temp = static_cast<int>(sizeof(buffer));
	file.seekg(-temp, std::ifstream::cur);

	int temp_pos = file.tellg() / sizeof(prod);
	prod.pos_prev_prod = temp_pos;

	file.write((char*)&buffer, sizeof(buffer));
	file.seekg(0, file.end);
	file.write((char*)&prod, sizeof(prod));
	file.close();
	plant.number_of_products++;
	writeToFileStructure<plants::Plant>(plant, PLANT_FILE_NAME, idx.position);
	return;
}
//update-s
void products::updateProductById(int id, char* name,char* description, char* FILE_NAME, plants::PlantIndexTable& idx_table)
{
	std::fstream file(FILE_NAME, std::fstream::in | std::fstream::out | std::fstream::binary);
	products::Product buffer;
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		std::cout << "There are no products.\n\n";
		return;
	}
	while (!file.eof())
	{
		file.read((char*)&buffer, sizeof(buffer));
		if (!file.good())
			break;
		if (buffer.id == id)
		{
			if (buffer.flag_deleted == true)
			{
				std::cout << "This product is deleted.";
				return;
			}
			strcpy_s(buffer.name, name);
			strcpy_s(buffer.description, description);
			int temp = static_cast<int>(sizeof(buffer));
			file.seekg(-temp, std::ifstream::cur);
			file.write((char*)&buffer, sizeof(buffer));
			file.close();
			return;
		}
		continue;
	}
	file.close();
	std::cout << "Product with such an ID doesn't exist.\n\n";
	return;
}
//del-s
void products::delProductById(int id, int id_plant, char* FILE_NAME, char* PLANT_FILE_NAME, char* GARBAGE_FILE, plants::PlantIndexTable& idx_table)
{
	int lect_pos = idx_table.getPositionOfPlantById(id_plant).position;
	plants::Plant prod = plants::getPlantById(id_plant, PLANT_FILE_NAME, idx_table);
	if (prod.flag_deleted == true || prod.product_beg == -1)
	{
		std::cout << "There is no such a Product.\n\n";
		return;
	}

	std::fstream file(FILE_NAME, std::fstream::in | std::fstream::out | std::fstream::binary);
	products::Product buffer;
	int cur_pos = prod.product_beg;;
	for (int i = 0; i < prod.number_of_products; i++)
	{
		file.clear();
		file.seekg(sizeof(buffer) * cur_pos, std::ifstream::beg);
		file.read((char*)&buffer, sizeof(buffer));
		if (buffer.id == id)
		{
			if (buffer.flag_deleted == true)
			{
				std::cout << "This Product is already deleted.\n\n";
				return;
			}
			buffer.flag_deleted = true;
			if (cur_pos == prod.product_beg)
				prod.product_beg = buffer.pos_next_prod;
			file.close();

			if (buffer.pos_prev_prod != -1)
			{
				products::Product temp1 = readFromFileStructure<products::Product>(FILE_NAME, buffer.pos_prev_prod);
				temp1.pos_next_prod = buffer.pos_next_prod;
				writeToFileStructure<products::Product>(temp1, FILE_NAME, buffer.pos_prev_prod);
			}

			//rewrite the next
			if (buffer.pos_next_prod != -1)
			{
				products::Product temp2 = readFromFileStructure<products::Product>(FILE_NAME, buffer.pos_next_prod);
				temp2.pos_prev_prod = buffer.pos_prev_prod;
				writeToFileStructure<products::Product>(temp2, FILE_NAME, buffer.pos_next_prod);
			}

			prod.number_of_products--;
			buffer.flag_deleted = true;
			garbcoll::GarbageCollector garb = readFromFileStructure<garbcoll::GarbageCollector>(GARBAGE_FILE, 1);
			garb.add(cur_pos);
			writeToFileStructure<plants::Plant>(prod, PLANT_FILE_NAME, lect_pos);
			writeToFileStructure<garbcoll::GarbageCollector>(garb, GARBAGE_FILE, 1);
			writeToFileStructure<products::Product>(buffer, FILE_NAME, cur_pos);
			return;
		}
		if (buffer.flag_deleted == true)
			i--;
		cur_pos = buffer.pos_next_prod;
	}
	std::cout << "There is no such a product\n\n";
	return;
}

void products::printProductByIdAndPlantId(int id, int id_plant, char* PRODUCTS_FILE_NAME,
	char* PLANT_FILE_NAME, plants::PlantIndexTable idx_table)
{
	products::Product prod = products::getProductByIdAndPlantId(id, id_plant, PRODUCTS_FILE_NAME, PLANT_FILE_NAME, idx_table);
	if (prod.id != -1)
	{
		std::cout << "NAME | ";
		std::cout << "ID | ";
		std::cout << "PLANT_ID | ";
		std::cout << "DESCRIPTION\n";
		prod.print();
	}
	else std::cout << "Incorrect input!\n\n";
}
void products::printProductByTheSamePlant(int id_plant, char* PRODUCTS_FILE_NAME,
	char* PLANT_FILE_NAME, plants::PlantIndexTable idx_table)
{
	plants::Plant plant = plants::getPlantById(id_plant, PLANT_FILE_NAME, idx_table);
	products::Product buffer;
	if (plant.id == -1)
		std::cout << "There is no plant with such an id.\n\n";
	int pos = plant.product_beg;;
	int amount = plant.number_of_products;
	std::ifstream file(PRODUCTS_FILE_NAME, std::ifstream::in | std::ifstream::binary);
	file.seekg(sizeof(buffer) * pos, std::ifstream::beg);
	std::cout << "NAME | ";
	std::cout << "ID | ";
	std::cout << "PLANT_ID | ";
	std::cout << "DESCRIPTION \n";
	for (int i = 1; i <= amount; i++)
	{
		file.read((char*)&buffer, sizeof(buffer));
		if (buffer.flag_deleted == true)
		{
			i--;
			continue;
		}
		buffer.print();
		file.seekg(sizeof(buffer) * buffer.pos_next_prod, std::ifstream::beg);
	}
	file.close();
}
void products::printAllProducts(char* PRODUCTS_FILE_NAME)
{
	std::ifstream file(PRODUCTS_FILE_NAME, std::ifstream::in | std::ifstream::binary);
	Product buffer;
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		std::cout << "There are no Products.\n\n";
		return;
	}
	std::cout << "NAME | ";
	std::cout << "ID | ";
	std::cout << "PLANT_ID | ";
	std::cout << "DESCRIPTION \n";
	while (!file.eof())
	{
		file.read((char*)&buffer, sizeof(buffer));
		if (!file.good())
			break;
		if (buffer.flag_deleted != false)
			continue;
		if (buffer.id != -1) buffer.print();
	}
	file.close();
}


