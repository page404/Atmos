#include <core/image/a3ImageDecoder.h>
#include <core/log/a3Log.h>

#ifdef A3_IMAGE_LIB_PNGPP
#include <png.hpp>

#elif defined A3_IMAGE_LIB_LODEPNG
#include <lodepng.h>
#endif

class a3ImageDecoder::a3Decoder
{
public:
    void load(const std::string& filePath)
    {
        unsigned error = lodepng::decode(image, width, height, filePath);

        if(error)
            a3Log::error("a3Decoder error %d: %s\n", error, lodepng_error_text(error));
        else
        {
            a3Log::success("Image %s load succeed\n", filePath.c_str());
            a3Log::success("width:%d, height:%d\n", width, height);
        }
    }

    t3Vector3f getColor(int x, int y)
    {
        if(x < 0 || x > width || y < 0 || y > height)
        {
            a3Log::warning("a3Decoder error coordinate: x:%d, y:%d\n", x, y);
            return t3Vector3f::zero();
        }

        // ����alphaֵ
        return t3Vector3f(image[(x + y * width) * 4 + 0], image[(x + y * width) * 4 + 1], image[(x + y * width) * 4 + 2]);
    }

    void print()
    {
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                a3Log::print("[%d]", i * 10 + j);
                a3Log::print("r: %d, ", image[i * 10 + j + 0]);
                a3Log::print("g: %d, ", image[i * 10 + j + 1]);
                a3Log::print("b: %d, ", image[i * 10 + j + 2]);
                a3Log::print("a: %d\n", image[i * 10 + j + 3]);
            }
        }
    }

#ifdef A3_IMAGE_LIB_PNGPP
    png::image< png::rgb_pixel >* im;
#else
    std::vector<unsigned char> image;
#endif

    unsigned width, height;
};

a3ImageDecoder::a3ImageDecoder()
{
    decoder = new a3Decoder();
}

a3ImageDecoder::a3ImageDecoder(const std::string& filePath)
{
    decoder = new a3Decoder();
    load(filePath);
}

a3ImageDecoder::~a3ImageDecoder()
{
    delete decoder;
}

void a3ImageDecoder::load(const std::string& filePath)
{
    decoder->load(filePath);
}

t3Vector3f a3ImageDecoder::getColor(int x, int y)
{
    return decoder->getColor(x, y);
}

t3Vector3f a3ImageDecoder::lookup(float u, float v)
{
    return decoder->getColor(u * (decoder->width - 1), v * (decoder->height - 1));
}

void a3ImageDecoder::print()
{
    decoder->print();
}