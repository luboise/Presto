module presto.internal.managers.entity_manager_impl;

import presto.objects;
import presto.objects.components;

import presto.types;

import presto.internal;

import presto.core;

namespace Pr {

// TODO: Implement this to clean up dangling entities/components
void EntityManagerImpl::collectGarbage() {};

/*
    MapFilterView<EntityManagerImpl::ComponentMap>
    EntityManagerImpl::findComponentsByType(CheckedComponentBits bits) {
        return components_ | std::views::values |
               std::views::filter([](auto& component) {
                   return component->hasBits(bits);
               })

};
                           */

}  // namespace Pr
