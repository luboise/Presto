#include <memory>

#include "Masterpiece.h"
#include "Presto/Objects/Scene.h"

using Presto::Scene;

class MasterpieceManager {
   public:
    static void Init();
    static void Shutdown();

    inline static MasterpieceManager& get() { return *instance_; };

    void newMasterpiece();

    bool openMasterpiece(const fs::path& projectFilepath);

    void clearMasterpiece();

    void loadScene(const fs::path& scenePath);

    [[nodiscard]] Presto::Scene* getMainScene() const;

   private:
    static std::vector<fs::path> getMasterpieceFilesInDirectory(
        const fs::path& directory);

    Masterpiece currentMasterpiece_{};
    std::map<Presto::scene_name_t, Scene*> sceneMap_;

    inline static std::unique_ptr<MasterpieceManager> instance_;
};
