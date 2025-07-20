#pragma once
#include "..\data\json_database.hpp"
#include "..\entity\entities.hpp"
#include "..\universalFunction\globalFile.hpp"
#include <vector>
#include <unordered_set>

template <typename T>
struct EntitiesManager
{
public:
    JsonDatabase db;
    std::unordered_set<std::shared_ptr<T>> entities;
    std::unordered_set<std::shared_ptr<T>> restrictedEntities;
    std::string collectionName;

    EntitiesManager(const std::string &collectionName, const std::string &dbPath = DATABASE_FILE_PATH)
        : db(dbPath), collectionName(collectionName) {}

    ~EntitiesManager() = default;

    inline void createOrUpdateEntity(T &entity)
    {
        for (const auto &existingEntity : this->entities)
            if (auto lockedEntity = existingEntity.lock())
                if (lockedEntity->Id == entity.Id)
                {
                    updateEntity(entity);
                    return;
                }
        createEntity(entity);
    }

    inline void deleteEntity(T &entity)
    {
        db.deleteEntity(collectionName, entity.Id);
        this->entities.erase(
            std::remove_if(this->entities.begin(), this->entities.end(),
                           [&entity](const std::weak_ptr<T> &e)
                           {
                               auto locked = e.lock();
                               return locked && locked->Id == entity.Id;
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
                                   return e.lock()->Id == entity.Id;
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