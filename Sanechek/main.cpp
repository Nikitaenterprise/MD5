#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include "sha256.h"
#include <sstream>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <thread>
#include <vector>

using std::string;
using std::cout;
using std::endl;

void start(std::string correct_hash, std::string correct_pass, int id, bool good)
{
	char *alph = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string str;

	str[0] = alph[id % 62];
	str[1] = alph[(id / 62) % 62];
	str[2] = alph[(id / 62 / 62) % 62];

	std::string newHash = sha256(str);
	if (newHash == correct_hash)
	{
		correct_pass = str;
		good = true;
	}
}

void sha256Main(int id, bool good)
{
	std::string input = "aaa";
	std::string crackhash = sha256(input);
	std::string newPass;
	start(crackhash, newPass, id, good);
	if (newPass == input)
	{
		std::cout << "id =" << id;
		good = true;
	}
}

int main(int argc, char *argv[])
{
	std::vector<std::thread> threadVector;
	bool good = false;
	for (int i = 0; i < 62; i++)
	{
		std::thread th(sha256Main, i, good);
		threadVector.push_back(move(th));
		//std::cout << i << std::endl;
		if (good == true) break;
	}
	std::cout << "Hi\n";
	for (auto it = threadVector.begin(); it != threadVector.end(); it++)
	{
		(*it).join();
	}
	system("pause");
	return 0;
}



