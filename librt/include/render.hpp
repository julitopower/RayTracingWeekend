#ifndef RENDER_HPP
#define RENDER_HPP

#include <string>

namespace rt {

class Hitable;
class Camera;
    
void render(std::uint16_t width,
	    std::uint16_t height,
	    const Hitable& world,
	    const Camera& cam,
	    std::uint16_t anti_alias,
	    const std::string& filepath,
	    bool background = false);
    
} // namespace rt

#endif // RENDER_HPP
