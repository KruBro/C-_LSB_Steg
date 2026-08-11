#ifndef ENCODE_H
#define ENCODE_H

#include <string>
#include <fstream>
#include <expected>
#include <span>
#include <array>
#include <cstdint>
#include "types.h"

class EncodeInfo {
private:
    std::string src_image_fname;
    std::ifstream fptr_src_image;
    size_t image_capacity;

    std::string secret_fname;
    std::ifstream fptr_secret;
    std::string extn_secret_file;
    size_t size_secret_file;

    std::string stego_image_fname;
    std::ofstream fptr_stego_image;

    EncodeInfo(std::string src, std::string secret, std::string ext, std::string stego) :
    src_image_fname(src), fptr_src_image(src), image_capacity(0), secret_fname(secret), 
    fptr_secret(secret), extn_secret_file(ext), size_secret_file(0),  
    stego_image_fname(stego), fptr_stego_image(stego)
    {
        ;
    }

public:
    static std::expected <EncodeInfo, Status> create(char *argv[]);
};

namespace lsb{
    constexpr std :: size_t bits_per_byte = 8;
    void encode_byte_to_lsb(char data, std :: span<char, bits_per_byte> image_buffer);
    std :: expected <void, Status> encode_data_to_image(std :: span<char> data, std :: ifstream& fptr_src_image, std :: ofstream& fptr_stego_image);
    std :: expected <void, Status> encode_size_to_lsb(int size, std :: ifstream& fptr_src_image, std :: ofstream& fptr_stego_image);
}

namespace bmp{
    constexpr std :: size_t header_size = 54;
    std :: size_t get_image_size_for_bmp(std :: ifstream& fptr_src_image);
    std::size_t get_file_size(std::ifstream& fptr);
    std :: expected <void, Status> copy_bmp_header(std :: ifstream& fptr_src_image, std :: ofstream& fptr_stego_image);
    std :: expected <void, Status> copy_remaining_img_data(std :: ifstream& fptr_src_image, std :: ofstream& fptr_stego_image);
}


#endif