#ifndef SHA256_HPP
#define SHA256_HPP

#include <string>

class SHA256 {
public:
    SHA256();
    void update(const unsigned char* data, size_t len);
    void final(unsigned char* hash);
    static std::string hash(const std::string& data);

private:
    static const size_t kBlockSize = 64;
    static const size_t kHashSize = 32;
    static const unsigned int kInitHash[kHashSize / 4];

    unsigned int mHash[kHashSize / 4];
    unsigned char mBlock[kBlockSize];
    size_t mBlockByteIndex;
    size_t mByteCount;

    void processBlock();
    void processPadding();
    static unsigned int rotr(unsigned int x, unsigned int n);
    static unsigned int choose(unsigned int e, unsigned int f, unsigned int g);
    static unsigned int majority(unsigned int a, unsigned int b, unsigned int c);
    static unsigned int sig0(unsigned int x);
    static unsigned int sig1(unsigned int x);
    static unsigned int theta0(unsigned int x);
    static unsigned int theta1(unsigned int x);
};

#endif // SHA256_HPP
