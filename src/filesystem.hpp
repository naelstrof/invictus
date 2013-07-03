#ifndef IS_FILESYSTEM_H_
#define IS_FILESYSTEM_H_

#include <physfs.h>

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
    std::vector<std::string>    getFiles( std::string dir );
    bool                        exists( std::string dir );
};

};

extern is::FileSystem* filesystem;

#endif // IS_FILESYSTEM_H_
