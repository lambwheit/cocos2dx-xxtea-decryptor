// cocos2d xxtea decryptor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "xxtea.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <direct.h>

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

int main(int argc, char* argv[]){
	if(argc != 4)
	{
		printf("Usage: %s [key] [sign] [input file/dir]\n", std::string(argv[0]).substr(std::string(argv[0]).find_last_of("/\\") + 1).c_str());
		return -1;
	}
	char* key = argv[1];
	char* sign = argv[2];
	char* path = argv[3];
	const int res = isDir(path);
	if(res == 1)
	{
		
	}
	else if(res == 0)
	{
		std::vector<char> data = ReadAllBytes(path);
		data.erase(data.begin(), data.begin() + static_cast<long>(strlen(sign)));
		size_t len;
		const char* decryptedData = static_cast<char*>(xxtea_decrypt(data.data(), data.size(), key, &len));
		if (decryptedData != nullptr)
		{
			const std::string dir = std::string(path).substr(0, std::string(path).find_last_of("/\\") + 1);
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
			std::cout << "Decrypted " << path << std::endl;
		}
		else
		{
			std::cout << "Failed to decrypt " << path << std::endl;
			return -1;
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
