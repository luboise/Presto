namespace Presto {
    class ModelComponent {
       public:
        glm::vec3 getProjected(glm::vec3 yawPitchRoll) const;
        glm::vec3 getProjected(glm::vec3 yawPitchRoll,
                               glm::vec3 cameraPos) const;
        glm::vec3 getProjected(glm::vec3 yawPitchRoll, glm::vec3 cameraPos,
                               glm::float32 scale) const;

        static glm::mat4 getModelViewMatrix(glm::vec3 offset,
                                            glm::vec3 yawPitchRoll,
                                            glm::float32 scale);
        static glm::mat4 getModelViewMatrix(glm::vec3 offset,
                                            glm::float32 scale);
        static glm::mat4 getProjectionMatrix(glm::float32 fovRad,
                                             VkExtent2D extents);

        void setScale(glm::vec3 scale);

       private:
        glm::vec3 _scale;
        glm::vec3 _translation;
        glm::vec3 _
    };
}  // namespace Presto