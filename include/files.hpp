#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <vector>
#include <cereal/cereal.hpp>

class File {
public:
    std::string name;
    std::streamsize size;
    std::vector<char> payload;

    // template <class Archive>
    // void serialize(Archive &ar) {
    //     ar(
    //         cereal::make_nvp("name", name),
    //         cereal::make_nvp("size", size),
    //         cereal::make_nvp("payload", payload)
    //     );
    // }
};

void send_file(int sockfd, const std::string path);
void recv_file(int sockfd, const std::string dir);

#endif