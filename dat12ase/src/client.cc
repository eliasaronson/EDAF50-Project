#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

string help = 
	"-Helpmessage-\n"
	"Input\tAction\n"
	"1\tList Newsgroups\n"
	"2\tCreate Newsgroup\n"
	"3\tDelete Newsgroup\n"
	"4\tList Articles in Newsgroup\n"
	"5\tCreate Article\n"
	"6\tDelete Article\n"
	"7\tGet Article\n\n"
	"9\tView Help\n"
	"0\tExit\n";

void list_ng(MessageHandler &mess);
void create_ng(MessageHandler &mess);
void delete_ng(MessageHandler &mess);
void list_art(MessageHandler &mess);
void create_art(MessageHandler &mess);
void delete_art(MessageHandler &mess);
void get_art(MessageHandler &mess);

/* Creates a client for the given args, if possible.
 * * Otherwise exits with error code.
 * */
Connection init(int argc, char* argv[])
{
		if (argc != 3) {
				cerr << "Usage: myclient host-name port-number" << endl;
				exit(1);
		}
		int port = -1;
		try {
				port = stoi(argv[2]);
		} catch (exception& e) {
				cerr << "Wrong port number. " << e.what() << endl;
				exit(2);
		}
		Connection conn(argv[1], port);
		if (!conn.isConnected()) {
				cerr << "Connection attempt failed" << endl;
				exit(3);
		}
		return conn;
}

int app(Connection& conn)
{
	MessageHandler mess(conn);
	int input;

	cout << help << endl;
	cout << endl << "Your input: ";
	while (true)
	{
		if (cin >> input)
		{	
			cin.ignore(1024, '\n'); // flush cin
			try
			{
				switch(input)
				{
					case 0: cout << endl << "Exiting..."; 	return 0;
					case 1: list_ng(mess);					break;
					case 2: create_ng(mess); 				break;
					case 3: delete_ng(mess); 				break;
					case 4: list_art(mess); 				break;
					case 5: create_art(mess); 				break;
					case 6: delete_art(mess); 				break;
					case 7: get_art(mess); 					break;
					case 9: cout << help << endl; 			break;
					default: cout << "Wrong input! Input 9 to see available options!" << endl;
				}
			}
			catch (const ConnectionClosedException&)
			{
				cout << "No reply from server. Exiting." << endl;
				return 1;
			}


		}
		else 
		{
			cout << "Not a number! Input 9 to see available options!" << endl;
			cin.clear();
			cin.ignore(1024, '\n');
		}

		cout << endl << "Your input: ";
	}

	return 0;
}

int main(int argc, char* argv[])
{
		Connection conn = init(argc, argv);
		return app(conn);
}

void list_ng(MessageHandler &mess)
{
	int ng_amount;
	string name;
	string id;

	mess.writeInt(Protocol::COM_LIST_NG);
	mess.writeInt(Protocol::COM_END);

	if (mess.usrCommand() != Protocol::ANS_LIST_NG)
	{
		cout << "Protocol error! (ANS)" << endl;
		return;
	}

	try 
	{
		ng_amount = stoi(mess.readParam());
	}
	catch (const invalid_argument& e)
	{
		cout << "Unknown response from server! (Amount of newsgroups)";
		return;
	}
	
	cout << "--List of newsgroups--" << endl << "[ID]\t[Name]" << endl;
	for (int i = 0; i < ng_amount; i++) {
		id = mess.readParam();
		name = mess.readParam();
		cout << id << "\t" << name << endl;
	}
	cout << "-----End of list!-----" << endl << "" << endl;
	
	if (mess.usrCommand() != Protocol::ANS_END) cout << "Protocol error! (END)" << endl;
}

void create_ng(MessageHandler &mess)
{
	string input;
	
	cout << "Enter name of newsgroup to create: ";	
	getline(cin, input);
	if (input.length() == 0)
	{
		cout << "Error! No name has been input!" << endl;
		return;
	}

	mess.writeInt(Protocol::COM_CREATE_NG);
	mess.writeString(input);
	mess.writeInt(Protocol::COM_END);

	
	if (mess.usrCommand() != Protocol::ANS_CREATE_NG)
	{
		cout << "Protocol error! (ANS)"  << endl;
		return;
	}

	Protocol ack = mess.usrCommand();
	if (ack == Protocol::ANS_ACK) cout << "Newsgroup created!" << endl;
	else if (ack == Protocol::ANS_NAK)
	{
		if (mess.usrCommand() == Protocol::ERR_NG_ALREADY_EXISTS)
		{
			cout << "Error! Newsgroup already exists!" << endl;
		}
		else 
		{
			cout << "Protocol error! (ERR)" << endl;
		}
	}
	else
	{
		cout << "Protocol error! (ANS_ACK)" << endl;
		return;
	}

	if (mess.usrCommand() != Protocol::ANS_END) cout << "Protocol error! (ANS_END)" << endl;
}


