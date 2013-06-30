// Big templates functions that must be implemented in a header are placed in a source file like this one to be included. Just for organization and cleanliness sake.

template<typename T, typename... Args>
int is::OS::printf( const char* s, T value, Args... args ) {
    int parses = 0;
    while (*s) {
        if (*s == '%') {
            if (*(s + 1) == '%') {
                ++s;
            } else {
                std::cout << value;
                parses += 1+printf(s + 1, args...); // call even when *s == 0 to detect extra arguments
                return parses;
            }
        }
        std::cout << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
}
