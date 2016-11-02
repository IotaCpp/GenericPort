/* === DONE === */

#ifndef CONVERTER_H_
#define CONVERTER_H_

#include <gsl/gsl>
using gsl::byte;
using gsl::to_integer;
using gsl::to_byte;
using gsl::narrow;
#include <cassert>      // assert()
#include <cstdlib>      // abs()
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <array>
using std::array;
#include <sstream>
using std::ostringstream;
#include <algorithm>    // std::copy_n(), std::for_each()


class Converter
{
private:
    static constexpr auto RADIX = 3;
    static constexpr auto MAX_TRIT_VALUE = (RADIX - 1) / 2;
    static constexpr auto MIN_TRIT_VALUE = -MAX_TRIT_VALUE;

    static constexpr auto NUMBER_OF_TRITS_IN_A_BYTE = 5;
    static           vector<vector<int>> BYTE_TO_TRITS_MAPPINGS;

    static constexpr auto NUMBER_OF_TRITS_IN_A_TRYTE = 3;
    static           vector<vector<int>> TRYTE_TO_TRITS_MAPPINGS;
    static           string TRYTE_ALPHABET;
    static constexpr auto MIN_TRYTE_VALUE = -13;
    static constexpr auto MAX_TRYTE_VALUE = 13;

public:
    static void initStatics (void)
    {
        TRYTE_ALPHABET = "9ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        auto trits = vector<int>(NUMBER_OF_TRITS_IN_A_BYTE);

        BYTE_TO_TRITS_MAPPINGS.resize(243);
        for (auto& v : BYTE_TO_TRITS_MAPPINGS)
        {
            v.resize(NUMBER_OF_TRITS_IN_A_BYTE);
            std::copy_n( begin(trits), NUMBER_OF_TRITS_IN_A_BYTE, begin(v) );

            increment( trits, NUMBER_OF_TRITS_IN_A_BYTE );
        }

        TRYTE_TO_TRITS_MAPPINGS.resize(27);
        for (auto& v : TRYTE_TO_TRITS_MAPPINGS)
        {
            v.resize(NUMBER_OF_TRITS_IN_A_TRYTE);
            std::copy_n( begin(trits), NUMBER_OF_TRITS_IN_A_TRYTE, begin(v) );

            increment(trits, NUMBER_OF_TRITS_IN_A_TRYTE);
        }
    }

public:
    static long longValue (const vector<int>& trits, const int offset, const int size)
    {
        long value = 0;
        for (int i = size; i-- > 0; )
        {
            value = value * RADIX + trits[offset + i];
        }
        return value;
    }

    static vector<byte> bytes (const vector<int>& trits, const int offset, const int size)
    {
        vector<byte> bytes((size + NUMBER_OF_TRITS_IN_A_BYTE - 1) / NUMBER_OF_TRITS_IN_A_BYTE);

        int bytesSize = narrow<int>(bytes.size());
        for (int i = 0; i < bytesSize; ++i)
        {
            int value = 0;
            for (int j = (size - i * NUMBER_OF_TRITS_IN_A_BYTE < 5)? size - i * NUMBER_OF_TRITS_IN_A_BYTE : NUMBER_OF_TRITS_IN_A_BYTE; j-- > 0; )
            {
                value = value * RADIX + trits[offset + i * NUMBER_OF_TRITS_IN_A_BYTE + j];
            }
            bytes[i] = to_byte(narrow<unsigned char>(value));
        }

        return bytes;
    }

    static vector<byte> bytes (const vector<int>& trits)
    {
        return bytes( trits, 0, trits.size() );
    }

    static void getTrits (const vector<byte>& bytes, vector<int>& trits)
    {
        size_t offset = 0;
        for (size_t i = 0; (i < bytes.size()) && (offset < trits.size()); ++i)
        {
            size_t idx = (to_integer<int>(bytes[i]) < 0)? to_integer<int>(bytes[i]) + BYTE_TO_TRITS_MAPPINGS.size() : to_integer<int>(bytes[i]);
            auto v = BYTE_TO_TRITS_MAPPINGS[idx];

            size_t len = (trits.size() < NUMBER_OF_TRITS_IN_A_BYTE + offset)? trits.size() - offset : NUMBER_OF_TRITS_IN_A_BYTE;

            std::copy_n( begin(v), len, begin(trits) + offset);

            offset += NUMBER_OF_TRITS_IN_A_BYTE;
        }

        while (offset < trits.size())
        {
            trits[offset++] = 0;
        }
    }

    static vector<int> trits (const string& trytes)
    {
        auto trits = vector<int>(trytes.size() * NUMBER_OF_TRITS_IN_A_TRYTE);

        for (decltype(trytes.size()) i = 0; i < trytes.size(); ++i)
        {
            auto idx = TRYTE_ALPHABET.find( trytes[i] );
            assert(idx != string::npos);
            std::copy_n( begin(TRYTE_TO_TRITS_MAPPINGS[idx]), NUMBER_OF_TRITS_IN_A_TRYTE,
                         begin(trits) + (i * NUMBER_OF_TRITS_IN_A_TRYTE) );
        }

        return trits;
    }

    static void copyTrits (const long value, vector<int>& destination, const int offset, const int size)
    {
        long absoluteValue = abs( value );

        std::for_each( begin(destination)+offset, begin(destination)+offset+size, [&absoluteValue](auto& e)
        {
            int remainder = narrow<int>(absoluteValue % RADIX);
            absoluteValue /= RADIX;
            if (remainder > MAX_TRIT_VALUE)
            {
                remainder = MIN_TRIT_VALUE;
                ++ absoluteValue;
            }
            e = remainder;
        });

        if (value < 0)
        {
            std::for_each( begin(destination)+offset, begin(destination)+offset+size, [](auto& e)
            {
                e = -e;
            });
        }
    }

    static string trytes (const vector<int>& trits, const int offset, const int size)
    {
        auto trytes = ostringstream{};

        for (int i = 0; i < (size + NUMBER_OF_TRITS_IN_A_TRYTE - 1) / NUMBER_OF_TRITS_IN_A_TRYTE; ++i)
        {
            assert( in_range(0, offset + i * 3,     static_cast<int>(trits.size())-1) );
            assert( in_range(0, offset + i * 3 + 1, static_cast<int>(trits.size())-1) );
            assert( in_range(0, offset + i * 3 + 2, static_cast<int>(trits.size())-1) );
            int j = trits[offset + i * 3] + trits[offset + i * 3 + 1] * 3 + trits[offset + i * 3 + 2] * 9;
            if (j < 0)
            {
                j += TRYTE_ALPHABET.size();
            }
            trytes << TRYTE_ALPHABET[j];
        }

        return trytes.str();
    }

    static string trytes (const vector<int>& trits)
    {
        return trytes( trits, 0, trits.size() );
    }

    static int tryteValue (const vector<int>& trits, const int offset)
    {
        assert( in_range(0, offset,     static_cast<int>(trits.size())-1) );
        assert( in_range(0, offset + 1, static_cast<int>(trits.size())-1) );
        assert( in_range(0, offset + 2, static_cast<int>(trits.size())-1) );
        return trits[offset] + trits[offset + 1] * 3 + trits[offset + 2] * 9;
    }

    static void increment(vector<int>& trits, const int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (++trits[i] > MAX_TRIT_VALUE)
            {
                trits[i] = MIN_TRIT_VALUE;
            }
            else
            {
                break;
            }
        }
    }

private:
    template<typename T>
    static constexpr bool in_range(const T& lower, const T& val, const T& upper)
    {
        return ((lower <= val) && (val <= upper));
    }
};

#endif
