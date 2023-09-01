// cocos2d xxtea decryptor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "xxtea.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <direct.h>
#include <functional>
#include "dirent.h"

static std::vector<char> ReadAllBytes(char const* filename)
{
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    if (pos == 0) {
        return std::vector<char>{};
    }

    std::vector<char>  result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(result.data(), pos);

    return result;
}
int isDir(char* path)
{
	struct stat s;
	if( stat(path,&s) == 0 )
	{
		if( s.st_mode & S_IFDIR )
		{
			return 1;
		}
		else if( s.st_mode & S_IFREG )
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
void listFiles(const std::string& path, std::function<void(const std::string&)> cb) {
	if (auto dir = opendir(path.c_str())) {
		while (auto f = readdir(dir)) {
			if (!f->d_name || f->d_name[0] == '.') continue;
			if (f->d_type == DT_DIR)
				listFiles(path + f->d_name + "\\", cb);

			if (f->d_type == DT_REG)
				cb(path + f->d_name);
		}
		closedir(dir);
	}
}
int makePath(const std::string& path)
{
#if defined(_WIN32)
	int ret = _mkdir(path.c_str());
#else
	mode_t mode = 0755;
	int ret = mkdir(path.c_str(), mode);
#endif
	if (ret == 0)
		return 1;

	switch (errno)
	{
	case ENOENT:
		// parent didn't exist, try to create it
	{
		int pos = path.find_last_of('/');
		if (pos == std::string::npos)
#if defined(_WIN32)
			pos = path.find_last_of('\\');
		if (pos == std::string::npos)
#endif
			return false;
		if (!makePath(path.substr(0, pos)))
			return false;
	}
	// now, try to create again
#if defined(_WIN32)
	return 0 == _mkdir(path.c_str());
#else 
	return 0 == mkdir(path.c_str(), mode);
#endif

	case EEXIST:
		// done!
		return isDir(const_cast<char*>(path.c_str()));
	default:
		return false;
	}
}
int main(int argc, char* argv[]){
	if(argc != 4)
	{
		printf("Usage: %s [key] [sign] [input file/dir]\n", std::string(argv[0]).substr(std::string(argv[0]).find_last_of("/\\") + 1).c_str());
		return -1;
	}
	std::string key = argv[1];
	std::string sign = argv[2];
	std::string path = argv[3];
	const int res = isDir(const_cast<char*>(path.c_str()));
	const std::string dir = std::string(path).substr(0, std::string(path).find_last_of("/\\") + 1);
	if(res == 1)
	{
		if(path.back() != '\\')
		{
			path.append("\\");
		}
		listFiles(path, [sign, key, path, dir](const std::string& path2) {
			size_t len;
			std::vector<char> dataOri = ReadAllBytes(path2.c_str());
			std::vector<char> data = dataOri;
			data.erase(data.begin(), data.begin() + static_cast<long>(strlen(sign.c_str())));
			const char* decryptedData = static_cast<char*>(xxtea_decrypt(data.data(), data.size(), key.c_str(), &len));
			const std::string outputPath = dir + "\out";
			std::string temp = path2.substr(path.size());
			if(temp.back() == 'c')
			{
				temp.pop_back();
			}
			std::string outputFileName = outputPath + "\\" + temp;
			std::string path3 =  std::string(outputFileName).substr(0,std::string(outputFileName).find_last_of("/\\"));
			if (isDir(const_cast<char*>(path3.c_str())) == -1)
			{
				
				if (makePath(path3) != 1)
				{
					std::cout << "Failed to create output directory: " << path3 << std::endl;
					return -1;

				}
			}
			if (decryptedData != nullptr)
			{
				std::ofstream out;
				out.open(outputFileName);
				out.write(decryptedData, len);
				out.close();
				std::cout << "Decrypted: " << path2 << std::endl;
			}
			else
			{
				std::ofstream out;
				out.open(outputFileName);
				out.write(dataOri.data(), dataOri.size());
				out.close();
				std::cout << "Failed to decrypt: " << path2 << std::endl;
			}
		});
	}
	else if(res == 0)
	{
		std::vector<char> data = ReadAllBytes(path.c_str());
		data.erase(data.begin(), data.begin() + static_cast<long>(strlen(sign.c_str())));
		size_t len;
		const char* decryptedData = static_cast<char*>(xxtea_decrypt(data.data(), data.size(), key.c_str(), &len));
		if (decryptedData != nullptr)
		{
			const std::string fileName = std::string(path).substr(std::string(path).find_last_of("/\\") + 1);
			const std::string outputPath = dir + "\out\\";
			if(isDir(const_cast<char*>(outputPath.c_str())) == -1)
			{
				if(_mkdir(outputPath.c_str()) == -1)
				{
					std::cout << "Failed to create output directory" << std::endl;
					return -1;
				}
			}
			const std::string outputFileName = outputPath + fileName.substr(0, fileName.size() - 1);
			std::ofstream out;
			out.open(outputFileName);
			out.write(decryptedData, len);
			out.close();
			std::cout << "Decrypted: " << path << std::endl;
		}
		else
		{
			std::cout << "Failed to decrypt: " << path << std::endl;
		}
	}
    else
    {
        std::cout << "Failed to process input" << std::endl;
        return -1;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
