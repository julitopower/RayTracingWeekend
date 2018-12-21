#include <fstream>
#include <random>

#include <camera.hpp>
#include <image.hpp>
#include <ray.hpp>
#include <render.hpp>
#include <vector.hpp>

#ifdef USE_OMP
#include <omp.h>
#endif

namespace rt {

void PPMWriter::write(const std::vector<uint8_t>& buffer, std::size_t width, std::size_t height) const {
    std::ofstream os{filepath_};
    os << "P3" << std::endl << width << " " << height << std::endl <<  "255" << std::endl;
    
    for (std::int32_t i = height - 1 ; i >= 0 ; --i) {
	for (auto j = 0U; j < width; ++j) {
	    os << +buffer[i * width * 3 + j * 3] << " "
	       << +buffer[i * width * 3 + j * 3 + 1] << " "
	       << +buffer[i * width * 3 + j * 3 + 2] << " ";
	}
	os << std::endl;
    }    
}
    
void render(uint16_t width,
            uint16_t height,
            const Hitable& world,
            const Camera& cam,
            uint anti_alias,
            const std::string& filepath) {
    const auto CONV = 255.99f;

    auto dis = std::uniform_real_distribution<>{0.0, 1.0};
    std::random_device device;

    std::vector<uint8_t> img(width * height * 3);
    for (auto i = height - 1 ; i >= 0 ; --i) {
        #pragma omp parallel for
	for (auto j = 0U; j < width; ++j) {
	    rt::Vector3f color{0, 0, 0};
	    for (auto a = 0U ; a < anti_alias ; ++a) {

		auto u = static_cast<float>(j + dis(device)) / width;
		auto v = static_cast<float>(i + dis(device)) / height;

		auto r = cam.ray(u,v);
		color += rt::ray_color(r, world, 0);
	    }
	    color /= static_cast<float>(anti_alias);
	    color.sqrt();

	    img[i * width * 3 + j * 3] = static_cast<uint8_t>(color.x() * CONV);
	    img[i * width * 3 + j * 3 + 1] = static_cast<uint8_t>(color.y() * CONV);
	    img[i * width * 3 + j * 3 + 2] = static_cast<uint8_t>(color.z() * CONV);
	}
    }

    PPMWriter{filepath}.write(img, width, height);
}
} // namespace rt
