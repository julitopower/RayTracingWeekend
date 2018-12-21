#ifndef RENDER_HPP
#define RENDER_HPP

#include <string>

namespace rt {

class Hitable;
class Camera;
    
void render(uint16_t width,
	    uint16_t height,
	    const Hitable& world,
	    const Camera& cam,
	    uint anti_alias,
	    const std::string& filepath);
    
} // namespace rt

#endif // RENDER_HPP
