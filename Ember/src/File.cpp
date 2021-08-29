#include "File.h"
#include <vector>
#include <iostream>

namespace Ember {
    void File::init(const File& file) {
        this->file_path = file.file_path;

        close();
        open();
    }

    bool File::check_path() {
        return (file_path != "");
    }

    File::File(const char* file_path) : file_path(file_path) {
        open();
    }

    File::File() : file_path() { }

    File::File(File& file) {
        init(file);
    }

    void File::operator= (const File& file) {
        init(file);
    }

    File::~File() {
        close();
    }

    void File::empty() {
        close();

        /* Setting it to the trunc mode will clear the file's contents. */
        if (check_path())
            file.open(file_path, std::fstream::out | std::fstream::trunc);

        close();
        open();
    }

    void File::close() {
        file.flush();
        file.close();
    }

    void File::destroy() {
        close();

        remove(file_path.c_str());
    }

    void File::open() {
        if (check_path())
            file = std::fstream(file_path, std::fstream::in | std::fstream::out | std::fstream::app);
    }

    bool File::is_open() {
        return file.is_open();
    }

    bool File::eof() {
        return file.eof();
    }

    std::streampos File::location() {
        return file.tellg();
    }

    void File::reset() {
        file.clear();
        file.seekg(0);
    }

    void File::location(const uint32_t read_location) {
        if (read_location > -1)
            file.seekg(read_location, std::ios::beg);
    }

    std::streamoff File::size() {
        std::streampos beg, end;

        file.seekg(0, std::ios::beg);
        beg = file.tellg();
        file.seekg(0, std::ios::end);
        end = file.tellg();

        return end - beg;
    }

    bool File::is_empty() {
        return (size() == 0) ? true : false;
    }

    const char* File::path() {
        if (check_path())
            return file_path.c_str();
        return "";
    }

    uint64_t File::line_count() {
        reset();
        int num_lines = 0;
        std::string unused;
        while (std::getline(file, unused))
            ++num_lines;

        return num_lines;
    }

    uint64_t File::character_count() {
        reset();

        char active_char;
        unsigned long number_chars = 0;

        while (file.get(active_char))
            if (active_char != '\n')
                number_chars++;

        return number_chars;
    }

    uint64_t File::word_count() {
        reset();

        std::string search_word;
        uint64_t word_counter = 0;
        while (file >> search_word)
            word_counter++;

        return word_counter;
    }

    std::string File::read() {
        reset();

        std::string buffer((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        return buffer;
    }

    std::string File::read_line(const uint32_t line_number) {
        reset();

        std::string line_search;
        uint32_t line_counter = 0;

        while (getline(file, line_search)) {
            if (line_counter == line_number)
                return line_search;
            line_counter++;
        }

        return "";
    }

    std::string File::read_word(const uint32_t location) {
        reset();

        std::string word_search;
        uint32_t word_counter = 0;
        while (file >> word_search) {
            if (word_counter == location)
                return word_search;
            word_counter++;
        }
        return "";
    }

    void File::delete_line(const uint32_t line_number) {
        std::string file_data = read();

        std::string line_search;
        unsigned int line_counter = 0;

        std::vector<size_t> line_sizes;

        while (getline(file, line_search)) {
            line_sizes.push_back(line_search.length());

            if (line_counter == line_number) {
                size_t location = 0;
                for (size_t i = 0; i < line_sizes.size() - 1; i++)
                    location += line_sizes[i];
                file_data.erase(location, line_sizes.back() + 1);
                break;
            }
            line_counter++;
        }

        line_sizes.clear();

        empty();
        file << file_data;
    }

    void File::do_each_word(const std::function<bool(const std::string& word, const uint32_t counter)>& func) {
        std::string word_search;
        uint32_t word_counter = 0;
        while (file >> word_search) {
            if (!func(word_search, word_counter))
                break;
            word_counter++;
        }

        reset();
    }

    void File::do_each_line(const std::function<bool(const std::string& line, const uint32_t counter)>& func) {
        std::string line;
        uint32_t line_counter = 0;
        while (getline(file, line)) {
            if (!func(line, line_counter))
                break;
            line_counter++;
        }

        reset();
    }
}