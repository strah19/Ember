#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

//TO-DO: Make copy constructors, and operators
//TO-DO: Check and test code

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define DEBUG_FILE_TIME 1
#define FILE_STATUS_DEBUG

//File logging (can be turned on and off)
#if     DEBUG_FILE 
#define LOG(...) { __VA_ARGS__); }
#elif   DEBUG_FILE_TIME 
#define LOG(...) { printf("[" __TIME__"] " __VA_ARGS__); }
#else
#define LOG(...)
#endif

//Asserts for any assert and modes and open files
#ifdef FILE_STATUS_DEBUG
#define ASSERT(condition, message) \
        do { \
            if (! (condition)) { \
                std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                          << " line " << __LINE__ << ": " << message << std::endl; \
                FileManager::close_all_files(); \
                std::terminate(); \
            } \
        } while (false)
#define ASSERT_OPEN ASSERT(m_FileData.is_open(), "Could Not Open File " << m_FilePath)
#define ASSERT_MODE(type) ASSERT(m_FileMode == type, "File Not In Correct Mode")
#endif

//Asserts can be off but should probably left on
#ifndef  FILE_STATUS_DEBUG
#define ASSERT(condition, message) do { } while (false)
#define ASSERT_OPEN
#define ASSERT_MODE(type)
#endif

//Start of FileManager namespace
namespace FileManager
{
    enum class Mode
    {
        Writing, Reading, Invalid   //File modes to stop from having a file read and write at the same time
    };

    struct DataForm
    {
        std::vector<std::string> Data;  //Stores file in memory
    };

    class File  //Base file class
    {
    public:
        File(const std::string& filePath);
        virtual ~File();

        void empty_file();
        void close_file();
        void delete_file();

        std::streamoff              get_size_in_bytes();
        inline const std::string& get_path() const { return m_FilePath; }
        inline std::fstream& get_file() { return m_FileData; }
        std::streampos              get_current_location();
        void                        change_file_mode(const Mode& mode);
        bool                        check_if_file_empty();
        void                        change_file_location_pointer(int location);
        void                        reset_location();
        __int64                     get_line_count();
        unsigned int                get_word_count();
        unsigned int                get_location_from_word(const std::string& word);
        inline void                 auto_set_mode() { m_AutoChange = true; }        //Turns on ability to have file take care of the modes
        std::string                 get_in_memory_data(int history) const { return m_ChangableData.Data[history]; }
        unsigned long               get_character_count();

        //reading functions
        std::string read_all_file();
        std::string read_line(int lineNumber);
        std::string read_word(int location);
        std::string get_word_from_location(int location);

        //writing functions
        template <typename _Type>
        void write_at_end_of_file(const _Type& data, bool newLine);
        template <typename _Type>
        void write_at_end_of_word(const _Type& data, const std::string& finderWord);
    protected:
        std::string         m_FilePath;
        std::fstream        m_FileData;
        Mode                m_FileMode;
        unsigned long long  m_CurrentSize;
        bool                m_AutoChange;
        DataForm            m_ChangableData;

        //private functions
        void open();
        void assert_type();
        bool change_mode_auto(const Mode& mode);
        void before_reading();
        void before_writing();
        void assert_data_is_full();
    };

    static std::vector<File*> m_AllFiles;   //Pointers to all created files

    class FileManager
    {
    public:
        static bool bind_file_to_manager(File* file);
        static void check_for_opening(const std::string& fileClosingFrom);
        static void close_all_files();
    };

    bool FileManager::bind_file_to_manager(File* file)
    {
        m_AllFiles.push_back(file);             //Stores file
        LOG("Pushed File Into Manager: %s\n", file->get_path().c_str());
        return true;
    }

    void FileManager::check_for_opening(const std::string& fileClosingFrom)
    {
        //Makes sure that two files aren't open at the same time
        for (auto it : m_AllFiles)
        {
            if (it->get_file().is_open())
            {
                LOG("Closing File From File Manager: %s (Using %s)\n", it->get_path().c_str(), fileClosingFrom.c_str());
                it->close_file();
            }
        }
    }

    void FileManager::close_all_files()
    {
        //Closes all files being used
        for (auto it : m_AllFiles)
        {
            it->close_file();
            LOG("Closing File From Manager: %s\n", it->get_path().c_str());
        }
    }

