#pragma once
#include <string>
#include <iostream>

namespace plants
{
	struct PlantIndex
	{
		int id;
		int position;
		bool flag_deleted;
		PlantIndex();
		PlantIndex(int id);
	};

	struct PlantIndexTable
	{
		PlantIndex idx_cells[20];
		int cur_pos;
		PlantIndexTable();
		void sortById();
		void posNext(int pos);
		bool add(int id);
		int findFirstDeletedPlantIdx();
		PlantIndex getPositionOfPlantById(int id);
		int getIndexPositionOfPlantById(int id);
	};

	struct Plant
	{
		int id;
		char name[20];
		char city[20];
		int number_of_workers;
		int number_of_products;
		int product_beg;
		 
		bool flag_deleted;


		Plant();
		Plant(int id, char* name, char* city, int number_of_workers);
		void setDeleted();
		void printinfo();

	};


	void merge(PlantIndex array[], int left, int mid, int right);
	void mergeSort(PlantIndex array[], int begin, int end);

	//get-m
	Plant getPlantById(int id, char* FILE_NAME, PlantIndexTable idx_table);
	//insert-m
	void insertPlant(int id, char* name, int number_of_workers, char* city,
		char* FILE_NAME, char* GARBAGE_FILE_NAME, PlantIndexTable& idx_table);
	//update-m
	void updatePlant(int id, int number_of_workers,
		char* FILE_NAME, PlantIndexTable& idx_table);
	//del-m
	void deletePlantById(int id, char* FILE_NAME, char* PRODUCTS_FILE_NAME, char* GARBAGE_FILE_NAME, PlantIndexTable& idx_table);

	void printPlantById(int ID, char* FILE_NAME, PlantIndexTable idx_table);
	void printAllPlants(char* FILE_NAME);
}