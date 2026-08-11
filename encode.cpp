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