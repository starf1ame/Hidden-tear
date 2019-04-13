#include "config.h"

void Traversal ( vector<string> & files, bool mode );
string getSuffix(string strPath);
string getFilename(string strPath);

////////////////////////////////////////////////
// traversal for filepaths
void Traversal ( vector<string> & files, bool mode )
{
    class path pwd_path = current_path(); 
    cout<< pwd_path<<endl;
	class path test_file_path = pwd_path / "test_file";
	recursive_directory_iterator beg_iter(test_file_path);
	recursive_directory_iterator end_iter;
	cout << "start walking...\n";
	for (; beg_iter != end_iter; ++beg_iter) {
        string strPath = beg_iter->path().string();
		if ((getFilename(strPath)=="/.DS_Store")or(getFilename(strPath)=="/.DS_Store.locked")) continue;
		if (is_directory(*beg_iter)) {
			cout<<current_path()<<endl;
            continue;
		}
		else if ( (getSuffix(strPath) != ".locked") and (mode) )
		{	
            files.push_back(strPath);
		}
		else if ( (getSuffix(strPath) == ".locked") and not(mode) )
		{	
            files.push_back(strPath);
		}
        cout<<"view file: "<<strPath<<endl;
	}
	cout << "finish walking.\n";
	for (int i=0;i<files.size();i++)
		cout<<getFilename(files[i])<<"; ";
	cout<<endl;
}
////////////////////////////////////////////////

////////////////////////////////////////////////
// get filename and suffix of target file
string getSuffix(string strPath)
{
    int dotNum = strPath.rfind('.');
    int suffixLen = strPath.length()-dotNum;
    string suffix = strPath.substr(dotNum,suffixLen);
    return suffix;
}
string getFilename(string strPath)
{
	int nameStart = strPath.rfind('/');
	int nameLen = strPath.length()-nameStart;
	string fileName = strPath.substr(nameStart,nameLen);
	return fileName;
}
////////////////////////////////////////////////