#include "engine/ecs/entity.h"
#include <iostream>

EntityID EntityManager::Create() {
    return s_entities.Create();
}
void EntityManager::Destroy(EntityID id) {
    auto entity = s_entities.Get(id);
    // remove each component 
    for (auto& cid : entity->component_ids)
        s_remove_components[cid.first](cid.second);

    s_entities.Remove(id);
}
bool EntityManager::Exists(EntityID id) {
    return s_entities.TryGet(id) != NULL;
}

bool EntityManager::Find(Entity& entity, ComponentTypeID type_id, ComponentID& handle) {
    for (auto& cid : entity.component_ids) {
        if (cid.first == type_id) {
            handle = cid.second;
            return true;
        }
    }
    return false;
}  
bool EntityManager::Find(Entity& entity, ComponentTypeID type_id) {
    for (auto& cid : entity.component_ids)
        if (cid.first == type_id)
            return true;
    return false;
}  

// initialize static variables
utl::slot_map<Entity> EntityManager::s_entities(MAX_ENTITIES);
std::vector<void (*)(ComponentID)> EntityManager::s_remove_components(MAX_COMPONENTS, NULL);
