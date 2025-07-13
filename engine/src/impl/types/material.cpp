module presto.types.material;

namespace Pr {

[[nodiscard]] Pr::size_t UniformBlock::size() const {
    return std::accumulate(
        this->bindings.begin(), this->bindings.end(), Pr::size_t{0},
        [](auto sum, const auto& binding) { return sum + binding.size(); });
};

[[nodiscard]] Pr::size_t UniformBinding::size() const {
    return SizeOfType(this->data_type);
};

}  // namespace Pr
