#include "config.h"

// declairations of functions uesd
struct crypt_data {
	unsigned char key[KEY_LEN];
	unsigned char iv[IV_LEN];
};

void encrypt(const crypt_data* data, string path);
void decrypt(const crypt_data* data, string path);
crypt_data* generatekey();

////////////////////////////////////////////////
// encrypt function
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

	ofstream ofile((path + LOCKED_EXTENSION).c_str(), ios::binary);
	ofile.write(cipher.c_str(), cipher.length());
	ofile.close();
}
////////////////////////////////////////////////

////////////////////////////////////////////////
// generatr key
crypt_data* generatekey() {
	crypt_data* data = new crypt_data;

	AutoSeededRandomPool rng;

	rng.GenerateBlock(data->key, sizeof(data->key));
	rng.GenerateBlock(data->iv, sizeof(data->iv));

	return data;
}
////////////////////////////////////////////////

////////////////////////////////////////////////
// decrypt function
void decrypt(const crypt_data* data, string path) {
	string decrypt;
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

	CBC_Mode<AES>::Decryption dk;
	dk.SetKeyWithIV(data->key, sizeof(data->key), data->iv);

	StreamTransformationFilter filter(dk);
	filter.Put((const unsigned char*) plain.data(), plain.size());
	filter.MessageEnd();

	const size_t ret = filter.MaxRetrievable();
	decrypt.resize(ret);
	filter.Get((unsigned char*) decrypt.data(), decrypt.size());

#ifdef DEBUG
	string ciphertext;
	StringSource(cipher, true, new HexEncoder(new StringSink(ciphertext)));
	cout << "Ciphertext:\t" << ciphertext << endl;
#endif
	// get original name of file
	int originEnd = path.rfind('.');
	string originFile = path.substr(0,originEnd);

	ofstream ofile(originFile, ios::binary);
	ofile.write(decrypt.c_str(), decrypt.length());
	ofile.close();
}
////////////////////////////////////////////////
