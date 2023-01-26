#include "../renderer/Shaders.h"

class MaterialInterface {
public:
    static Shaders CompileShaders(std::string vertex_path, std::string fragment_path);
};