    File::File(const std::string& filePath)
        : m_FilePath(filePath), m_FileMode(Mode::Reading), m_CurrentSize(0), m_AutoChange(false)
    {
        FileManager::check_for_opening(m_FilePath);
        if (FileManager::bind_file_to_manager(this))
        {
            open();
            get_size_in_bytes();    //Size is always stored in memory
            assert_type();
        }
    }

    File::~File()
    {
        LOG("Closing File %s\n", m_FilePath.c_str());

        m_FileData.close();
    }

    void File::open()
    {
        if (m_FileMode == Mode::Reading)
        {
            m_FileData = std::fstream(m_FilePath, std::fstream::in | std::fstream::app);
            LOG("File Type: Reading To File\n");
        }
        if (m_FileMode == Mode::Writing)
        {
            m_FileData = std::fstream(m_FilePath, std::fstream::out | std::fstream::app);
            LOG("File Type: Writing To File\n");
        }

        ASSERT_OPEN;
        LOG("Opened File %s\n", m_FilePath.c_str());
    }

    void File::assert_type()
    {
        ASSERT_OPEN;
        if (m_FileMode == Mode::Reading)
        {
            if (m_CurrentSize < 1)
            {
                m_FileMode = Mode::Writing;
                LOG("Changed Mode Because Could Not Read Empty File\n");
                close_file();
                open();
            }
        }
    }

    bool File::change_mode_auto(const Mode& mode)
    {
        if (m_AutoChange)
        {
            change_file_mode(mode);
            return true;
        }
        return false;
    }

    void File::close_file()
    {
        LOG("Closing File %s\n", m_FilePath.c_str());

        m_FileData.close();
    }

    void File::delete_file()
    {
        m_FileData.close();
        remove(m_FilePath.c_str());
    }

    void File::empty_file()
    {
        m_FileData.close();
        m_FileData.open(m_FilePath.c_str(), std::fstream::out | std::fstream::trunc);
        close_file();
        open();
        ASSERT_OPEN;
    }

    std::streamoff File::get_size_in_bytes()
    {
        ASSERT_OPEN;

        std::streampos beginningOfFile, endOfFile;
        m_FileData.seekg(0, std::ios::beg);
        beginningOfFile = m_FileData.tellg();
        m_FileData.seekg(0, std::ios::end);
        endOfFile = m_FileData.tellg();
        LOG("File Size In Bytes: %llu\n", endOfFile - beginningOfFile);

        m_CurrentSize = endOfFile - beginningOfFile;

        return endOfFile - beginningOfFile;
    }

    std::streampos File::get_current_location()
    {
        return m_FileData.tellg();
    }

    std::string File::read_all_file()
    {
        before_reading();

        m_FileData.seekg(0, std::ios::end);
        std::streampos sizeOfWholeFile = get_current_location();
        std::string bufferOfWholeFile((size_t)sizeOfWholeFile, ' ');
        m_FileData.seekg(0);
        m_FileData.read(&bufferOfWholeFile[0], sizeOfWholeFile);

        m_ChangableData.Data.push_back(bufferOfWholeFile);

        return bufferOfWholeFile;
    }

    void File::change_file_mode(const Mode& mode)
    {
        if (m_FileMode != mode && mode != Mode::Invalid)
        {
            m_FileMode = mode;
            LOG("Changing Mode\n");
            close_file();
            open();
            assert_type();
        }
        else
        {
            LOG("Could Not Process File Mode Change Command\n");
        }
    }

    bool File::check_if_file_empty()
    {
        return (get_size_in_bytes() == 0) ? true : false;
    }

    void File::change_file_location_pointer(int location)
    {
        ASSERT_OPEN;
        LOG("Changed Location To %d\n", location);
        m_FileData.seekg(location, std::ios::beg);
    }

    void File::reset_location()
    {
        ASSERT_OPEN;
        LOG("Reset Location\n");
        m_FileData.clear();
        m_FileData.seekg(std::ios::beg);
    }

    std::string File::read_line(int lineNumber)
    {
        before_reading();

        std::string searchLineOfFile;
        unsigned int lineCounterForFile = 0;

        while (getline(m_FileData, searchLineOfFile))
        {
            if (lineCounterForFile == lineNumber)
            {
                return searchLineOfFile;
            }
            lineCounterForFile++;
        }

        return m_FilePath;
    }