void delete_ng(MessageHandler &mess)
{
	int id;

	cout << "Enter ID of newsgroup to delete: ";	
	cin >> id;
	if (!cin)
	{
		cout << "Error! Not a number!" << endl;
		cin.clear(); 
		cin.ignore(1024, '\n');
		return;
	}

	mess.writeInt(Protocol::COM_DELETE_NG);
	mess.writeInt(id);
	mess.writeInt(Protocol::COM_END);

	if (mess.usrCommand() != Protocol::ANS_DELETE_NG)
	{
		cout << "Protocol error! (ANS)" << endl;
		return;
	}

	Protocol ack = mess.usrCommand();
	if (ack == Protocol::ANS_ACK) cout << "Newsgroup deleted!" << endl;
	else if (ack == Protocol::ANS_NAK)
	{
		if (mess.usrCommand() == Protocol::ERR_NG_DOES_NOT_EXIST)
		{
			cout << "Error! That Newsgroup doesn't exist!" << endl;
		}
		else cout << "Protocol error! (ERR)";
	}
	else
	{
		cout << "Protocol error! (ACK)" << endl;
		return;
	}

	if (mess.usrCommand() != Protocol::ANS_END) cout << "Protocol error! (END)" << endl;
}

void list_art(MessageHandler &mess)
{
	int id, art_amount;
	string name, art_id;

	cout << "Enter newsgroup ID to list articles from: ";
	cin >> id;
	if (!cin)
	{ 
		cout << "Error! Not a number!" << endl;
		cin.clear(); 
		cin.ignore(1024, '\n');
		return;
	}

	mess.writeInt(Protocol::COM_LIST_ART);
	mess.writeInt(id);
	mess.writeInt(Protocol::COM_END);

	if (mess.usrCommand() != Protocol::ANS_LIST_ART)
	{
		cout << "Protocol error! (ANS)" << endl;
		return;
	}

	Protocol ack = mess.usrCommand();
	if (ack == Protocol::ANS_ACK)
	{
		try 
		{
			art_amount = stoi(mess.readParam());
		}
		catch (const invalid_argument& e)
		{
			cout << "Unknown response from server! (Amount of articles)" << endl;
			return;
		}
		
		cout << "--List of articles--" << endl << "[ID]\t[Name]" << endl;
		for (int i = 0; i < art_amount; i++) {
			art_id = mess.readParam();
			name = mess.readParam();
			cout << art_id << "\t" << name << endl;
		}
		cout << "----End of list!----" << endl;
	}
	else if (ack == Protocol::ANS_NAK)
	{
		if (mess.usrCommand() == Protocol::ERR_NG_DOES_NOT_EXIST)
		{
			cout << "Error! That newsgroup doesn't exist!" << endl;
		}
		else cout << "Protocol error! (ERR)" << endl;
	}
	else
	{
		cout << "Protocol error! (ACK)" << endl;
		return;
	}

	if (mess.usrCommand() != Protocol::ANS_END) cout << "Protocol error! (END)" << endl;
}

