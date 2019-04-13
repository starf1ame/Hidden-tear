#include "config.h"

// declairations of functions uesd
struct crypt_data {
	unsigned char key[KEY_LEN];
	unsigned char iv[IV_LEN];
	string hexKey;
	string hexIV;
};

void encrypt(const crypt_data* data, string path);
void decrypt(const crypt_data* data, string path);
crypt_data* generatekey();
void hex_to_byte_key(crypt_data* data);

////////////////////////////////////////////////
// generatr key
crypt_data* generatekey() {
	crypt_data* data = new crypt_data;

	AutoSeededRandomPool rng;
	rng.GenerateBlock(data->key, sizeof(data->key));
	rng.GenerateBlock(data->iv, sizeof(data->iv));

	//-----------encode to get hex of key------------//
	string encoded_1;
	HexEncoder encoder_1;
	encoder_1.Put(data->key, sizeof(data->key));
	encoder_1.MessageEnd();

	word64 size_1 = encoder_1.MaxRetrievable();
	if(size_1)
	{
    	encoded_1.resize(size_1);		
    	encoder_1.Get((unsigned char*)&encoded_1[0], encoded_1.size());
	}
	cout << "testKey:\t\t" << encoded_1 << "len:\t:"<<encoded_1.size()<<" "<<sizeof(encoded_1)<<endl;
	data->hexKey = encoded_1;

	//-----------encode to get hex of iv------------//
	string encoded_2;
	HexEncoder encoder_2;
	encoder_2.Put(data->iv, sizeof(data->iv));
	encoder_2.MessageEnd();

	word64 size_2 = encoder_2.MaxRetrievable();
	if(size_2)
	{
    	encoded_2.resize(size_2);		
    	encoder_2.Get((unsigned char*)&encoded_2[0], encoded_2.size());
	}
	data->hexIV = encoded_2;

	return data;
}
////////////////////////////////////////////////

////////////////////////////////////////////////
// transfer hex key to byte key function
void hex_to_byte_key(crypt_data* data){
	//-------------verify decoding of key-----------------//
	string decoded_1;
	StringSource ss(data->hexKey, true, new HexDecoder(new StringSink(decoded_1)));

	for(int i=0;i<KEY_LEN;i++) {
		if (decoded_1[i]<0) data->key[i]=decoded_1[i]+256;
		else data->key[i]=decoded_1[i];
	} 
	//-------------verify decoding of key-----------------//
	string decoded_2;
	StringSource ss_2(data->hexIV, true, new HexDecoder(new StringSink(decoded_2)));

	for(int i=0;i<KEY_LEN;i++) {
		if (decoded_2[i]<0) data->iv[i]=decoded_2[i]+256;
		else data->iv[i]=decoded_2[i];
	} 

}

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
	string dkey;
	StringSource(d->key, sizeof(d->key), true, new HexEncoder(new StringSink(skey)));
	// StringSource(skey, sizeof(skey), true, new HexDecoder(new StringSink(dkey)));
	cout << "Key:\t\t" << skey << endl;
	string encoded;

	HexEncoder encoder;
	encoder.Put(d->key, sizeof(d->key));
	encoder.MessageEnd();

	word64 size = encoder.MaxRetrievable();
	if(size)
	{
    	encoded.resize(size);		
    	encoder.Get((unsigned char*)&encoded[0], encoded.size());
	}
	cout << "testKey:\t\t" << encoded << endl;
	skey.clear();

	//--------------------iv--------------------//
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
