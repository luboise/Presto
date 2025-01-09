#include <memory>

#include "Presto/Objects/Scene.h"
#include "Project.h"

using namespace Presto;

class MasterpieceManager {
   public:
    static void Init();
    static void Shutdown();

    inline static MasterpieceManager& get() { return *instance_; };

    void newMasterpiece();

    void openMasterpiece(const fs::path& projectFilepath);

    void loadScene(const fs::path& scene);

   private:
    static std::vector<fs::path> getMasterpieceFilesInDirectory(
        const fs::path& directory);

    Project currentProject_{};
    std::map<scene_id_t, std::unique_ptr<Scene>> sceneMap_;

    static std::unique_ptr<MasterpieceManager> instance_;
};
