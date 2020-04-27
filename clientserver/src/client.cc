#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"


#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

string help = "help";

void create_ng(MessageHandler &mess);
void list_ng(MessageHandler &mess);
void delete_ng(MessageHandler &mess);
void list_art(MessageHandler &mess);
void create_art(MessageHandler &mess);
void delete_art(MessageHandler &mess);

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
	cout << help;
	int input;
	while (cin >> input)
	{
		cout << input;
	}

	list_ng(mess);
	//create_ng(mess);
	delete_ng(mess);
	list_ng(mess);
		



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
	cout << "Enter name of newsgroup to create: ";	
	string input;
	cin >> input;
	//Check for user error here
	mess.writeInt(Protocol::COM_CREATE_NG);
	mess.writeString(input);
	mess.writeInt(Protocol::COM_END);
	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_CREATE_NG) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK);
		else if (ack == Protocol::ANS_NAK) cout << "Newsgroup already exists!" << endl;
		else cout << "Unkown ack from server!" << endl; return;

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "Newsgroup created!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}


void delete_ng(MessageHandler &mess)
{
	cout << "Enter ID of newsgroup to delete: ";	
	string input;
	cin >> input;
	//Check for user error here
	int id = stoi(input);
	mess.writeInt(Protocol::COM_DELETE_NG);
	mess.writeInt(id);
	mess.writeInt(Protocol::COM_END);
	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_DELETE_NG) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK);
		else if (ack == Protocol::ANS_NAK) cout << "Newsgroup doesn't exists!" << endl;
		else cout << "Unkown ack from server!" << endl; return;

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "Newsgroup deleted!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}

void list_art(MessageHandler &mess)
{
	cout << "Enter newsgroup ID to list articles from: ";
	string input;
	cin >> input;
	//Check for user error here
	int id = stoi(input);
	mess.writeInt(Protocol::COM_DELETE_NG);
	mess.writeInt(id);
	mess.writeInt(Protocol::COM_END);
	Protocol handshake = mess.usrCommand();
	if (handshake != Protocol::ANS_LIST_ART) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = mess.usrCommand();
		if (ack == Protocol::ANS_ACK);
		else if (ack == Protocol::ANS_NAK) cout << "Newsgroup doesn't exists!" << endl;
		else cout << "Unkown ack from server!" << endl; return;

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "Newsgroup deleted!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}

void create_art(MessageHandler &mess)
{
	cout << "Enter newsgroup ID to create article in: ";
	string input;
	cin >> input;
	//Check for user error here
	int id = stoi(input);
	cout << "Enter article title: ";
	cin >> input;
	string title = input;
	cout << "Enter article author: ";
	cin >> input;
	string author = input;
	cout << "Enter article text: ";
	cin >> input;
	string text = input;
	//Check for user error here
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
		else if (ack == Protocol::ANS_NAK) cout << "Newsgroup doesn't exists!" << endl;
		else cout << "Unkown ack from server!" << endl; return;

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "Newsgroup deleted!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
	
}

void delete_art(MessageHandler &mess)
{
	cout << "Enter ID of the newsgroup the soon to be deleted article belongs to: ";	
	string input;
	cin >> input;
	//Check for user error here
	int ng_id = stoi(input);
	cout << "Enter ID of the article to delete: ";	
	cin >> input;
	//Check for user error here
	int art_id = stoi(input);
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
		else cout << "Unkown ack from server!" << endl; return;

		Protocol end = mess.usrCommand();
		if (end == Protocol::ANS_END) cout << "Newsgroup deleted!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}
