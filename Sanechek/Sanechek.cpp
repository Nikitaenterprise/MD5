#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "sha256.h"
#include <thread>
#include <vector>

void start(const std::string &correctHash, std::string &newPass, const std::string &correctPass, int id)
{
	//std::cout << "I`m start\n";
	char *alph = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string str = correctPass;
	//std::cout << correct_hash << std::endl;

	str[0] = alph[id % 62];
	str[1] = alph[(id / 62) % 62];
	str[2] = alph[(id / 62 / 62) % 62];
	//std::cout << "str = " << str << std::endl;
	std::string newHash = sha256(str);
	if (newHash == correctHash)
	{
		newPass = str;
	}
}

void sha256Main(int id, int &good)
{
	//std::cout << "I`m sha main\n";
	//std::cout << "The good is " << good << std::endl;
	std::string correctPass = "bbb";
	std::string correctHash = sha256(correctPass);
	std::string newPass;
	start(std::ref(correctHash), std::ref(newPass), std::ref(correctPass), id);
	if (newPass == correctPass)
	{
		good = 1;
		std::cout << "We found pass" << std::endl;
		std::cout << "Password = " << newPass << std::endl;
		std::cout << "Hash = " << correctHash << std::endl;
		std::cout << "id = " << id << std::endl;
		//std::cout << "Now the good is " << good << std::endl;
	}
}

int main(int argc, char *argv[])
{
	std::vector<std::thread> threadVector;
	int good = 0;
	for (int id = 0; id < 62 * 62 * 62; id++)
	{
		//std::cout << "The main good is "<< good << std::endl;
		std::thread th(sha256Main, id, std::ref(good));
		threadVector.push_back(move(th));
		//std::cout << i << std::endl;
		if (good == 1) break;
	}
	std::cout << "Hi\n";
	for (auto it = threadVector.begin(); it != threadVector.end(); it++)
	{
		(*it).join();
	}
	system("pause");
	return 0;
}