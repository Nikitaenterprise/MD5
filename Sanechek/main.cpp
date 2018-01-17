#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <vector>
#include "sha256.h"

void start(const std::string &correctHash, std::string &newPass, const std::string &correctPass, const std::vector<int> &diapasone)
{
	char *alph = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string str = correctPass;

	for (int id = diapasone[0]; id < diapasone[1]; id++)
	{
		str[0] = alph[id % 62];
		str[1] = alph[(id / 62) % 62];
		str[2] = alph[(id / 62 / 62) % 62];
		std::cout << str << std::endl;
		std::string newHash = sha256(str);
		if (newHash == correctHash)
		{
			newPass = str;
			break;
		}
		newHash.clear();
	}
}

void sha256Main(int id, bool &good)
{
	std::string correctPass = "bbb";
	std::string correctHash = sha256(correctPass);
	std::string newPass;
	int maxVariantsOfPasswords = 62 * 62 * 62; //238328 variants for password length of 3
	std::vector<int> diapasone;
	diapasone.push_back(id*maxVariantsOfPasswords / 4);
	diapasone.front();
	diapasone.push_back((id+1)*maxVariantsOfPasswords / 4);

	start(std::ref(correctHash), std::ref(newPass), std::ref(correctPass), diapasone);
	if (newPass == correctPass)
	{
		good = true;
		std::cout << "We found pass" << std::endl;
		std::cout << "Password = " << newPass << std::endl;
		std::cout << "Hash = " << correctHash << std::endl;
		CreateMutexA(NULL, FALSE, "bhikvgufct");
	}
}

int main(int argc, char *argv[])
{
	STARTUPINFOA si[4];
	PROCESS_INFORMATION pi[4];
	for (int i = 0; i < 4; i++)
	{
		ZeroMemory(&si[i], sizeof(si[i]));
		si[i].cb = sizeof(si[i]);
		ZeroMemory(&pi[i], sizeof(pi[i]));
	}

	if (argc == 1)
	{
		std::cout << "Parrent process\n";
		for (int i = 0; i < 4; i++)
		{
			char p[256];
			sprintf_s(p, "%s %i", argv[0], i);
			std::cout << p << std::endl;
			CreateProcessA("D:/Ó÷¸áà/C++/MD5/Debug/Sanechek Tlhelp32.exe", p, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]);
		}
		while (!OpenMutexA(MUTEX_ALL_ACCESS, false, "bhikvgufct")) Sleep(5);
		for (int i = 0; i < 4; i++)
			TerminateProcess(pi[i].hProcess, 0);
		std::cout << "Success!\n" << std::endl;
	}
	else
	{
		std::cout << "Child process\n";
		std::cout << "argc = " << argc << std::endl;
		std::cout << "argv[0] = " << argv[0] << std::endl;
		std::cout << "argv[1] = " << argv[1] << std::endl;
		std::cout << "argv[2] = " << argv[2] << std::endl;
		//std::cout << "argv[3] = " << argv[3] << std::endl;
		bool good = false;
		int id = atoi(argv[2]);
		std::vector<int> diapasone;
		std::cout << "id = " << id << std::endl;
		sha256Main(id, good);
	}

	system("pause");
	return 0;
}


