#include <files.hpp>

#include <fstream>
#include <vector>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
// #include <cereal/archives/json.hpp>

std::string get_filename(const std::string path);

/**
 * Sends a file to the given socket.
 * First sends the file metadata, then the file contents.
 */
void send_file(int sockfd, const std::string path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open file '" + path + "'");
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    File f;
    f.name = get_filename(path);
    f.size = size;
    std::vector<char> payload(size);

    if (!file.read(payload.data(), size)) {
        throw std::runtime_error("Failed to read file '" + path + "'");
    }

    // Serialize file
    // std::ostringstream archive_stream;
    // {
    //     cereal::JSONOutputArchive archive_out(archive_stream);
    //     archive_out(f);
    // }

    // std::string archive_json = archive_stream.str();
    // std::size_t length = archive_json.length();

    // if (write(sockfd, &length, sizeof(std::size_t)) == -1) {
    //     throw std::runtime_error("Failed to send file object length");
    // }

    // ssize_t bsent = write(sockfd, archive_json.data(), length);
    // if (bsent == -1) {
    //     throw std::runtime_error("Failed to send serialized file and metadata");
    // }

    if (write(sockfd, (void*)&f, sizeof(File)) < 0) {
        throw std::runtime_error("Failed to send file metadata");
    }

    ssize_t bsent = 0;
    while (bsent < f.size) {
        ssize_t b = write(sockfd, payload.data() + bsent, f.size - bsent);
        if (b < 0) {
            throw std::runtime_error("Failed to send file contents");
        }
        bsent += b;
    }

    // if ((std::size_t) bsent < length) {
    //     throw std::runtime_error("Sent less bytes than expected (" + std::to_string(bsent) + "/" + std::to_string(length) + ")");
    // }
} 

/**
 * Receives a file from the given socket.
 * First receives the file metadata, then the file contents.
 * Saves file to the given directory.
 */
void recv_file(int sockfd, const std::string dir) {
    // std::size_t length;
    // if (read(sockfd, &length, sizeof(std::size_t)) == -1) {
    //     throw std::runtime_error("Failed to receive file object length");
    // }

    File f;

    if (read(sockfd, (void*)&f, sizeof(File)) < 0) {
        throw std::runtime_error("Failed to read file metadata");
    }

    std::vector<char> payload(f.size);

    ssize_t bread = 0;
    while (bread < f.size) {
        ssize_t b = read(sockfd, payload.data() + bread, f.size - bread);
        if (b < 0) {
            throw std::runtime_error("Failed to read file contents");
        }
        bread += b;
    }

    // std::string archive_json(length, '\0');
    // unsigned long bread = 0;
    // while (bread < length) {
    //     int b = read(sockfd, &archive_json[bread], length - bread);
    //     if (b == -1) {
    //         throw std::runtime_error("Failed to receive serialized file and metadata");
    //     }
    //     bread += b;
    // }
    

    // Deserialize file
    // std::istringstream archive_stream(archive_json);
    // File f;
    // {
    //     cereal::JSONInputArchive archive_in(archive_stream);
    //     archive_in(f);
    // }

    std::ofstream file(dir + f.name, std::ios::binary | std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Cannot create file '" + f.name + "'");
    }

    if (!file.write(payload.data(), f.size)) {
        throw std::runtime_error("Failed to write file '" + f.name + "'");
    }
}

std::string get_filename(const std::string path) {
    return path.substr(path.find_last_of("/\\") + 1);
    // char* filename = (char*) calloc(1, 256);
    // int i = strlen(path) - 1;
    // while (i >= 0 && path[i] != '/') {
    //     i--;
    // }
    // strcpy(filename, path + i + 1);
    // return filename;
}