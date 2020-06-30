#include "FileManager.h"
#include <vector>

namespace Ember
{
    void check_for_closure(std::fstream* s)
    {
        static std::vector<std::fstream*> Streams;
        Streams.push_back(s);
        if (Streams.size() > 2) {
            for (auto& i : Streams) {
                if (i->is_open()) {
                    i->close();
                    break;
                }
            }
        }
    }
    File::File(const std::string& filePath, bool append)
    {
        Append = append;

        FilePath = filePath;
        std::fstream temp;
        temp.open(filePath.c_str());
        temp.close();

        open();

        check_for_closure(&FileData);
    }
    File::~File()
    {
        FileData.close();
    }
    std::string File::get_line(int line)
    {
        std::string Stringline;
        std::string l;
        FileData.clear();
        FileData.seekg(std::ios::beg);
        unsigned int counter = 0;
        if (FileData.is_open()) {
            while (getline(FileData, Stringline)) {
                if (counter == line)
                    l = Stringline;
                counter++;
            }
        }

        FileData.clear();

        return l;
    }
    std::string File::get_word_from_location(int location)
    {
        FileData.clear();
        FileData.seekg(std::ios::beg);
        unsigned int counter = 0;
        std::string line;
        std::string value;

        while (FileData >> line) {
            if (counter == location)
                value = line;
            counter++;
        }
        FileData.clear();
        return value;
    }
    std::string File::get_all_file()
    {
        std::string line;
        std::string allData;

        FileData.clear();
        FileData.seekg(std::ios::beg);

        if (FileData.is_open()) {
            while (getline(FileData, line)) {
                line.append("\n");
                allData.append(line);
            }
        }

        FileData.clear();

        return allData;
    }
    std::streamoff File::get_size_in_bytes()
    {
        std::streampos begin, end;
        FileData.seekg(0, std::ios::beg);
        begin = FileData.tellg();
        FileData.seekg(0, std::ios::end);
        end = FileData.tellg();
        return end - begin;
    }
    int File::get_line_count()
    {
        unsigned int counter = 0;
        FileData.clear();
        FileData.seekg(std::ios::beg);

        if (FileData.is_open()) {
            std::string line;
            while (getline(FileData, line)) {
                counter++;
            }
        }
        return counter;;

    }
    void File::delete_line(int line)
    {
        if (FileData.is_open()) {
            FileData.clear();
            FileData.seekg(std::ios::beg);
            std::ofstream ofs;
            ofs.open("temp.txt", std::ofstream::out);
            char c;
            int ln = 1;
            while (FileData.get(c)) {
                if (c == '\n')
                    ln++;

                if (ln != line)
                    ofs << c;

            }

            FileData.close();
            ofs.close();
            remove(FilePath.c_str());
            rename("temp.txt", FilePath.c_str());
        }
    }
    void File::delete_file()
    {
        if (FileData.is_open()) {
            FileData.close();
            remove(FilePath.c_str());
        }
    }
    void File::empty_file()
    {
        if (FileData.is_open()) {
            FileData.close();
            FileData.open(FilePath.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
        }
    }
    void File::close_file()
    {
        FileData.close();
    }
    void File::open()
    {
        if (!Append) {
            FileData.open(FilePath.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
        }
        else if (Append) {
            FileData.open(FilePath.c_str(), std::fstream::in | std::fstream::out | std::fstream::ate);
        }
    }
}