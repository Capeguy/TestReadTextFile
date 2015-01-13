#include <iostream>
#include <fstream>		//File reading
#include <iomanip>		//std::setw
#include "List.h"
#include "Music.h"
#include <Windows.h>	//Console Utility
#include <ctime>		//Elapsed Time

using namespace std;

int musicInfoFileLength = 779074;

/*
Parses a music info string and return a music object from it
@param music Music Info String
@return Music Object
*/
Music parseMusicItem(string music){
	string parsed[6];
	
	string tmpToken;
	size_t pos = 0;
	string delimiter = "<SEP>";
	int column = 0;

	while ((pos = music.find(delimiter)) != string::npos) {
			tmpToken = music.substr(0, pos);
		parsed[column] = tmpToken;
		music.erase(0, pos + delimiter.length());
		column++;
	}
	tmpToken = music;
	parsed[5] = tmpToken;

	Music musicResult(parsed[0], parsed[1], parsed[2], parsed[3], parsed[4], parsed[5]);

	return musicResult;
}

/* An Inline Progress Bar
@param x Current Index
@param n Total Number of Progress
@param w Width of the bar
@param beginClock Start of elapsed time
*/
static inline void loadbar(unsigned int x, unsigned int n, clock_t beginClock, unsigned int w = 20)
{
	if ((x != n) && (x % (n / 100 + 1) != 0) && n >= 2000) return;

	//Get Elapsed Time
	clock_t endClock = clock();
	double elapsedSec = double(endClock - beginClock) / CLOCKS_PER_SEC;

	float ratio = x / (float)n;
	int   c = ratio * w;

	cout << setw(3) << "Parsed: " << x << "/" << n << " [";
	for (int x = 0; x<c; x++) cout << "=";
	for (int x = c; x<w; x++) cout << " ";
	cout << "] " << (int)(ratio * 100) << "% Time Elapsed: " << setprecision(2) << fixed << elapsedSec;
	cout << " sec\r" << flush;
}

/*
Reads the Text File
@param &list Linked list to store the music data lines in
@param count How many lines in the text file to process
*/
void readMatchFile(List &list, int count){
	bool verboseMode = false; //Enable Verbose Mode

	ifstream file("mxm_779k_matches.txt");
	string str;
	int internalCounter = 0;
	int progressCounter = count;
	if (count > musicInfoFileLength){
		cout << "Lines to read specified exceeds lines in file. Defaulting to read all" << endl;
		count = -1;
	}
	if (count == -1){
		progressCounter = musicInfoFileLength;
		cout << "As the file is extremely large, this may take a couple of minutes..." << endl;
	}
	cout << "===============" << endl;
	cout << "Reading file..." << endl;
	cout << "===============" << endl << endl;
	clock_t beginClock = clock();
	while (getline(file, str)){
		if (internalCounter >= progressCounter)
			break;
		if (verboseMode)
			cout << str << endl; 

		//Check if string is a comment
		if (str[0] == '#'){
			//cout << "DEBUG: IS COMMENT" << endl;
		} else {
			//Parse Music Details Line
			list.add(str);
		}
		loadbar(internalCounter, progressCounter, beginClock);
		//Increment counter
		internalCounter++;
	}

	loadbar(progressCounter, progressCounter, beginClock);
	cout << endl << "Finished Reading and Adding File..." << endl;
	cout << "Total Lines Read: " << internalCounter << endl;
	cout << "Total Music List Length: " << list.getLength() << endl << endl;
}

/*
Prints out the main menu
*/
void mainMenu(){
	cout << "=======================" << endl;
	cout << "       Main Menu" << endl;
	cout << "=======================" << endl;
	cout << "1) View Songs in Database" << endl;
	cout << "2) Search for a song in database with name" << endl;
	cout << "0) Quit" << endl;
}

/*
Prints out a song's information
@param musicData The Song Info Object
*/
void printMusicInfo(Music musicData){
	cout << "Million Song Track DB ID: " << musicData.getTid() << endl;
	cout << "Million Song Track Artist: " << musicData.getTArtist() << endl;
	cout << "Million Song Track Title: " << musicData.getTTitle() << endl;
	cout << "musicXmatch Track DB ID: " << musicData.getMid() << endl;
	cout << "musicXmatch Track Artist: " << musicData.getMArtist() << endl;
	cout << "musicXmatch Track Title: " << musicData.getMTitle() << endl;
}

/*
Option 2 : Search for a song
@param &list Linked List of the songs
*/
void searchSong(List &list){
	cout << "=========================" << endl;
	cout << "     List Songs in DB" << endl;
	cout << "=========================" << endl;
	string target;
	string empty;
	getline(cin, empty);
	cout << "Enter Exact Song Name: ";
	getline(cin, target);

	for (int i = 1; i <= list.getLength(); i++){
		string res = list.get(i);
		Music musIfo = parseMusicItem(res);
		if (musIfo.getMTitle() == target){
			cout << endl << "Music Found! Details of the music file is found below:" << endl;
			printMusicInfo(musIfo);
			cout << endl;
			break;
		}
	}
}


/*
Option 1 : List of all songs currently in the linked list
@param &list Linked List of the songs
*/
void listAllSongs(List &list){
	cout << "=========================" << endl;
	cout << "     List Songs in DB" << endl;
	cout << "=========================" << endl;

	//cout << "ITEMS IN LIST " << endl;
	//list.print();
	//cout << "=============" << endl;
	//cout << "DEBUG SIZE: " << list.getLength() << endl;
	for (int i = 1; i <= list.getLength(); i++){
		string res = list.get(i);
		//cout << "DEBUG STR OF INDEX " << i << ": " << res << endl;
		Music musIfo = parseMusicItem(res);
		cout << "=========================================================" << endl;
		cout << "                 Song " << i << endl;
		cout << "=========================================================" << endl;
		printMusicInfo(musIfo);
		cout << "=========================================================" << endl;
	}
}


/*
Main Method
@return End Error Code
*/
int main(){
	SetConsoleTitle(TEXT("Read File Test Project"));
	List mainList;
	cout << "How many lines to read? (-1 to read all): ";
	int count;
	cin >> count;
	readMatchFile(mainList, count);

	if (mainList.getLength() == 0){
		cout << "As Database do not have any music item, the program will quit." << endl;
		return 0;
	}

	//Main Menu
	while (true) {
		mainMenu();
		int selection;
		cin >> selection;
		switch (selection)
		{
		case 1: listAllSongs(mainList); break;
		case 2: searchSong(mainList); break;
		case 0: return 0;
		//case 4: mainList.print(); break;
		default: cout << "Invalid Selection." << endl; break;
		}
	}
	return 0;
}