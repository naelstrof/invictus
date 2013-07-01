#ifndef IS_FILESYSTEM_H_
#define IS_FILESYSTEM_H_

#include <physfs.h>
#include <System/Utf.hpp>
#include <System/String.hpp>

#include "os.hpp"

#include "filesystem_read.hpp"

namespace is {

class FileSystem {
private:
    std::string                 m_pwd;
    void                        checkError();
public:
    FileSystem();
    ~FileSystem();
    int                         init( std::string pwd );
    void                        tick();
    std::vector<std::string>    getFiles( sf::String dir );
    bool                        exists( sf::String dir );
};

};

extern is::FileSystem* filesystem;

#endif // IS_FILESYSTEM_H_
