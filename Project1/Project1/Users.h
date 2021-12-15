#pragma once

#include <iostream>
#include "Exceptions.h"
#include <conio.h>

using namespace std;

namespace Control {
	class User
	{
	protected:
	public:
		string username = "";
		string password = "";
		User() {}

		User(string username, string password) {
			if (username.empty())
				throw Exceptions::InvalidArgumentException("username can't be empty\n");
			if (password.empty())
				throw Exceptions::InvalidArgumentException("password can't be empty\n");
			this->username = username;
			this->password = password;
		}

		void SignIn() {
			string user, pass = "";
			cout << "\nenter username: "; cin >> user;
			cout << "enter password: ";
			while (true)
			{
				char ch = _getch();
				if (ch == 13)
					break;
				if (ch != 8 && ch != 13 && ch != 32) {
					_putch('*');
					pass += ch;
				}
			}
			if (this->password == pass && this->username == user) {
				cout << "\n\nyou succesfully logged in\n";
			}
			
			if (this->password != pass || this->username != user) {
				throw Exceptions::InvalidArgumentException("wrong credentials\n");
			}
		}

		void WriteToFile() {
			fstream f("Users.txt",ios::app);
			if (f.is_open()) {
				f << setw(15) << left << this->username << this->password;
			}
			f.close();
		}

		virtual ~User() = 0 {}
	};

	class Admin : public User
	{
	public:
		Admin(string username, string password) : User(username, password) {		}


	};

	class Client : public User
	{
	public:
		Client(string username, string password) : User(username, password) {
			
		}

	};


}
