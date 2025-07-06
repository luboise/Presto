export module presto.types.entity;

import std;

import presto.types.core;

export namespace Pr {

using entity_id_t = PR_NUMERIC_ID;
using entity_name_t = Pr::string;

constexpr size_t MAX_TAG_COUNT = 20;
using entity_tag_id_t = Pr::int8_t;
using entity_tag_name_t = Pr::string;
using entity_tag_map = std::array<bool, MAX_TAG_COUNT>;

constexpr entity_tag_id_t INVALID_TAG_ID = -1;

}  // namespace Pr
