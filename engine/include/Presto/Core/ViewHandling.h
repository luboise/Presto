#pragma once

#include <map>
#include <ranges>
#include "Presto/Objects/Component.h"

namespace Presto {
    using namespace std::ranges;
    using namespace std::views;

    template <typename V>
    using Filter = std::function<bool(V &)>;

    /** A template for results from filtering a map. Used when querying for
     * entities and components to provide a non-owning view.
     *
     */

    template <typename M>
    concept MapLike = requires(M m) {
        { m.begin() } -> std::input_iterator;
        { m.end() } -> std::input_iterator;
        typename M::key_type;
        typename M::mapped_type;
    };

    template <MapLike M>
    using MapFilter = Filter<typename M::mapped_type>;

    template <typename K, typename V>
    using MapFilterResult = std::ranges::filter_view<  //
        std::ranges::values_view<std::map<K, V>>,      //
        Filter<V>>;

    template <typename K, typename V>
    using FilterView = std::ranges::filter_view<
        std::views::all_t<std::ranges::elements_view<
            std::ranges::ref_view<std::map<
                K, V, std::less<K>, std::allocator<std::pair<const K, V>>>>,
            1>>,
        std::function<bool(V &)>>;

    template <MapLike M>
    using MapFilterView =
        FilterView<typename M::key_type, typename M::mapped_type>;

    using ComponentFilter = std::function<bool(const GenericComponentPtr &)>;

    using ComponentList = std::vector<GenericComponentPtr>;
    using ComponentDatabase = std::map<class_id_t, ComponentList>;

    using ComponentSearchResults = filter_view<
        all_t<join_view<elements_view<ref_view<ComponentDatabase>, 1>>>,
        ComponentFilter>;
}  // namespace Presto
