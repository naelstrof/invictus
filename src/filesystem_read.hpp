#ifndef IS_FILESYSTEM_READ_H_
#define IS_FILESYSTEM_READ_H_

#include "os.hpp"
#include "filesystem.hpp"

namespace is {
namespace File {

class Read {
public:
    Read( std::string dir );
    ~Read();
    void close();
    bool good();
    unsigned int read( void* buffer, unsigned int size );
    unsigned int size();
private:
    bool m_open;
    bool m_exists;
    bool m_eof;
    PHYSFS_File* m_file;
};

};
};

#endif // IS_FILESYSTEM_READ_H_
