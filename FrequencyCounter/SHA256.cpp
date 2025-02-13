#include "SHA256.hpp"
#include <cstring>
#include <sstream>
#include <iomanip>
#include <array>

const unsigned int SHA256::kInitHash[SHA256::kHashSize / 4] = 
{
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

static const unsigned int k[64] = 
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

SHA256::SHA256() 
{
    std::memcpy(mHash, kInitHash, kHashSize);
    mBlockByteIndex = 0;
    mByteCount = 0;
}

void SHA256::update(const unsigned char* data, size_t len) 
{
    for (size_t i = 0; i < len; ++i) 
    {
        mBlock[mBlockByteIndex++] = data[i];
        ++mByteCount;
        if (mBlockByteIndex == kBlockSize) 
        {
            processBlock();
            mBlockByteIndex = 0;
        }
    }
}

void SHA256::final(unsigned char* hash) 
{
    processPadding();
    for (size_t i = 0; i < kHashSize / 4; ++i) 
    {
        hash[i * 4] = (mHash[i] >> 24) & 0xff;
        hash[i * 4 + 1] = (mHash[i] >> 16) & 0xff;
        hash[i * 4 + 2] = (mHash[i] >> 8) & 0xff;
        hash[i * 4 + 3] = mHash[i] & 0xff;
    }
}

std::string SHA256::hash(const std::string& data) 
{
    SHA256 sha256;
    sha256.update(reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
    unsigned char hash[kHashSize];
    sha256.final(hash);

    std::stringstream ss;
    for (size_t i = 0; i < kHashSize; ++i) 
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

void SHA256::processBlock() 
{
    unsigned int w[64];
    for (size_t i = 0; i < 16; ++i) 
    {
        w[i] = (mBlock[i * 4] << 24) | (mBlock[i * 4 + 1] << 16) | (mBlock[i * 4 + 2] << 8) | (mBlock[i * 4 + 3]);
    }
    for (size_t i = 16; i < 64; ++i) 
    {
        w[i] = theta1(w[i - 2]) + w[i - 7] + theta0(w[i - 15]) + w[i - 16];
    }

    unsigned int a = mHash[0], b = mHash[1], c = mHash[2], d = mHash[3];
    unsigned int e = mHash[4], f = mHash[5], g = mHash[6], h = mHash[7];

    for (size_t i = 0; i < 64; ++i) 
    {
        unsigned int t1 = h + sig1(e) + choose(e, f, g) + k[i] + w[i];
        unsigned int t2 = sig0(a) + majority(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    mHash[0] += a;
    mHash[1] += b;
    mHash[2] += c;
    mHash[3] += d;
    mHash[4] += e;
    mHash[5] += f;
    mHash[6] += g;
    mHash[7] += h;
}

void SHA256::processPadding() 
{
    size_t i = mBlockByteIndex;
    mBlock[i++] = 0x80;

    if (i > kBlockSize - 8) {
        std::memset(mBlock + i, 0, kBlockSize - i);
        processBlock();
        i = 0;
    }
    std::memset(mBlock + i, 0, kBlockSize - 8 - i);

    mByteCount *= 8;
    mBlock[56] = (mByteCount >> 56) & 0xff;
    mBlock[57] = (mByteCount >> 48) & 0xff;
    mBlock[58] = (mByteCount >> 40) & 0xff;
    mBlock[59] = (mByteCount >> 32) & 0xff;
    mBlock[60] = (mByteCount >> 24) & 0xff;
    mBlock[61] = (mByteCount >> 16) & 0xff;
    mBlock[62] = (mByteCount >> 8) & 0xff;
    mBlock[63] = mByteCount & 0xff;

    processBlock();
}

unsigned int SHA256::rotr(unsigned int x, unsigned int n) 
{
    return (x >> n) | (x << (32 - n));
}

unsigned int SHA256::choose(unsigned int e, unsigned int f, unsigned int g) 
{
    return (e & f) ^ (~e & g);
}

unsigned int SHA256::majority(unsigned int a, unsigned int b, unsigned int c) 
{
    return (a & b) ^ (a & c) ^ (b & c);
}

unsigned int SHA256::sig0(unsigned int x) 
{
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

unsigned int SHA256::sig1(unsigned int x) 
{
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

unsigned int SHA256::theta0(unsigned int x) 
{
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

unsigned int SHA256::theta1(unsigned int x) 
{
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}