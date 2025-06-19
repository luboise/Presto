#pragma once

#include <functional>
#include <map>
#include <ranges>

#include "Presto/Core/Concepts.h"

namespace Presto {

using namespace std::ranges;
using namespace std::views;

template <typename V>
using Filter = std::function<bool(V&)>;

/** A template for results from filtering a map. Used when querying for
 * entities and components to provide a non-owning view.
 */
template <MapLike M>
using MapFilter = Filter<typename M::mapped_type>;

template <typename K, typename V>
using MapFilterResult = std::ranges::filter_view<  //
    std::ranges::values_view<std::map<K, V>>,      //
    Filter<V>>;

template <typename K, typename V>
using FilterView = std::ranges::filter_view<
    std::views::all_t<std::ranges::elements_view<
        std::ranges::ref_view<std::map<K, V, std::less<K>,
                                       std::allocator<std::pair<const K, V>>>>,
        1>>,
    std::function<bool(V&)>>;

template <MapLike M>
using MapFilterView = FilterView<typename M::key_type, typename M::mapped_type>;

}  // namespace Presto
