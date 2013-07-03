#include "filesystem_read.hpp"

// Must be valid utf-8!
is::File::Read::Read( std::string dir ) {
    m_open = false;
    m_exists = filesystem->exists( dir );
    if ( m_exists ) {
        m_file = PHYSFS_openRead( dir.c_str() );
        if ( !m_file ) {
            std::string absolutedir = PHYSFS_getRealDir( dir.c_str() );
            os->printf( "ERR %/%: %", absolutedir, dir, PHYSFS_getLastError());
            return;
        }
        m_eof = PHYSFS_eof( m_file );
        m_open = true;
    } else {
        os->printf( "ERR File % does not exist!\n", dir );
    }
}

is::File::Read::~Read() {
    close();
}

unsigned int is::File::Read::read( void* buffer, unsigned int size ) {
    if ( !good() ) {
        return 0;
    }
    return PHYSFS_read( m_file, buffer, 1, size );
}

unsigned int is::File::Read::size() {
    if ( !m_exists ) {
        return 0;
    }
    return PHYSFS_fileLength( m_file );
}

void is::File::Read::close() {
    if (m_file)
    {
        PHYSFS_close(m_file);
    }
    m_open = false;
}

bool is::File::Read::good() {
    if ( m_exists ) {
        m_eof = PHYSFS_eof( m_file );
    }
    return ( m_open && m_exists && ~m_eof && m_file );
}
