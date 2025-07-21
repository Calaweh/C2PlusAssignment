#pragma once
#include "../json/single_include/nlohmann/json.hpp"
#include <fstream>
#include <iomanip>
#include "../universalFunction/globalFile.hpp"
#include "to_from_json.hpp"

using json = nlohmann::json;

class JsonDatabase
{
private:
    std::string dbPath;

    json loadDatabase()
    {
        std::ifstream dbFile(dbPath);
        if (!dbFile.is_open())
        {
            return emptyDbArray();
        }

        json db;
        dbFile >> db;
        return db;
    }

    json emptyDbArray()
    {
        return json{
            {EVENTS, json::array()},
            {BOOKINGS, json::array()},
            {STALLS, json::array()},
            {CUSTOMERS, json::array()},
            {VENDORS, json::array()},
            {STAFFS, json::array()},
            {ITEMS, json::array()},
            {ORDERS, json::array()},
            {DELIVERIES, json::array()},
            {ADDRESSES, json::array()},
            {IDENTITIES, json::array()},
            {CATEGORIES, json::array()},
            {SPECIFICATIONS, json::array()},
            {MONITORING_RECORDS, json::array()},
            {REPORTS, json::array()},
            {MARKETING_CAMPAIGNS, json::array()},
            {ORDERLISTS, json::array()}};
    }

    void saveDatabase(const json &DATA)
    {
        std::ofstream dbFile(dbPath);
        if (!dbFile.is_open())
        {
            throw std::runtime_error("Could not open database file for writing.");
        }

        dbFile << DATA.dump(4);
    }

public:
    // Constructor
    JsonDatabase(const std::string &PATH = DATABASE_FILE_PATH) : dbPath(PATH) {}

    template <typename T>
    inline void createEntity(const std::string &collectionName, const T &ENTITY)
    {
        json db = loadDatabase();
        json entityJson;
        to_json(ENTITY, entityJson);
        db[collectionName].push_back(entityJson);
        saveDatabase(db);
    }

    template <typename T>
    inline std::vector<T> readEntities(const std::string &collectionName)
    {
        json db = loadDatabase();
        std::vector<T> entities;

        if (db.contains(collectionName))
        {
            for (const auto &item : db[collectionName])
            {
                T entity = item.get<T>();
                entities.push_back(entity);
            }
        }

        return entities;
    }

    template <typename T>
    inline void updateEntity(const std::string &collectionName, const T &ENTITY)
    {
        json db = loadDatabase();
        json entityJson;
        to_json(ENTITY, entityJson);

        for (auto &item : db[collectionName])
        {
            if (item[PRIMARY_KEY_ID] == entityJson[PRIMARY_KEY_ID]) 
            {
                item = entityJson;
                break;
            }
        }

        saveDatabase(db);
    }

    template <typename T>
    inline void deleteEntity(const std::string &collectionName, const std::string &Id)
    {
        json db = loadDatabase();
        auto &collection = db[collectionName];

        for (auto it = collection.begin(); it != collection.end();)
        {
            if ((*it)[PRIMARY_KEY_ID] == Id)
                it = collection.erase(it);
            else
                ++it;
        }

        saveDatabase(db);
    }
};