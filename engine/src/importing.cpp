module presto.internal.importing;

Presto::size_t ImportedVertexAttribute::dataSize() const {
    return ShaderTypeSize(type);
};

bool Presto::DefaultAttributeName::IsDefaultAttributeName(
    const Presto::string& str) {
    return str == POSITION || str == COLOUR || str == NORMAL ||
           str == TEXCOORDS;
}