    std::string File::get_word_from_location(int location)
    {
        before_reading();

        unsigned int wordCounterForFile = 0;
        std::string searchWordOfFile;

        while (m_FileData >> searchWordOfFile)
        {
            if (wordCounterForFile == location)
            {
                return searchWordOfFile;
            }
            wordCounterForFile++;
        }

        return m_FilePath;
    }

    unsigned int File::get_word_count()
    {
        before_reading();

        std::string searchWordOfFile;
        unsigned int wordCounterForFile = 0;
        while (m_FileData >> searchWordOfFile)
        {
            wordCounterForFile++;
        }

        return wordCounterForFile;
    }

    __int64 File::get_line_count()
    {
        before_reading();

        return std::count(std::istreambuf_iterator<char>(m_FileData),
            std::istreambuf_iterator<char>(), '\n') + 1;
    }

    std::string File::read_word(int location)
    {
        before_reading();

        std::string searchWordOfFile;
        unsigned int wordCounterForFile = 0;
        while (m_FileData >> searchWordOfFile)
        {
            if (wordCounterForFile == location)
            {
                return searchWordOfFile;
            }
            wordCounterForFile++;
        }

        return m_FilePath;
    }

    unsigned int File::get_location_from_word(const std::string& word)
    {
        before_reading();

        std::string searchWordOfFile;
        unsigned int wordCounterForFile = 0;
        while (m_FileData >> searchWordOfFile)
        {
            if (searchWordOfFile == word)
            {
                return wordCounterForFile;
            }
            wordCounterForFile++;
        }

        return -1;
    }

    void File::before_reading()
    {
        ASSERT_OPEN;
        change_mode_auto(Mode::Reading);
        ASSERT_MODE(Mode::Reading);
        reset_location();
    }

    void File::before_writing()
    {
        ASSERT_OPEN;
        change_mode_auto(Mode::Writing);
        ASSERT_MODE(Mode::Writing);
        reset_location();
    }

    void File::assert_data_is_full()
    {
        bool isWriting = false;
        if (m_FileMode == Mode::Writing)
        {
            isWriting = true;
        }
        if (m_CurrentSize > 0)
        {
            LOG("Storing Data In Memory\n");
            if (change_mode_auto(Mode::Reading))
            {
                read_all_file();
            }
            else
            {
                change_file_mode(Mode::Reading);
                read_all_file();
                if (isWriting)
                {
                    change_file_mode(Mode::Writing);
                }
            }
        }
        else
        {
            m_ChangableData.Data.push_back(std::string());
        }
    }

    template <typename _Type>
    void File::write_at_end_of_file(const _Type& data, bool newLine)
    {
        assert_data_is_full();
        before_writing();
        std::stringstream buffer;
        buffer << data;

        if (newLine)
        {
            m_ChangableData.Data.back().append("\n");
        }
        m_ChangableData.Data.back().append(buffer.str());

        empty_file();
        open();
        m_FileData << m_ChangableData.Data.back();
        get_size_in_bytes();
    }

    unsigned long File::get_character_count()   //includes '\n' and ' ' for now (FIX)
    {
        before_reading();

        char activeCharacterBeingRead;
        unsigned long numberofCharacters = 0;

        while (m_FileData.get(activeCharacterBeingRead))
        {
            numberofCharacters++;
        }
        LOG("Number of characters: %d\n", numberofCharacters);

        return numberofCharacters;
    }

    template <typename _Type>
    void File::write_at_end_of_word(const _Type& data, const std::string& finderWord)
    {
        assert_data_is_full();
        before_writing();

        size_t locationOfFoundWord = m_ChangableData.Data.back().find(finderWord);
        std::stringstream buffer(data);

        if (locationOfFoundWord != std::string::npos)
        {
            m_ChangableData.Data.back().insert(locationOfFoundWord + finderWord.length(), buffer.str());
        }
        else
        {
            LOG("Could Not Find finderWord In File.\n");
        }

        empty_file();
        open();
        m_FileData << m_ChangableData.Data.back();
        get_size_in_bytes();
    }
}

#endif // !FILE_MANAGER_H