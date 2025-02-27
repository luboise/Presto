#pragma once
namespace Presto {
class Buffer;

struct MeshRegistrationData {
    mesh_id_t id;
    Allocated<Buffer> vertices;
    Allocated<Buffer> indices;
};

}  // namespace Presto
