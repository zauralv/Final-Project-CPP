#pragma once
#include <iostream>

using namespace std;

namespace Exceptions {
	class Exception {
	protected:
		string errorMessage;
	public:
		virtual void showError() {		}
	};

	class InvalidArgumentException : public Exception {
	public:
		InvalidArgumentException(string text) {
			this->errorMessage = text;
		}

		virtual void showError() override
		{
			std::cout << "\nInvalid Argument Error : " << errorMessage << "\n";
		}
	};

	class FileWriteException : public Exception {
	public:
		FileWriteException(string text) {
			this->errorMessage = text;
		}

		virtual void showError() override
		{
			std::cout << "\nFile Write Error : " << errorMessage << "\n";
		}
	};

}