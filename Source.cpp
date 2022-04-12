#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include "md5.h" // NOT MY OWN WORK, THIS FILE WAS TAKEN FROM http://www.zedwood.com/article/cpp-md5-function
#include "sha256.h" // NOT MY OWN WORK, THIS FILE WAS TAKEN FROM http://www.zedwood.com/article/cpp-sha256-function

using namespace std;

// Import things we need from the standard library
using std::chrono::duration_cast;
using std::chrono::milliseconds;

// Define the alias "the_clock" for the clock type we're going to use.
typedef std::chrono::steady_clock the_clock;

// String for the found password
string FoundPassword;

const char CapitaLetters[26] =
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U',
	'V', 'W', 'X', 'Y', 'Z'
};

const char LowerCaseLetters[26] =
{
	'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u',
	'v', 'w', 'x', 'y', 'z'
};

const char Symbols[22] =
{
	'!','£','$','%','^','&','*','(',')'
	,'@','~','#','|','?','¬','`','{',
	'}','[',']',';',':'
};

const char Numbers[10] =
{
	'1','2','3','4','5','6','7','8','9','0'
};

bool found = false;

void DictionaryAttack(string filename, string inputHash, int hashChoice) {
	//Variables for the contents of the file
	string contents;

	//Opens the text file for reading
	ifstream file(filename, ios_base::binary);

	//Check to see if the Wordlist.txt file was sucessfully opened
	if (!file.good()) {
		cout << "Unable to open Text File";
		system("pause");
		exit(0);
	}
	else {
		//Takes each line in the textfile and hashes the value then compares it to the entered hash
		while (file >> contents && !found) {
			//cout << contents << endl;

			// Initialise hash variable to store the hashed attempted password
			string hash = "";
			// If the hashing algorithm choice is md5
			if (hashChoice == 1) {
				hash = md5(contents);
			}

			// If the hashing algorithm choice is sha256
			else if (hashChoice == 2) {
				hash = sha256(contents);
				//cout << hash << endl;
			}
			// Checks to see if the current hashed attempted password is equal to the inputed hash
			if (hash == inputHash) {
				cout << endl << hash << endl;
				cout << "Cracked Password: " << contents << endl;
				FoundPassword = contents;
				found = true;
				//cout << endl;
				//cout << "HASH : " << hash << endl;
			}
		}
	}
	//Closes the input textfile
	file.close();
	return;
}

void BruteForce(unsigned int length, string s, string inputHash, int hashChoice) {
	string append;
	if (found == false) {
		if (length == 0)
		{
			//cout << s << endl; // print out attempting
			string hash;
			hash = md5(s);

			if (hash == inputHash) {
				found = true;
				cout << endl;
				cout << "HASH : " << hash << endl;
				cout << "Cracked Password: " << s << endl;
			}
			return;
		}

		for (unsigned int i = 0; i < 26; i++) // iterate through alphabet
		{
			// Append new character onto the string
			// Recursively call function again untill string has reached its length
			append = s + LowerCaseLetters[i];
			BruteForce(length - 1, append, inputHash, hashChoice);
		}

		for (int j = 0; j < 26; j++) {
			append = s + CapitaLetters[j];
			BruteForce(length - 1, append, inputHash, hashChoice);
		}

		for (int x = 0; x < 22; x++) {
			append = s + Symbols[x];
			BruteForce(length - 1, append, inputHash, hashChoice);
		}

		for (int y = 0; y < 10; y++) {
			append = s + Numbers[y];
			BruteForce(length - 1, append, inputHash, hashChoice);
		}
	}
	else {
		return;
	}
	
}

// Simple function to check if an integer is within a certain range
int validateInt(int min, int max, int choice) {
	while (choice < min || choice > max) {
		cout << "Invalid Input" << endl;
		cout << "Please enter a value from the range above: ";
		cin >> choice;
		cout << endl;
	}
	return choice;
}

// Recieve the choice of which hashing algorithm the password was hashed with
int RecieveChoiceInput() {
	int choice;
	cout << "Please enter the type of hash you are wanting to crack" << endl << "1:MD5" << endl << "2:sha256" << endl;
	cin >> choice;
	cout << endl;
	validateInt(1, 2, choice);
	return choice;
}

// Recieve the hash that is trying to be cracked
string RecieveHashInput() {
	string hash;
	cout << "Please enter the hash you would like to crack: ";
	cin >> hash;
	cout << endl;
	return hash;
}

string RecieveFilename() {
	string filename;
	cout << "Please enter the filename for the disctionart attack with the correct extension: ";
	cin >> filename;
	cout << endl;
	return filename;
}

int ChooseFunction() {
	int choice;
	cout << "Please choose the type of attack you would like to carry out: " << endl << "1 - Brute Force" << endl << "2 - Dictionary" << endl;
	cin >> choice;
	validateInt(1, 2, choice);
	return choice;
}

int main()
{
	string filename;

	// Choice of which hash is being used
	int hashChoice = 0;

	// The inputed hash that is the target goal to crack
	string inputHash = "";

	int functionChoice;
	int stringlength = 1;

	hashChoice = RecieveChoiceInput();
	inputHash = RecieveHashInput();
	functionChoice = ChooseFunction();
	the_clock::time_point start = the_clock::now();
	if (functionChoice == 1) {
		cout << "Initialising Brtue Force password crack..." << endl;
		while (found == false) {
			BruteForce(stringlength, "", inputHash, hashChoice);
			stringlength++;
		}
	}
	else {
		filename = RecieveFilename();
		cout << "Initialising Dictionary password crack..." << endl;
		while (found == false) {
			DictionaryAttack(filename, inputHash, hashChoice);
			stringlength++;
		}
	}
	the_clock::time_point end = the_clock::now();
	if (found == true) {
		// Compute the difference between the two times in milliseconds
		auto time_taken = duration_cast<milliseconds>(end - start).count();
		cout << "Cracking the password took " << time_taken << " ms." << endl;
	}
	
	system("pause");
	return 0;
}