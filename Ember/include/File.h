#ifndef FILE_H
#define FILE_H

#include <string>
#include <functional>
#include <sstream>
#include <fstream>

namespace Ember {
    class File {
    public:
        /**
        * Basic file input output handler.
        *
        * @param The path of the desired file.
        */
        File(const char* file_path);

        /**
        * Basic file input output handler.
        */
        File();

        /**
        * Basic file input output handler.
        *
        * @param A source for the CFSFile to use.
        */
        File(File& file);

        /**
        * Equal operator.
        *
        * @param A source for CFSFile to use.
        */
        void operator= (const File& file);

        /**
        * Will close the file automatically.
        *
        */
        virtual ~File();

        /**
        * Delete contents in file.
        */
        void empty();

        /**
        * Close the file.
        */
        void close();

        /**
        * Completely destroy it from the directory.
        */
        void destroy();

        /**
        * Open the file for use.
        */
        void open();

        /**
        * Checks if file was successfully opened.
        */
        bool is_open();

        /**
        * Check if the read pointer = -1.
        *
        * @return true or false for reaching -1
        */
        bool eof();

        /**
        * Give the current read location.
        *
        * @return the read location.
        */
        std::streampos location();

        /**
        * Will set the read location back to the top of the file.
        */
        void reset();

        /**
        * Can set the read location.
        *
        * @param The desired read location.
        */
        void location(const uint32_t read_location);

        /**
        * Get the file in bytes.
        *
        * @return The number of bytes.
        */
        std::streamoff size();

        /**
        * Checks if the file is empty.
        *
        * @return True or false if file is empty or not.
        */
        bool is_empty();

        /**
        * Get the current working path.
        *
        * @return The path.
        */
        const char* path();

        /**
        * The line count of the file.
        *
        * @return The line number.
        */
        uint64_t line_count();

        /**
        * The charcter count of the file.
        *
        * @return The number of charcters.
        */
        uint64_t character_count();

        /**
        * The number of words in a file.
        *
        * @return The number of words.
        */
        uint64_t word_count();

        /**
        * Reads the entire file.
        *
        * @return The whole file in a string format.
        */
        std::string read();

        /**
        * Will read a specific line.
        *
        * @param The line that wants to be read starting at 0.
        *
        * @return The line a string format.
        */
        std::string read_line(const uint32_t line_number);

        /**
        * Will read a word according to the location.
        *
        * @param The location of the word in the file starting at 0.
        *
        * @return The word in a string.
        */
        std::string read_word(const uint32_t location);

        /**
        * Will delete the desired line.
        *
        * @param The line number to be deleted.
        */
        void delete_line(const uint32_t line_number);

        /**
        * Will read in an object using's it's size according to the read location.
        *
        * @param Object that will be filled.
        */
        template <typename T>
        void read_obj(T& object) {
            file.read((char*)&object, sizeof(object));
        }

        /**
        * Will write in the object passed.
        *
        * @param The object going into the file.
        */
        template <typename T>
        void write_obj(T& object) {
            file.write((char*)&object, sizeof(object));
        }

        /**
        * Will write information at the end of a given line.
        *
        * @param The informartion to be written.
        * @param The line number.
        */
        template<typename T>
        void write_at_line(const T& stream, const uint32_t line_number) {
            std::string file_data = read();

            int word_counter = 0;
            if (line_number > -1) {
                for (size_t i = 0; i < file_data.length(); i++) {
                    if (file_data[i] == '\n') {
                        if (line_number == word_counter) {
                            std::stringstream ss;
                            ss << stream;
                            file_data.insert(i, ss.str());
                            break;
                        }
                        word_counter++;
                    }
                }

                empty();
                file << file_data;
            }
        }

        /**
        * Simple write function that will take information and write at end of the line.
        *
        * @param The information that will be written.
        */
        template <typename T>
        void write(const T& stream) {
            std::stringstream ss;
            ss << stream;

            file << ss.str();
        }

        /**
        * Will cycle through each word and be able to operate on the information passed in.
        *
        * @param The user lambda that will operate with the given info.
        */
        void do_each_word(const std::function<bool(const std::string& word, const uint32_t counter)>& func);

        /**
        * Will cycle through each line and be able to operate on the information passed in.
        *
        * @param The user lambda that will operate with the given info.
        */
        void do_each_line(const std::function<bool(const std::string& line, const uint32_t counter)>& func);
    protected:
        std::string file_path;
        std::fstream file;
    private:
        void init(const File& file);
        bool check_path();
    };
}

#endif // !FILE_H