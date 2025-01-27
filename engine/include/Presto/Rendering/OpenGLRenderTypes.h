namespace Presto {
    struct GlobalUniforms {
        Presto::mat4 view;
        Presto::mat4 projection;
    };

    struct ObjectUniforms {
        Presto::mat4 transform;
    };
}  // namespace Presto
