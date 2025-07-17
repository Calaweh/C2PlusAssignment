#pragma once
#include "data\json_database.hpp"
#include "entity\entities.hpp"
#include "globalFile.hpp"
#include <vector>
#include <unordered_set>

template <typename T>
class EntitiesManager
{
public:
    JsonDatabase db;
    std::unordered_set<std::weak_ptr<T>> entities;
    std::unordered_set<std::weak_ptr<T>> restrictedEntities;
    std::string collectionName;

    EntitiesManager(const std::string &dbPath = DATABASE_FILE_PATH, const std::string &collectionName)
        : db(dbPath), collectionName(collectionName) {}

    ~EntitiesManager() = default;

    inline void createOrUpdateEntity(T &entity)
    {
        for (const auto &existingEntity : this->entities)
            if (auto lockedEntity = existingEntity.lock())
                if (lockedEntity->ID == entity.ID)
                {
                    updateEntity(entity);
                    return;
                }
        createEntity(entity);
    }

    inline void deleteEntity(T &entity)
    {
        db.deleteEntity(collectionName, entity.ID);
        this->entities.erase(
            std::remove_if(this->entities.begin(), this->entities.end(),
                           [&entity](const std::weak_ptr<T> &e)
                           {
                               auto locked = e.lock();
                               return locked && locked->ID == entity.ID;
                           }),
            this->entities.end());
    }

    inline void loadEntities()
    {
        auto entities = db.readEntities<T>(collectionName);
        for (const auto &entity : entities)
        {
            this->entities.insert(std::make_shared<T>(entity));
        }
    }

    inline void updateEntity(T &entity)
    {
        db.updateEntity(collectionName, entity);
        auto it = std::find_if(this->entities.begin(), this->entities.end(),
                               [&entity](const std::weak_ptr<T> &e)
                               {
                                   return e.lock()->ID == entity.ID;
                               });
        if (it != this->entities.end())
            *it = std::make_shared<T>(entity);
    }

    inline void updateEntity(std::vector<T> entities)
    {
        for (const auto &entity : entities)
            updateEntity(entity);
    }

    inline void createEntity(T &entity)
    {
        db.createEntity(collectionName, entity);
        this->entities.insert(std::make_shared<T>(entity));
    }
};