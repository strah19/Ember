#ifndef CFSFILE_H
#define CFSFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <sstream>

/* CINDER_DEBUG can be activated by going to Properties->C/C++->Preprocessor->Preprocessor Definations */
#ifdef CINDER_DEBUG
#define ASSERT(condition, message) \
        do { \
            if (! (condition)) { \
                std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                          << " line " << __LINE__ << ": " << message << std::endl; \
                std::terminate(); \
            } \
        } while (false)
#define ASSERT_OPEN ASSERT(file.is_open(), "Could Not Open File " << file_path)
#endif

#ifndef  CINDER_DEBUG
#define ASSERT(condition, message) do { } while (false)
#define ASSERT_OPEN
#endif

namespace Cinder {
    /* Base class that can read and write files. */
    class CFSFile {
    public:
        /**
        * Basic file input output handler.
        * 
        * @param The path of the desired file.
        */
        CFSFile(const char* file_path);

        /**
        * Basic file input output handler.
        */
        CFSFile();

        /**
        * Basic file input output handler.
        * 
        * @param A source for the CFSFile to use.
        */
        CFSFile(CFSFile& file);

        /**
        * Equal operator.
        * 
        * @param A source for CFSFile to use.
        */
        void operator= (const CFSFile& file);

        /**
        * Will close the file automatically.
        *
        */
        virtual ~CFSFile();

        /**
        * Delete contents in file.
        */
        void EmptyFile();

        /**
        * Close the file.
        */
        void CloseFile();

        /**
        * Completely destroy it from the directory.
        */
        void Destroy();

        /**
        * Open the file for use.
        */
        void Open();

        /**
        * Check if the read pointer = -1.
        * 
        * @return true or false for reaching -1
        */
        bool IsReadLocationToEnd();

        /**
        * Give the current read location.
        * 
        * @return the read location.
        */
        std::streampos GetCurrentReadLocation();

        /**
        * Will set the read location back to the top of the file.
        */
        void ResetReadLocation();

        /**
        * Can set the read location.
        * 
        * @param The desired read location.
        */
        void SetReadLocation(const uint32_t read_location);

        /**
        * Get the file in bytes.
        * 
        * @return The number of bytes.
        */
        std::streamoff GetFileSizeInBytes();

        /**
        * Checks if the file is empty.
        * 
        * @return True or false if file is empty or not.
        */
        bool IsFileEmpty();

        /**
        * Get the current working path.
        * 
        * @return The path.
        */
        const char* GetPath();

        /**
        * The line count of the file.
        * 
        * @return The line number.
        */
        uint64_t GetLineCount();

        /**
        * The charcter count of the file.
        * 
        * @return The number of charcters.
        */
        uint64_t GetCharacterCount();

        /**
        * The number of words in a file.
        * 
        * @return The number of words.
        */
        uint64_t GetWordCount();

        /**
        * Reads the entire file.
        * 
        * @return The whole file in a string format.
        */
        std::string ReadFile();

        /**
        * Will read a specific line.
        * 
        * @param The line that wants to be read starting at 0.
        * 
        * @return The line a string format.
        */
        std::string ReadLine(const uint32_t line_number);

        /**
        * Will read a word according to the location.
        * 
        * @param The location of the word in the file starting at 0.
        * 
        * @return The word in a string.
        */
        std::string ReadWord(const uint32_t location);

        /**
        * Will find the position of the first seen word.
        * 
        * @param The word being searched for.
        * 
        * @return The words location in the file.
        */
        uint32_t GetFirstLocationFromWord(const char* word);
    public:
        /**
        * Will delete the desired line.
        * 
        * @param The line number to be deleted.
        */
        void DeleteLine(const uint32_t line_number);

        /**
        * Will read in an object using's it's size according to the read location.
        * 
        * @param Object that will be filled.
        */
        template <typename T>
        void ReadObjectOfSize(T& object) {
            ASSERT_OPEN;
            file.read((char*)&object, sizeof(object));
        }

        /**
        * Will write in the object passed.
        * 
        * @param The object going into the file.
        */
        template <typename T>
        void WriteObjectOfSize(T& object) {
            ASSERT_OPEN;
            file.write((char*)&object, sizeof(object));
        }

        /**
        * Will write information at the end of a given line.
        * 
        * @param The informartion to be written.
        * @param The line number.
        */
        template<typename T>
        void WriteAtEndLine(const T& stream, const uint32_t line_number) {
            ASSERT_OPEN;
            std::string file_data = ReadFile();

            int word_counter = 0;
            if (line_number > -1) {
                for (size_t i = 0; i < file_data.length(); i++) {
                    if (file_data[i] == '\n') {
                        if (line_number == word_counter) {
                            std::stringstream buffer;
                            buffer << stream;
                            file_data.insert(i, buffer.str());
                            break;
                        }
                        word_counter++;
                    }
                }

                EmptyFile();
                file << file_data;
            }
        }

        /**
        * Find the first word then write the information after it.
        *
        * @param The information to write after the word.
        * @param The word that will be found to write the information.
        */
        template <typename T>
        void WriteAfterFirstWord(const T& stream, const std::string& finder_word) {
            ASSERT_OPEN;

            std::string file_data = ReadFile();

            size_t loc_word = file_data.find(finder_word);
            std::stringstream buffer(stream);

            if (loc_word != std::string::npos)
                file_data.insert(loc_word + finder_word.length(), buffer.str());

            EmptyFile();
            file << file_data;
        }

        /**
        * Simple write function that will take information and write at end of the line.
        *
        * @param The information that will be written.
        */
        template <typename T>
        void Write(const T& stream) {
            ASSERT_OPEN;

            std::stringstream buffer;
            buffer << stream;

            file << buffer.str();
        }

        /**
        * Will cycle through each word and be able to operate on the information passed in.
        *
        * @param The user lambda that will operate with the given info.
        */
        void DoEachWord(const std::function<bool(std::string& word, const uint32_t counter)>& func);

        /**
        * Will cycle through each line and be able to operate on the information passed in.
        *
        * @param The user lambda that will operate with the given info.
        */
        void DoEachLine(const std::function<bool(std::string& line, const uint32_t counter)>& func);
    protected:
        std::string file_path;
        std::fstream file;

    private:
        void InitFromCFSFile(const CFSFile& file);
        bool CheckPath();
    };
}

#endif // !CFSFILE_H
