#ifndef TEXTURE_HXX__
#define TEXTURE_HXX__

#include <string>

namespace cubes {

class Texture
{
public:
    Texture(const std::string& filename, const std::string& type);

    uint32_t id() const { return texId_; }
    const std::string& type() const { return type_; }

private:
    uint32_t texId_;
    std::string type_;
};

}  // namespace cubes

#endif  // #ifndef TEXTURE_HXX__
