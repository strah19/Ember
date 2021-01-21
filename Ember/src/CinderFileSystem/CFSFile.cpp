#include "CFSFile.h"
#include <vector>

namespace Cinder {
    void CFSFile::InitFromCFSFile(const CFSFile& file) {
        this->file_path = file.file_path;

        CloseFile();
        Open();
    }

    bool CFSFile::CheckPath() {
        return (file_path != "");
    }

    CFSFile::CFSFile(const char* file_path) : file_path(file_path) {
        Open();
    }

    CFSFile::CFSFile() : file_path() { }

    CFSFile::CFSFile(CFSFile& file) {
        InitFromCFSFile(file);
    }

    void CFSFile::operator= (const CFSFile& file) {
        InitFromCFSFile(file);
    }

    CFSFile::~CFSFile() {
        CloseFile();
    }

    void CFSFile::EmptyFile() {
        CloseFile();

        /* Setting it to the trunc mode will clear the file's contents. */
        if (CheckPath())
            file.open(file_path, std::fstream::out | std::fstream::trunc);

        CloseFile();
        Open();
    }

    void CFSFile::CloseFile() {
        file.flush();
        file.close();
    }

    void CFSFile::Destroy() {
        CloseFile();

        remove(file_path.c_str());
    }

    void CFSFile::Open() {
        if (CheckPath())
            file = std::fstream(file_path, std::fstream::in | std::fstream::out | std::fstream::app);
        ASSERT_OPEN;
    }

    bool CFSFile::IsReadLocationToEnd() {
        ASSERT_OPEN;
        return file.eof();
    }

    std::streampos CFSFile::GetCurrentReadLocation() {
        ASSERT_OPEN;
        return file.tellg();
    }

    void CFSFile::ResetReadLocation() {
        ASSERT_OPEN;

        file.clear();
        file.seekg(0);
    }

    void CFSFile::SetReadLocation(const uint32_t read_location) {
        ASSERT_OPEN;
        if(read_location > -1)
            file.seekg(read_location, std::ios::beg);
    }

    std::streamoff CFSFile::GetFileSizeInBytes() {
        ASSERT_OPEN;

        std::streampos beg, end;

        file.seekg(0, std::ios::beg);
        beg = file.tellg();
        file.seekg(0, std::ios::end);
        end = file.tellg();

        return end - beg;
    }

    bool CFSFile::IsFileEmpty() {
        return (GetFileSizeInBytes() == 0) ? true : false;
    }

    const char* CFSFile::GetPath() {
        if (CheckPath())
            return file_path.c_str();
        return "";
    }

    uint64_t CFSFile::GetLineCount() {
        ASSERT_OPEN;
        ResetReadLocation();

        return (uint64_t)(std::count(std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>(), '\n'));
    }

    uint64_t CFSFile::GetCharacterCount() {
        ASSERT_OPEN;
        ResetReadLocation();

        char active_char;
        unsigned long number_chars = 0;

        while (file.get(active_char))
            if (active_char != '\n')
                number_chars++;

        return number_chars;
    }

    uint64_t CFSFile::GetWordCount() {
        ASSERT_OPEN;
        ResetReadLocation();

        std::string search_word;
        uint64_t word_counter = 0;
        while (file >> search_word)
            word_counter++;

        return word_counter;
    }

    std::string CFSFile::ReadFile() {
        ASSERT_OPEN;
        ResetReadLocation();

        std::string buffer((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        return buffer;
    }

    std::string CFSFile::ReadLine(const uint32_t line_number) {
        ASSERT_OPEN;
        ResetReadLocation();

        std::string line_search;
        uint32_t line_counter = 0;

        while (getline(file, line_search)) {
            if (line_counter == line_number)
                return line_search;
            line_counter++;
        }   

        return "";
    }

    std::string CFSFile::ReadWord(const uint32_t location) {
        ASSERT_OPEN;
        ResetReadLocation();

        std::string word_search;
        uint32_t word_counter = 0;
        while (file >> word_search) {
            if (word_counter == location)
                return word_search;
            word_counter++;
        }
        return "";
    }

    uint32_t CFSFile::GetFirstLocationFromWord(const char* word) {
        ASSERT_OPEN;
        ResetReadLocation();

        std::string word_search;
        uint32_t word_counter = 0;
        while (file >> word_search) {
            if (word_search == word)
                return word_counter;

            word_counter++;
        }

        return -1;
    }

    void CFSFile::DeleteLine(const uint32_t line_number) {
        ASSERT_OPEN;
        std::string file_data = ReadFile();

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

        EmptyFile();
        file << file_data;
    }

    void CFSFile::DoEachWord(const std::function<bool(std::string& word, const uint32_t counter)>& func) {
        ASSERT_OPEN;

        std::string word_search;
        uint32_t word_counter = 0;
        while (file >> word_search) {
            if (!func(word_search, word_counter))
                break;
            word_counter++;
        }

        ResetReadLocation();
    }

    void CFSFile::DoEachLine(const std::function<bool(std::string& line, const uint32_t counter)>& func) {
        ASSERT_OPEN;

        std::string line;
        uint32_t line_counter = 0;
        while (getline(file, line)) {
            if (!func(line, line_counter))
                break;
            line_counter++;
        }

        ResetReadLocation();
    }

}