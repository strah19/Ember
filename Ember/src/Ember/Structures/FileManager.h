#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// Asserts for any assert and modes and open files.
#ifdef FILE_STATUS_DEBUG
#define ASSERT(condition, message) \
        do { \
            if (! (condition)) { \
                std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                          << " line " << __LINE__ << ": " << message << std::endl; \
                Close(); \
                std::terminate(); \
            } \
        } while (false)
#define ASSERT_OPEN ASSERT(file.is_open(), "Could Not Open File " << file_path)
#endif

// Asserts can be off but should probably left on.
#ifndef  FILE_STATUS_DEBUG
#define ASSERT(condition, message) do { } while (false)
#define ASSERT_OPEN
#endif

namespace ember
{
    class File {
    public:
        File(const char* file_path);
        virtual ~File();

        void EmptyFile();
        void CloseFile();
        void DeleteFile();

        std::streampos CurrentLocation(); 
        void ChangeLocation(const int location);
        void ResetLocation();

        __int64 LineCount();
        unsigned int WordCount();
        unsigned long CharacterCount();
       
        std::streamoff SizeInBytes();
        bool IfEmpty();
        inline const std::string Path() const { return file_path; }

        // Reading functions.
        std::string ReadAll();
        std::string ReadLine(const int line_number);
        std::string ReadWord(const int location);
        int LocationToWord(const char* word);

        // Writing functions.
        template <typename T>
        void Write(const T& data);
        template <typename T>
        void WriteAfterWord(const T& data, std::string& finder_word);
        template <typename T>
        void WriteAtEndLine(const T& data, int line_number);

        template <typename T>
        void WriteObject(const T& obj);
        template <typename T>
        void ReadObject(const T& obj);

        void DeleteLine(const int line_number);
    protected:
        std::string file_path;
        std::fstream file;

        void Open();
    };

    static std::vector<File*> files;   // Pointers to all created files.

    static bool Bind(File* file);
    static void Check(const char* host_file);
    static void Close();

    bool Bind(File* file) {
        files.push_back(file);             // Stores file.
        return true;
    }

    void Check(const char* host_file) {
        // Makes sure that two file instances with the same host path is not open.
        for (auto& i : files) 
            if (i->Path() == host_file) 
                i->CloseFile();     
    }

    void Close() {
        // Closes all files being used.
        for (auto i : files) 
            i->CloseFile();
    }

    File::File(const char* file_path)
        : file_path(file_path) {
        Check(file_path);
        if (Bind(this)) 
            Open(); 
    }

    File::~File() {
        CloseFile();
    }

    void File::Open() {
        file = std::fstream(file_path, std::fstream::in | std::fstream::out | std::fstream::app);
        ASSERT_OPEN;
    }

    void File::CloseFile() {
        file.flush();
        file.close();
    }

    void File::DeleteFile() {
        file.close();
        remove(file_path.c_str());
    }

    void File::EmptyFile() {
        CloseFile();
        file.open(file_path.c_str(), std::fstream::out | std::fstream::trunc);
        CloseFile();
        Open();
    }

    std::streamoff File::SizeInBytes() {
        ASSERT_OPEN;

        std::streampos beg, end;
        file.seekg(0, std::ios::beg);
        beg = file.tellg();
        file.seekg(0, std::ios::end);
        end = file.tellg();

        return end - beg;
    }

    std::streampos File::CurrentLocation() {
        return file.tellg();
    }

    bool File::IfEmpty() {
        return (SizeInBytes() == 0) ? true : false;
    }

    void File::ResetLocation() {
        ASSERT_OPEN;

        file.clear();
        file.seekg(0);
    }

    void File::ChangeLocation(const int location) {
        ASSERT_OPEN;
        file.seekg(location, std::ios::beg);
    }

    unsigned int File::WordCount() {
        ASSERT_OPEN;
        ResetLocation();

        std::string search_word;
        unsigned int word_counter = 0;
        while (file >> search_word) 
            word_counter++;
  
        return word_counter;
    }

    __int64 File::LineCount() {
        ASSERT_OPEN;
        ResetLocation();

        return std::count(std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>(), '\n');
    }

    int File::LocationToWord(const char* word) {
        ASSERT_OPEN;
        ResetLocation();
        
        std::string word_search;
        int word_counter = 0;
        while (file >> word_search) {
            if (word_search == word) 
                return word_counter;
            
            word_counter++;
        }

        return -1;
    }

    unsigned long File::CharacterCount() {
        ASSERT_OPEN;
        ResetLocation();
       
        char active_char;
        unsigned long number_chars = 0;

        while (file.get(active_char)) 
            if(active_char != '\n')
                number_chars++;

        return number_chars;
    }

    std::string File::ReadAll() {
        ASSERT_OPEN;
        ResetLocation();

        std::string buffer((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        return buffer;
    }

    std::string File::ReadWord(const int location) {
        ASSERT_OPEN;
        ResetLocation();
        
        std::string word_search;
        unsigned int word_counter = 0;
        while (file >> word_search) {
            if (word_counter == location) 
                return word_search;        
            word_counter++;
        }

        return file_path;
    }

    std::string File::ReadLine(const int line_number) {
        ASSERT_OPEN;
        ResetLocation();

        std::string line_search;
        unsigned int line_counter = 0;

        while (getline(file, line_search)) {
            if (line_counter == line_number) 
                return line_search;
            line_counter++;
        }

        return file_path;
    }

    template <typename T>
    void File::Write(const T& data) {
        ASSERT_OPEN;

        std::stringstream buffer;
        buffer << data;

        file << buffer.str();
    }

    template <typename T>
    void File::WriteAfterWord(const T& data, std::string& finder_word) {
        ASSERT_OPEN;

        std::string file_data = ReadAll();

        size_t loc_word = file_data.find(finder_word);
        std::stringstream buffer(data);

        if (loc_word != std::string::npos) 
            file_data.insert(loc_word + finder_word.length(), buffer.str());

        EmptyFile();
        file << file_data;
    }

    template<typename T>
    void File::WriteAtEndLine(const T& data, int line_number) {
        ASSERT_OPEN;
        std::string file_data = ReadAll();

        int word_counter = 0;
        for (size_t i = 0; i < file_data.length(); i++) {
            if (file_data[i] == '\n') {
                if (line_number == word_counter) {
                    std::stringstream buffer;
                    buffer << data;
                    file_data.insert(i, buffer.str());
                    break;
                }
                word_counter++;
            }
        }

        EmptyFile();
        file << file_data;
    }

    void File::DeleteLine(const int line_number) {
        ASSERT_OPEN;
        std::string file_data = ReadAll();

        std::string line_search;
        unsigned int line_counter = 0;

        std::vector<size_t> line_sizes;

        while (getline(file, line_search)) {
            line_sizes.push_back(line_search.length());

            if (line_counter == line_number) {
                size_t location = 0;
                for (size_t i = 0; i < line_sizes.size() - 1; i++) {
                    location += line_sizes[i];
                }
                file_data.erase(location, line_sizes.back() + 1);
                break;
            }
            line_counter++;
        }

        line_sizes.clear();

        EmptyFile();
        file << file_data;
    }

    template <typename T>
    void File::WriteObject(const T& obj) {
        ASSERT_OPEN;
        file.write((char*)&obj, sizeof(obj));
    }

    template <typename T>
    void File::ReadObject(const T& obj) {
        ASSERT_OPEN;
        file.read((char*)&obj, sizeof(obj));
    }
}

#endif // !FILE_MANAGER_H