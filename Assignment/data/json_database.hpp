#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include "globalFile.hpp"

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
    }

public:
    // Constructor
    JsonDatabase(const std::string &path) : dbPath(path) {}

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
                T entity;
                from_json(item, entity);
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
            if (item[PRIMARY_KEY_ID] == entityJson[PRIMARY_KEY_ID]) // Assuming 'id' is the primary key
            {
                item = entityJson;
                break;
            }
        }

        saveDatabase(db);
    }

    template <typename T>
    inline void deleteEntity(const std::string &collectionName, const std::string &ID)
    {
        json db = loadDatabase();
        auto &collection = db[collectionName];

        for (auto it = collection.begin(); it != collection.end();)
        {
            if ((*it)[PRIMARY_KEY_ID] == ID)
                it = collection.erase(it);
            else
                ++it;
        }

        saveDatabase(db);
    }
};