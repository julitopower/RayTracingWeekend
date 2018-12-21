#include <cstdio>
#include <fstream>
#include <random>

#include <camera.hpp>
#include <image.hpp>
#include <ray.hpp>
#include <render.hpp>
#include <vector.hpp>

#include <png.h>

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

void PNGWriter::write(const std::vector<uint8_t>& buffer, std::size_t width, std::size_t height) const {
    // Initialize write machinery, this includes a pointer to the png write context
    // and a pointer to the image information
    png_structp png_ptr = nullptr;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    png_infop png_info_ptr = nullptr;
    png_info_ptr = png_create_info_struct(png_ptr);


    // Open a file for binary writing
    FILE* fp = nullptr;
    fp = fopen(filepath_.c_str(), "wb");

    // Initialize IO
    png_init_io(png_ptr, fp);

    // Populate info data structure
    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, png_info_ptr, width, height,
		 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    // Actually write the header
    png_write_info(png_ptr, png_info_ptr);

    // Allocate memory for 3 bytes per pixel. We write the output one
    // row at a time
    auto row = std::make_unique<png_byte[]>(3 * width);
    for (int i = height - 1 ; i >= 0 ; --i) {
	for (auto j = 0U ; j < width ; ++j) {
	    row[j * 3 + 0] = +buffer[i * width * 3 + j * 3];
	    row[j * 3 + 1] = +buffer[i * width * 3 + j * 3 + 1];
	    row[j * 3 + 2] = +buffer[i * width * 3 + j * 3 + 2];
	}
	png_write_row(png_ptr, row.get());
    }

    // Write the footer (if necessary) and flush
    png_write_end(png_ptr, png_info_ptr);
    png_write_flush(png_ptr);

    // Cleanup
    // See: http://www.ludism.org/~rwhe/LSB/png/libpng-png-free-data-1.html
    png_free_data(png_ptr, png_info_ptr, PNG_FREE_ALL, -1);
    // See: http://refspecs.linuxbase.org/LSB_3.1.0/LSB-Desktop-generic/LSB-Desktop-generic/libpng12.png.destroy.write.struct.1.html
    png_destroy_write_struct(&png_ptr, &png_info_ptr);
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

	    const std::size_t base_idx = i * width * 3 + j * 3;
	    img[base_idx] = static_cast<uint8_t>(color.x() * CONV);
	    img[base_idx + 1] = static_cast<uint8_t>(color.y() * CONV);
	    img[base_idx + 2] = static_cast<uint8_t>(color.z() * CONV);
	}
    }

    PNGWriter{filepath}.write(img, width, height);
}
} // namespace rt
