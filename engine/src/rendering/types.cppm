export module presto.internal.rendering:types;

export namespace Presto {

struct CanvasDrawDetails {
    mesh_registration_id_t mesh;
    TransformData transform;
};

struct MeshRegistrationData {
    // The ID of the mesh's place in the RenderingManager
    mesh_registration_id_t render_manager_id;
    Allocated<Buffer> vertices;
    Allocated<Buffer> indices;

    Presto::size_t index_draw_count{0};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    // The ID that points to the internal details in the renderer
    mesh_context_id_t context_id{-1U};
};

struct MeshData {
    pipeline_id_t pipeline_id{PR_PIPELINE_ANY};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    AnyVertexList vertices;
    IndexList indices;
    // [[nodiscard]] BoundingBox getBoundingBox() const;
    //

    void setVertices(const ImportedAttributeList& attributes);
};

struct MeshDraw {
    const MeshPtr mesh;
    const MaterialPtr material;
};

}  // namespace Presto
