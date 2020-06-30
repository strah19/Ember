#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Ember
{
    class File
    {
    public:
        File(const std::string& filePath, bool append);
        virtual ~File();
        std::string get_line(int line);
        std::string get_word_from_location(int location);
        std::string get_all_file();
        int get_line_count();
        std::streamoff get_size_in_bytes();
        void delete_line(int line);
        void delete_file();
        void empty_file();
        void close_file();
        inline const std::string& get_path() const { return FilePath; }
        template <typename T>
        void write_to_file(const T& data, bool NewLine)
        {
            std::stringstream ss;
            ss << data;
            if (FileData.is_open()) {
                FileData << ss.str();
                if (NewLine)
                    FileData << '\n';
            }
        }
    private:
        std::fstream FileData;
        std::string FilePath;
        bool Append;
        void open();
    };
    class IniFile : public File
    {
    public:

    private:

    };
}

#endif // !FILE_MANAGER_H