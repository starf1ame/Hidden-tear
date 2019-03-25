#include <iostream>
#include <cryptopp/aes.h>

#include <cryptopp/default.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/ccm.h>

#include <boost/filesystem/operations.hpp>
#include <iostream>

using namespace CryptoPP;
using namespace boost::filesystem;
using namespace std;

// #define KEY_LEN 32 // 256 bits
#define KEY_LEN AES::DEFAULT_KEYLENGTH
#define IV_LEN CryptoPP::AES::BLOCKSIZE
#define LOCKED_EXTENSION ".locked"

struct crypt_data {
	unsigned char key[KEY_LEN];
	unsigned char iv[IV_LEN];
};

void encrypt(const crypt_data* data, string path);
crypt_data* generatekey();

int main()
{
    //AES中使用的固定参数是以类AES中定义的enum数据类型出现的，而不是成员函数或变量
    //因此需要用::符号来索引
    cout << "AES Parameters: " << endl;
    cout << "Algorithm name : " << AES::StaticAlgorithmName() << endl; 

    //Crypto++库中一般用字节数来表示长度，而不是常用的字节数
    cout << "Block size : " << AES::BLOCKSIZE * 8 << endl;
    cout << "Min key length : " << AES::MIN_KEYLENGTH * 8 << endl;
    cout << "Max key length : " << AES::MAX_KEYLENGTH * 8 << endl;

    //AES中只包含一些固定的数据，而加密解密的功能由AESEncryption和AESDecryption来完成
    //加密过程
    AESEncryption aesEncryptor; //加密器 

    unsigned char aesKey[AES::DEFAULT_KEYLENGTH]; //密钥
    unsigned char inBlock[AES::BLOCKSIZE] = "123456789"; //要加密的数据块
    unsigned char outBlock[AES::BLOCKSIZE]; //加密后的密文块
    unsigned char xorBlock[AES::BLOCKSIZE]; //必须设定为全零

    memset( xorBlock, 0, AES::BLOCKSIZE ); //置零

    aesEncryptor.SetKey( aesKey, AES::DEFAULT_KEYLENGTH ); //设定加密密钥
    aesEncryptor.ProcessAndXorBlock( inBlock, xorBlock, outBlock ); //加密

    //以16进制显示加密后的数据
    for( int i=0; i<16; i++ ) {
        cout << hex << (int)outBlock[i] << " ";
    }
    cout << endl;

    //解密
    AESDecryption aesDecryptor;
    unsigned char plainText[AES::BLOCKSIZE];

    aesDecryptor.SetKey( aesKey, AES::DEFAULT_KEYLENGTH );
    aesDecryptor.ProcessAndXorBlock( outBlock, xorBlock, plainText );

    for( int i=0; i<16; i++ ) { 
        cout << plainText[i]; 
    }
    cout << endl;

    // encrypt a file
    crypt_data* d = generatekey();
    cout<<"Start encrpt a file...\n";
    class path pwd_path = current_path(); 
    class path encryptFile = pwd_path / "test_file" / "README.md";
    cout<<"The file path is : "<<encryptFile.c_str()<<endl;

    encrypt(d, encryptFile.c_str());
    cout<<"Successfully encrypt...";
    return 0;
}


// defination of function used

void encrypt(const crypt_data* d, string path) {
	string cipher;
	string plain;

    // pipeling, change file into string that all data can be pumped at once
	FileSource(path.c_str(), true, new StringSink(plain));

#ifdef DEBUG
	// Print key and initialization vector
	string skey;
	StringSource(d->key, sizeof(d->key), true, new HexEncoder(new StringSink(skey)));
	cout << "Key:\t\t" << skey << endl;
	skey.clear();

	string siv;
	StringSource(d->iv, sizeof(d->iv), true, new HexEncoder(new StringSink(siv)));
	cout << "IV:\t\t" << siv << endl;
	siv.clear();

	cout << "Plaintext:\t" << plain << endl;
#endif

	CBC_Mode<AES>::Encryption e;
	e.SetKeyWithIV(d->key, sizeof(d->key), d->iv);

	StreamTransformationFilter filter(e);
	filter.Put((const unsigned char*) plain.data(), plain.size());
	filter.MessageEnd();

	const size_t ret = filter.MaxRetrievable();
	cipher.resize(ret);
	filter.Get((unsigned char*) cipher.data(), cipher.size());

#ifdef DEBUG
	string ciphertext;
	StringSource(cipher, true, new HexEncoder(new StringSink(ciphertext)));
	cout << "Ciphertext:\t" << ciphertext << endl;
#endif

	ofstream ofile((path + LOCKED_EXTENSION).c_str(), std::ios::binary);
	ofile.write(cipher.c_str(), cipher.length());
	ofile.close();
}

crypt_data* generatekey() {
	crypt_data* d = new crypt_data;

	AutoSeededRandomPool prng;

	prng.GenerateBlock(d->key, sizeof(d->key));
	prng.GenerateBlock(d->iv, sizeof(d->iv));

	return d;
}
