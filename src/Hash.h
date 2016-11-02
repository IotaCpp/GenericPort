/* === DONE === */

#ifndef HASH_H_
#define HASH_H_

#include "Curl.h"
#include "Converter.h"
#include <gsl/gsl>
using gsl::byte;
using gsl::to_integer;
#include <cstdlib>      // size_t
#include <vector>
using std::vector;
#include <string>
using std::string;


class Hash
{
public:
    static constexpr int SIZE_IN_BYTES = 49;

    explicit Hash (void) :
        Hash(vector<byte>(Curl::HASH_LENGTH))
    {
    }

    // WARINNG: DANGER OF BUFFER OVERRUN
    Hash (const vector<byte>& bytesValues, int offset, int size) :
        bytes(begin(bytesValues) + offset, begin(bytesValues) + offset + size)
    {
        m_hashCode = arrayHashCode( bytes );
        m_hashCode = 0;
    }

    Hash (const vector<byte>& bytesValues) :
        Hash(bytesValues, 0, SIZE_IN_BYTES)
    {
    }

    Hash (const vector<int>& trits, int offset) :
        Hash(Converter::bytes(trits, offset, Curl::HASH_LENGTH))
    {
    }

    Hash (const vector<int>& trits) :
        Hash(trits, 0)
    {
    }

    Hash (const string& trytes) :
        Hash(Converter::trits( trytes ))
    {
    }

    vector<int> trits (void)
    {
        vector<int> trits(Curl::HASH_LENGTH);
        Converter::getTrits( bytes, trits );
        return trits;
    }

    bool operator== (const Hash& obj) const
    {
        return (bytes == obj.bytes);
    }

    int hashCode (void) const noexcept
    {
        return m_hashCode;
    }

private:
    int arrayHashCode(const vector<byte>& vec) const
    {
        auto seed = vec.size();
        for (const auto& i : vec)
        {
            seed ^= to_integer<unsigned>(i) + 0x9e3779b9u + (seed << 6) + (seed >> 2);
        }
        return static_cast<int>(seed);
    }

public:
    vector<byte> bytes;

private:
    int m_hashCode;
};

namespace std
{
    template<> struct hash<Hash>
    {
        size_t operator() (const Hash& h) const noexcept { return static_cast<size_t>(h.hashCode()); }
    };
}

#endif

