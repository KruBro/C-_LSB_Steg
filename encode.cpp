#include "encode.h"

std::expected<EncodeInfo, Status> EncodeInfo::create(char *argv[])
{
    // Guard 1: source image must exist and contain ".bmp"
    if (argv[2] == NULL || std::string(argv[2]).find(".bmp") == std::string::npos)
    {
        return std::unexpected(Status::e_failure);
    }
    std::string src = std::string(argv[2]);

    // Guard 2: secret file must exist and contain ".txt"
    if (argv[3] == NULL || std::string(argv[3]).find(".txt") == std::string::npos)
    {
        return std::unexpected(Status::e_failure);
    }
    std::string secret = std::string(argv[3]);

    // Extension is hardcoded ".txt" in the original C too — same here
    std::string ext = ".txt";

    // stego filename: use argv[4] if present, else default
    std::string stego = (argv[4] != NULL) ? std::string(argv[4]) : std::string("stego.bmp");


    return EncodeInfo(src, secret, ext, stego);
}

void lsb :: encode_byte_to_lsb(char data, std :: span<char, bits_per_byte> image_buffer)
{
    for(std :: size_t i = 0; i < bits_per_byte; i++)
    {
        image_buffer[i] = ((image_buffer[i] & 0xFE) | ((data >> i) & 1));
    }
}

std :: expected <void, Status> lsb :: encode_data_to_image(std :: span<char> data, std :: ifstream& fptr_src_image, std :: ofstream& fptr_stego_image)
{
    std :: array <char, lsb :: bits_per_byte> image_buffer;
    for(std :: size_t i = 0; i < data.size(); i++)
    {
        fptr_src_image.read(image_buffer.data(), lsb :: bits_per_byte);
        if(!fptr_src_image)
        {
            return std :: unexpected(Status :: e_failure);
        }

        encode_byte_to_lsb(data[i], std :: span <char, bits_per_byte> (image_buffer));

        fptr_stego_image.write(image_buffer.data(), lsb :: bits_per_byte);

        if(!fptr_stego_image)
        {
            return std :: unexpected(Status :: e_failure);
        }
    }

    return {};
}

std :: expected <void, Status> lsb :: encode_size_to_lsb(int size, std :: ifstream& fptr_src_image, std :: ofstream& fptr_stego_image)
{
    std :: array <char, 32> buffer;

    fptr_src_image.read(buffer.data(), 32);

    if(!fptr_src_image)
    {
        return std :: unexpected(Status :: e_failure);
    }

    for(std :: size_t i = 0; i < 32; i++)
    {
        buffer[i] = ((buffer[i] & 0xFE) | ((size >> i) & 1));
    }

    fptr_stego_image.write(buffer.data(), 32);

    if(!fptr_stego_image)
    {
        return std :: unexpected(Status :: e_failure);
    }

    return {};
}

std :: size_t bmp::get_image_size_for_bmp(std :: ifstream& fptr_src_image)
{
    uint32_t width, height;
    fptr_src_image.seekg(18);
    fptr_src_image.read(reinterpret_cast<char*> (&width), sizeof(uint32_t));
    fptr_src_image.read(reinterpret_cast<char*> (&height), sizeof(uint32_t));

    return static_cast<size_t>(width) * height * 3;
}

std::size_t bmp::get_file_size(std::ifstream& fptr)
{
    fptr.seekg(0, std::ios::end);
    return static_cast<std :: size_t>(fptr.tellg());
}

std :: expected <void, Status> bmp :: copy_bmp_header(std :: ifstream& fptr_src_image, std :: ofstream& fptr_stego_image)
{
    fptr_src_image.seekg(0);
    std :: array<char, header_size> header;

    fptr_src_image.read(header.data(), header_size);
    if(!fptr_src_image)
    {
        return std :: unexpected(Status :: e_failure);
    }

    fptr_stego_image.write(header.data(), header_size);
    if(!fptr_stego_image)
    {
        return std :: unexpected(Status :: e_failure);
    }

    return {};
}

std :: expected <void, Status> bmp :: copy_remaining_img_data(std :: ifstream& fptr_src_image, std :: ofstream& fptr_stego_image)
{
    fptr_stego_image << fptr_src_image.rdbuf();

    if(!fptr_stego_image)
    {
        return std :: unexpected(Status :: e_failure);
    }

    return {};
}