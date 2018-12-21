#ifndef IMAGE_HPP
#define IMAGE_HPP
namespace rt {
    class ImageWriter {
    public:
	virtual void write(const std::vector<uint8_t>& buffer,
			   std::size_t width,
			   std::size_t height) const = 0;
    };

    class FileWriter {
    public:
	FileWriter(const std::string& filepath) : filepath_{filepath} {}
	virtual void write(const std::vector<uint8_t>& buffer,
			   std::size_t width,
			   std::size_t height) const = 0;
    protected:
	const std::string& filepath_;
    };
    
    class PPMWriter : public FileWriter {
    public:
	PPMWriter(const std::string& filepath) : FileWriter{filepath} {}
	void write(const std::vector<uint8_t>& buffer,
		   std::size_t width,
		   std::size_t height) const override;
    };

    class PNGWriter : public FileWriter {
    public:
	PNGWriter(const std::string& filepath) : FileWriter{filepath} {}	
	void write(const std::vector<uint8_t>& buffer,
		   std::size_t width,
		   std::size_t height) const override;
    };	
}
#endif // IMAGE_HPP
