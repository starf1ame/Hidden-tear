#include <boost/filesystem/operations.hpp>
#include <cryptopp/default.h>
#include <iostream>

using namespace CryptoPP;
using namespace boost::filesystem;
using namespace std;

int main()
{
    class path pwd_path = current_path(); 
    cout<< pwd_path<<endl;
    class path parent_path = pwd_path.parent_path();
    cout << parent_path<<endl;
    boost::filesystem::path file_path = pwd_path / "file"; //path支持重载/运算符
    cout << file_path<<endl;

    if(boost::filesystem::exists(file_path))  //推断文件存在性  
	{  
		string strPath = file_path.string();
		cout<< strPath;
	} 
	else 
	{  
		//文件夹不存在;   
		// create_directory(file_path);  //文件夹不存在。创建 
        cout<<"file path doesn't exist"<<endl;
	}  
	bool bIsDirectory = is_directory(file_path); //推断file_path是否为文件夹
	recursive_directory_iterator beg_iter(pwd_path);
	recursive_directory_iterator end_iter;
	for (; beg_iter != end_iter; ++beg_iter)
	{
		if (is_directory(*beg_iter))
		{
			cout<<current_path()<<endl;
            continue;
		}
		else
		{	
			string strPath = beg_iter->path().string();  //遍历出来的文件名称
			cout<<strPath<<endl;
		}
	}
	// boost::filesystem::path new_file_path = file_path / "test.txt";
	// if(boost::filesystem::is_regular_file(new_file_path))	//推断是否为普通文件
	// {  
	// 	UINT sizefile = boost::filesystem::file_size(new_file_path);  //文件大小(字节)
	// 	int x =1;
	// }   
	// boost::filesystem::remove(new_file_path);//删除文件new_file_path

}