void create_art(MessageHandler &mess)
{
	string input, title, author, text;
	int id;

	cout << "Enter newsgroup ID to create article in: ";
	cin >> id;
	if (!cin)
	{ 
		cout << "Error! Not a number!" << endl;
		cin.clear(); 
		cin.ignore(1024, '\n');
		return;
	}
	cin.ignore(1024, '\n');
	cout << "Enter article title (one line): ";
	getline(cin, title);
	cout << "Enter article author (one line): ";
	getline(cin, author);
	cout << "Enter article text (multi-line, press ctrl+d when finished): ";
	while (getline(cin, input)) text += input += "\n";
	cin.clear();
	cin.ignore(1024, '\n');

	mess.writeInt(Protocol::COM_CREATE_ART);
	mess.writeInt(id);
	mess.writeString(title);
	mess.writeString(author);
	mess.writeString(text);
	mess.writeInt(Protocol::COM_END);

	if (mess.usrCommand() != Protocol::ANS_CREATE_ART)
	{
		cout << "Unknown handshake from server!" << endl;
		return;
	}

	Protocol ack = mess.usrCommand();
	if (ack == Protocol::ANS_ACK) cout << "Article created!" << endl;
	else if (ack == Protocol::ANS_NAK)
	{
		if (mess.usrCommand() == Protocol::ERR_NG_DOES_NOT_EXIST)
		{
			cout << "Error! Newsgroup doesn't exists!" << endl;
		}
		else cout << "Protocol error! (ERR)" << endl;
	}
	else
	{
		cout << "Protocol error! (ACK)" << endl;
		return;
	}

	if (mess.usrCommand() != Protocol::ANS_END) cout << "Protocol error! (END)" << endl;
}

void delete_art(MessageHandler &mess)
{
	int ng_id, art_id;

	cout << "Enter ID of the newsgroup the soon to be deleted article belongs to: ";	
	cin >> ng_id;
	if (!cin)
	{ 
		cout << "Error! Not a number!" << endl;
		cin.clear(); 
		cin.ignore(1024, '\n');
		return;
	}
	cout << "Enter ID of the article to delete: ";	
	cin >> art_id;
	if (!cin)
	{ 
		cout << "Error! Not a number!" << endl;
		cin.clear(); 
		cin.ignore(1024, '\n');
		return;
	}

	mess.writeInt(Protocol::COM_DELETE_ART);
	mess.writeInt(ng_id);
	mess.writeInt(art_id);
	mess.writeInt(Protocol::COM_END);

	if (mess.usrCommand() != Protocol::ANS_DELETE_ART)
	{
		cout << "Protocol error! (ANS)" << endl;
		return;
	}

	Protocol ack = mess.usrCommand();
	if (ack == Protocol::ANS_ACK) cout << "Article deleted!" << endl;
	else if (ack == Protocol::ANS_NAK) {
		Protocol err = mess.usrCommand();
		if (err == Protocol::ERR_NG_DOES_NOT_EXIST)
		{
			cout << "Error! Newsgroup doesn't exist!" << endl;
		}
		else if (err == Protocol::ERR_ART_DOES_NOT_EXIST)
		{
			cout << "Error! Article doesn't exist in newsgroup!";
		}
	}
	else
	{
		cout << "Protocol error! (ACK)" << endl;
		return;
	}

	if (mess.usrCommand() != Protocol::ANS_END) cout << "Protocol error! (END)" << endl;
}

void get_art(MessageHandler &mess)
{
	int ng_id, art_id;
	string title, author;
	
	cout << "Enter ID of the newsgroup the article belongs to: ";	
	cin >> ng_id;
	if (!cin)
	{ 
		cout << "Error! Not a number!" << endl;
		cin.clear(); 
		cin.ignore(1024, '\n');
		return;
	}
	cout << "Enter ID of the article to get: ";	
	cin >> art_id;
	if (!cin)
	{ 
		cout << "Error! Not a number!" << endl;
		cin.clear(); 
		cin.ignore(1024, '\n');
		return;
	}

	mess.writeInt(Protocol::COM_GET_ART);
	mess.writeInt(ng_id);
	mess.writeInt(art_id);
	mess.writeInt(Protocol::COM_END);

	if (mess.usrCommand() != Protocol::ANS_GET_ART) cout << "Protocol error! (ANS)" << endl;

	Protocol ack = mess.usrCommand();
	if (ack == Protocol::ANS_ACK)
	{
		title = mess.readParam();
		author = mess.readParam();
		cout << "Title: " << title << endl << "Author: " << author << endl;		
		cout << mess.readParam();
	}
	else if (ack == Protocol::ANS_NAK)
	{
		Protocol err = mess.usrCommand();
		if (err == Protocol::ERR_NG_DOES_NOT_EXIST)
		{
			cout << "Error! Newsgroup doesn't exist!" << endl;
		}
		else if (err == Protocol::ERR_ART_DOES_NOT_EXIST)
		{
			cout << "Error! Article doesn't exist in newsgroup!";
		}
	}
	else
	{
		cout << "Protocol error! (ACK)" << endl;
		return;
	}

	if (mess.usrCommand() != Protocol::ANS_END) cout << "Protocol error! (END)" << endl;
}	
