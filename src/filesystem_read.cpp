#include "filesystem_read.hpp"

// What a confusing cluster of 'dir' name variants. absolutedir is the full path, actualdir is just a utf-8 version of dir.
is::File::Read::Read( sf::String dir ) {
    std::string actualdir;
    sf::Utf<32>::toUtf8( dir.begin(), dir.end(), back_inserter( actualdir ) );
    m_open = false;
    m_exists = filesystem->exists( actualdir );
    if ( m_exists ) {
        m_file = PHYSFS_openRead( actualdir.c_str() );
        if ( !m_file ) {
            std::string absolutedir = PHYSFS_getRealDir( actualdir.c_str() );
            os->printf( "ERR %/%: %", absolutedir, actualdir, PHYSFS_getLastError());
            return;
        }
        m_eof = PHYSFS_eof( m_file );
        m_open = true;
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
    return PHYSFS_fileLength(m_file);
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
