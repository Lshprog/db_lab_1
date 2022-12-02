#include "Plant.h"
#include "Product.h"
#include "GarbageCollector.h"
#include "FilesFunctionality.h"

#include <stdio.h>
#include <fstream>
#include <iostream>




bool menu(char* GARBAGE_COLLECTOR_FILE, char* PLANT_FILE, char* PLANT_INDEX_FILE, char* PRODUCTS_FILE, plants::PlantIndexTable& index_table)
{
    int option;

    std::cout << "Choose the option:\n";
    std::cout << "1 - Add plant (insert-m)\n";
    std::cout << "2 - Update plant (update-m)\n";
    std::cout << "3 - Delete plant (del-m)\n";
    std::cout << "4 - Find plant by ID (get-m)\n";
    std::cout << "5 - Print information about all the plants\n";
    std::cout << "6 - Add product (insert-s)\n";
    std::cout << "7 - Update product (update-s)\n";
    std::cout << "8 - Delete product (del-s)\n";
    std::cout << "9 - Find product by ID and plant ID (get-s)\n";
    std::cout << "10 - Print information about all the products\n";
    std::cout << "11 - Print information about all the products by the same plant\n";
    std::cout << "12 - Clear data from all files\n";
    std::cout << "13 - Clear data from product files\n";

    std::cin >> option;

    //insert-m
    if (option == 1)
    {
        int id,number_of_workers;
        char name[20], city[20];
        std::cout << "Enter the id, name, city, number of workers\n";
        std::cin >> id >> name >> city >> number_of_workers;
        plants::insertPlant(id, name, number_of_workers,city,
            PLANT_FILE, GARBAGE_COLLECTOR_FILE, index_table);
        writeToFileStructure<plants::PlantIndexTable>(index_table, PLANT_INDEX_FILE, 0);
        return true;
    }

    //update-m
    else if (option == 2)
    {
        int id, number_of_workers;
        
        std::cout << "Enter the id, and number of workers of plant\n";
        std::cin >> id >> number_of_workers;
        plants::updatePlant(id, number_of_workers, PLANT_FILE, index_table);
        return true;
    }

    //del-m
    else if (option == 3)
    {
        int id;
        std::cout << "Enter the id of the plant\n";
        std::cin >> id;
        plants::deletePlantById(id, PLANT_FILE, PRODUCTS_FILE, GARBAGE_COLLECTOR_FILE, index_table);
        return true;
    }

    //get-m
    else if (option == 4)
    {
        int id;
        std::cout << "Please, enter ID of the plant :\n";
        std::cin >> id;
        plants::printPlantById(id, PLANT_FILE, index_table);
        return true;
    }

    //Print all lecturers
    else if (option == 5)
    {
        plants::printAllPlants(PLANT_FILE);
        return true;
    }

    //insert-s
    else if (option == 6)
    {
        int id, id_plant;
        char name[20], description[20];

        std::cout << "Enter the id, id_plant, name, description of the product\n";
        std::cin >> id >> id_plant >> name >> description;
        products::insertProduct(id, id_plant, name, description,
            PRODUCTS_FILE, PLANT_FILE, GARBAGE_COLLECTOR_FILE, index_table);
        return true;
    }

    //update-s
    else if (option == 7)
    {
        int id;
        char name[20], description[20];
        std::cout << "Enter the id, new name and description of the product\n";
        std::cin >> id >> name>> description;
        products::updateProductById(id, name, description, PRODUCTS_FILE, index_table);
        return true;
    }

    //del-s
    else if (option == 8)
    {
        int id, lecturer_id;
        std::cout << "Enter the id of the product and plant\n";
        std::cin >> id >> lecturer_id;
        products::delProductById(id, lecturer_id, PRODUCTS_FILE, PLANT_FILE, GARBAGE_COLLECTOR_FILE, index_table);
        return true;
    }

    else if (option == 9)
    {
        int plant_id, product_id;
        std::cout << "Please, enter ID of the plant and the product ID:\n";
        std::cin >> plant_id >> product_id;
        products::printProductByIdAndPlantId(product_id, plant_id,
            PRODUCTS_FILE, PLANT_FILE, index_table);
        return true;
    }

    else if (option == 10)
    {
        products::printAllProducts(PRODUCTS_FILE);
        return true;
    }

    else if (option == 11)
    {
        int plant_id;
        std::cout << "Please, enter the plant ID:\n";
        std::cin >> plant_id;
        products::printProductByTheSamePlant(plant_id, PRODUCTS_FILE, PLANT_FILE, index_table);
        return true;
    }

    else if (option == 12)
    {
        std::ofstream plant;
        plant.open(PLANT_FILE, std::ofstream::out | std::ofstream::trunc);
        plant.close();

        std::ofstream plant_idx;
        plant_idx.open(PLANT_INDEX_FILE, std::ofstream::out | std::ofstream::trunc);
        plant_idx.close();

        std::ofstream prod;
        prod.open(PRODUCTS_FILE, std::ofstream::out | std::ofstream::trunc);
        prod.close();

        std::ofstream garb;
        garb.open(GARBAGE_COLLECTOR_FILE, std::ofstream::out | std::ofstream::trunc);
        garb.close();

        plants::PlantIndexTable temp;
        index_table = temp;

        return true;
    }

    else if (option == 13)
    {
        std::ofstream publ;
        publ.open(PRODUCTS_FILE, std::ofstream::out | std::ofstream::trunc);
        publ.close();

        std::ofstream garb;
        garb.open(GARBAGE_COLLECTOR_FILE, std::ofstream::out | std::ofstream::trunc);
        garb.close();

        plants::PlantIndexTable temp;
        index_table = temp;

        return true;
    }

    else return false;
}


int main()
{
    char GARBAGE_COLLECTOR_FILE[30] = "garbage_collector.dat";
    char PLANT_FILE[30] = "plant.dat";
    char PLANT_INDEX_FILE[30] = "plant.index";
    char PRODUCTS_FILE[30] = "products.dat";

    garbcoll::garbageCollectorInitilialization(GARBAGE_COLLECTOR_FILE);

    plants::PlantIndexTable index_table = readFromFileStructure<plants::PlantIndexTable>(PLANT_INDEX_FILE, -1);

    while (menu(GARBAGE_COLLECTOR_FILE, PLANT_FILE, PLANT_INDEX_FILE, PRODUCTS_FILE, index_table))
    {
        continue;
    }
    return 0;
}