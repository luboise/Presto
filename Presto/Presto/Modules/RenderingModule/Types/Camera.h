namespace Presto {
    class PRESTO_API Camera {
       public:
        glm::mat4 getViewMatrix() const;

        void setYaw(double newYaw);
        void setPitch(double newPitch);
        void setRoll(double newRoll);
        void setPos(glm::vec3 newPos);

        double getYaw() const;
        double getPitch() const;
        double getRoll() const;
        glm::vec3 getPos() const;

       private:
        glm::vec3 _cameraPos;

        void recalculate();

        double _yaw;
        double _pitch;
        double _roll;

        glm::highp_mat4 _viewMatrix;
    };
}  // namespace Presto