module presto.internal.importing;

Pr::size_t ImportedVertexAttribute::dataSize() const {
    return ShaderTypeSize(type);
};

bool Pr::DefaultAttributeName::IsDefaultAttributeName(const Pr::string& str) {
    return str == POSITION || str == COLOUR || str == NORMAL ||
           str == TEXCOORDS;
}
