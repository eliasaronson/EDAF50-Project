#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"


#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

void create_ng(MessageHandler &mess);
void list_ng(MessageHandler &mess);
void delete_ng(MessageHandler &mess);
void list_art(MessageHandler &mess);
/*
 * * Send an integer to the server as four bytes.
 * */
void writeNumber(const Connection& conn, int value)
{
		conn.write((value >> 24) & 0xFF);
		conn.write((value >> 16) & 0xFF);
		conn.write((value >> 8) & 0xFF);
		conn.write(value & 0xFF);
}

/* Read a string from the server. */
string readString(const Connection& conn)
{
		string s;
		char ch;
		while ((ch = conn.read()) != '$') {
				s += ch;
		}
		return s;
}

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
		/*cout << "Type a number: ";
		int nbr;
		while (cin >> nbr) {
				try {
						cout << nbr << " is ...";
						writeNumber(conn, nbr);
						string reply = readString(conn);
						cout << " " << reply << endl;
						cout << "Type another number: ";
				} catch (ConnectionClosedException&) {
						cout << " no reply from server. Exiting." << endl;
						return 1;
				}
		}
		cout << "\nexiting.\n";
		*/
		MessageHandler mess(conn);

		//create_ng(mess);
		list_ng(mess);
		



		return 0;
}

int main(int argc, char* argv[])
{
		Connection conn = init(argc, argv);
		return app(conn);
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
	Protocol handshake = static_cast<Protocol>(mess.usrCommand());
	if (handshake != Protocol::ANS_CREATE_NG) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = static_cast<Protocol>(mess.usrCommand());
		if (ack == Protocol::ANS_ACK);
		else if (ack == Protocol::ANS_NAK) cout << "Newsgroup already exists!" << endl;
		else cout << "Unkown ack from server!" << endl; return;

		Protocol end = static_cast<Protocol>(mess.usrCommand());
		if (end == Protocol::ANS_END) cout << "Newsgroup created!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}

void list_ng(MessageHandler &mess)
{
	mess.writeInt(Protocol::COM_LIST_NG);
	mess.writeInt(Protocol::COM_END);
	Protocol response = static_cast<Protocol>(mess.usrCommand());
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
		
		Protocol end = static_cast<Protocol>(mess.usrCommand());
		if (end == Protocol::ANS_END) cout << "End of list!" << endl;
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
	Protocol handshake = static_cast<Protocol>(mess.usrCommand());
	if (handshake != Protocol::ANS_DELETE_NG) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = static_cast<Protocol>(mess.usrCommand());
		if (ack == Protocol::ANS_ACK);
		else if (ack == Protocol::ANS_NAK) cout << "Newsgroup doesn't exists!" << endl;
		else cout << "Unkown ack from server!" << endl; return;

		Protocol end = static_cast<Protocol>(mess.usrCommand());
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
	Protocol handshake = static_cast<Protocol>(mess.usrCommand());
	if (handshake != Protocol::ANS_DELETE_NG) cout << "Unknown handshake from server!" << endl;
	else
	{
		Protocol ack = static_cast<Protocol>(mess.usrCommand());
		if (ack == Protocol::ANS_ACK);
		else if (ack == Protocol::ANS_NAK) cout << "Newsgroup doesn't exists!" << endl;
		else cout << "Unkown ack from server!" << endl; return;

		Protocol end = static_cast<Protocol>(mess.usrCommand());
		if (end == Protocol::ANS_END) cout << "Newsgroup deleted!" << endl;
		else  cout << "Unknown message ending!" << endl;
	}
}
