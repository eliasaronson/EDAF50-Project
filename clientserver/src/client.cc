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
	cout << "Your input: ";
	while (true)
	{
		if (cin >> input)
		{	
			try
			{
				switch(input)
				{
					case 0: cout << "\nexiting.\n"; return 0;
					case 1: list_ng(mess); break;
					case 2: create_ng(mess); break;
					case 3: delete_ng(mess); break;
					case 4: list_art(mess); break;
					case 5: create_art(mess); break;
					case 6: delete_art(mess); break;
					case 7: get_art(mess); break;
					case 9: cout << help << endl; break;
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

		cout << "Your input: ";
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
	mess.writeInt(Protocol::COM_LIST_NG);
	mess.writeInt(Protocol::COM_END);

	Protocol response = mess.usrCommand();
	if (response != Protocol::ANS_LIST_NG) cout << "Unknown handshake from server!" << endl;
	else
	{
		int ng_amount = 0;
		try 
		{
			ng_amount = stoi(mess.readParam());
		}
		catch (const invalid_argument& e)
		{
			cout << "Unknown response from server! (Amount of newsgroups)";
			return;
		}
		
		cout << "List of newsgroups" << endl << "[title]\t\t[id number]" << endl;
		for (int i = 0; i < ng_amount; i++) {
			cout << mess.readParam() << "\t\t" << mess.readParam() << endl;
		}
		
		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "End of list!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}

void create_ng(MessageHandler &mess)
{
	string input;
	
	cout << "Enter name of newsgroup to create: ";	
	getline(cin, input);
	if (input.length() == 0)
	{
		cout << ""
	}

	mess.writeInt(Protocol::COM_CREATE_NG);
	mess.writeString(input);
	mess.writeInt(Protocol::COM_END);

	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_CREATE_NG) cout << "Unknown handshake from server!" << endl;
	else
	{
		cout << "Handshake recieved!" << endl;
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK) cout << "Ack recieved!" << endl;
		else if (ack == Protocol::ANS_NAK) {mess.usrCommand(); cout << "Newsgroup already exists!" << endl;}
		else {cout << "Unkown ack from server!" << endl; return;}

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "End recieved!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}


void delete_ng(MessageHandler &mess)
{
	int id;

	cout << "Enter ID of newsgroup to delete: ";	
	cin >> id;
	if (!cin){ cout << "Error! Not a number!" << endl; return; }

	mess.writeInt(Protocol::COM_DELETE_NG);
	mess.writeInt(id);
	mess.writeInt(Protocol::COM_END);

	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_DELETE_NG) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK) cout << "Newsgroup deleted!";
		else if (ack == Protocol::ANS_NAK) {mess.usrCommand(); cout << "Newsgroup doesn't exists!" << endl;}
		else {cout << "Unkown ack from server!" << endl; return;}

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "End recieved!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}

void list_art(MessageHandler &mess)
{
	int id;

	cout << "Enter newsgroup ID to list articles from: ";
	cin >> id;
	if (!cin){ cout << "Error! Not a number!" << endl; return; }

	mess.writeInt(Protocol::COM_LIST_ART);
	mess.writeInt(id);
	mess.writeInt(Protocol::COM_END);

	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_LIST_ART) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK)
		{
			int art_amount = 0;
		try 
		{
			art_amount = stoi(mess.readParam());
		}
		catch (const invalid_argument& e)
		{
			cout << "Unknown response from server! (Amount of articles)";
			return;
		}
		
		cout << "List of articles" << endl << "[title]\t\t[id number]" << endl;
		for (int i = 0; i < art_amount; i++) {
			cout << mess.readParam() << "\t\t" << mess.readParam() << endl;
		}
	}
		else if (ack == Protocol::ANS_NAK) {mess.usrCommand(); cout << "Newsgroup doesn't exists!" << endl;}
		else {cout << "Unkown ack from server!" << endl; return;}

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "End recieved!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}

void create_art(MessageHandler &mess)
{
	string input, title, author, text;
	int id;

	cout << "Enter newsgroup ID to create article in: ";
	cin >> id;
	if (!cin){ cout << "Error! Not a number!" << endl; return; }
	cout << "Enter article title: ";
	getline(cin, title);
	cout << "Enter article author: ";
	getline(cin, author);
	cout << "Enter article text: ";
	while (getline(cin, input)) text += input += "\n";

	mess.writeInt(Protocol::COM_CREATE_ART);
	mess.writeInt(id);
	mess.writeString(title);
	mess.writeString(author);
	mess.writeString(text);
	mess.writeInt(Protocol::COM_END);

	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_CREATE_ART) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK);
		else if (ack == Protocol::ANS_NAK) {mess.usrCommand(); cout << "Newsgroup doesn't exists!" << endl;}
		else {cout << "Unkown ack from server!" << endl; return;}

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "End recieved!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
	
}

void delete_art(MessageHandler &mess)
{
	int ng_id, art_id;

	cout << "Enter ID of the newsgroup the soon to be deleted article belongs to: ";	
	cin >> ng_id;
	if (!cin){ cout << "Error! Not a number!" << endl; return; }
	cout << "Enter ID of the article to delete: ";	
	cin >> art_id;
	if (!cin){ cout << "Error! Not a number!" << endl; return; }

	mess.writeInt(Protocol::COM_DELETE_ART);
	mess.writeInt(ng_id);
	mess.writeInt(art_id);
	mess.writeInt(Protocol::COM_END);

	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_DELETE_ART) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK);
		else if (ack == Protocol::ANS_NAK) {
			Protocol err = mess.usrCommand();
			if (err == Protocol::ERR_NG_DOES_NOT_EXIST) cout << "Error! No such newsgroup!";
			else if (err == Protocol::ERR_ART_DOES_NOT_EXIST) cout << "Error! No such article!";
		}
		else {cout << "Unkown ack from server!" << endl; return;}

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "End recieved" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}

void get_art(MessageHandler &mess)
{
	int ng_id, art_id;
	
	cout << "Enter ID of the newsgroup the article belongs to: ";	
	cin >> ng_id;
	if (!cin){ cout << "Error! Not a number!" << endl; return; }
	cout << "Enter ID of the article to get: ";	
	cin >> art_id;
	if (!cin){ cout << "Error! Not a number!" << endl; return; }

	mess.writeInt(Protocol::COM_GET_ART);
	mess.writeInt(ng_id);
	mess.writeInt(art_id);
	mess.writeInt(Protocol::COM_END);

	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_GET_ART) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK)
		{
			cout << "Title: " << mess.readParam() << "\tAuthor: " << mess.readParam() << endl;		
			cout << mess.readParam();
		}
		else if (ack == Protocol::ANS_NAK)
		{
			Protocol err = mess.usrCommand();
			if (err == Protocol::ERR_NG_DOES_NOT_EXIST) cout << "Error! No such newsgroup!" << endl;
			else if (err == Protocol::ERR_ART_DOES_NOT_EXIST) cout << "Error! No such article!" << endl;
		}
		else {cout << "Unkown ack from server!" << endl; return;}

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "End of article!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}	
