#pragma once
#include "Plant.h"

namespace products
{
	struct Product
	{
		int id;
		int id_plant;
		char name[20];
		char description[30];
		
		int pos_prev_prod;
		int pos_next_prod;
		bool flag_deleted;

		Product();
		Product(int id, int id_plant, char* name, char*  description);
		void print();
	};


	//get-s
	products::Product getProductByIdAndPlantId(int id, int id_plant, char* PRODUCTS_FILE_NAME,
		char* PLANT_FILE_NAME, plants::PlantIndexTable idx_table);
	//insert-m
	void insertProduct(int id, int id_plant, char* name, char* description,
		char* FILE_NAME, char* PLANT_FILE_NAME, char* GARBAGE_FILE_NAME, plants::PlantIndexTable& idx_table);
	//update-s
	void updateProductById(int id, char* name, char* description, char* FILE_NAME, plants::PlantIndexTable& idx_table);
	//del-s
	void delProductById(int id, int id_plant, char* FILE_NAME, char* PLANT_FILE_NAME, char* GARBAGE_FILE, plants::PlantIndexTable& idx_table);

	void printProductByIdAndPlantId(int id, int id_plant, char* PRODUCTS_FILE_NAME,
		char* PLANT_FILE_NAME, plants::PlantIndexTable idx_table);
	void printProductByTheSamePlant(int id_plant, char* PRODUCTS_FILE_NAME,
		char* PLANT_FILE_NAME, plants::PlantIndexTable idx_table);
	void printAllProducts(char* PRODUCTS_FILE_NAME);
}