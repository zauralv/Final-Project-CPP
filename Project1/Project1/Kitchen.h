#pragma once
#include <vector>
#include "Cooking.h"
#include <map>
#include <Windows.h>

namespace Kitchen {
	class Menu
	{
		vector<Cooking::Dish*> dishes;
	public:
		Menu() {}

		Menu(vector<Cooking::Dish*> dishes) : dishes(dishes) {}

		vector<Cooking::Dish*> GetDishes() {
			return this->dishes;
		}

		void Show(int check, bool check2) {
			/*cout << "\n===================Menu===================\n\n";

			int count = 0;
			for (auto i : this->dishes)
			{
				count++;
				if (i->HasEnoughIngredients( )) {
					cout << count << ".";
					i->Info(check);
					cout << endl;
				}
			}*/
		}

		void WriteToFile() {
			fstream f("Menu.txt", ios::out);
			if (f.is_open()) {
				f << setw(15) << left << "Name" << setw(15) << left << "Price";
				for (auto i : this->dishes) {
					f << setw(15) << left << i->GetName() << setw(15) << left << i->GetPrice();
				}
			}
			f.close();
		}

		void DeleteIngredient(int dIndex, string pIndex) {
			map<Cooking::Ingredient*, int>::iterator it;
			for (it = this->dishes[dIndex - 1]->GetDishIngredients().begin(); it != this->dishes[dIndex - 1]->GetDishIngredients().end(); ++it)
				if (it->first->GetName() == pIndex) {
					this->dishes[dIndex - 1]->GetDishIngredients().erase(it);
					return;
				}
			throw Exceptions::InvalidArgumentException("ingredient with such doesn't exist\n");
		}

		void AddIngredient(int dIndex, Cooking::Ingredient* in, int pAmount) {
			for (auto i : this->dishes[dIndex]->GetDishIngredients()) {
				if (i.first->GetName() == in->GetName()) {
					this->dishes[dIndex]->GetDishIngredients()[i.first] = i.second + pAmount;
					return;
				}
			}

			this->dishes[dIndex]->GetDishIngredients().insert({ in,pAmount });
		}

		void AddMeal(Cooking::Dish* d) {
			this->dishes.push_back(d);
		}

		void DeleteMeal(int index) {
			if (index < 0 || index > dishes.size())
				throw Exceptions::InvalidArgumentException("there is no such meal in the menu\n");
			this->dishes.erase(this->dishes.begin() + index);
		}

		~Menu() {}
	};


	class Order
	{
		map<Cooking::Dish*, int> dishes;
	public:
		Order() : dishes() {}

		Order(map<Cooking::Dish*, int> dishes) :dishes(dishes) {}

		void AddDish(Cooking::Dish* d, int amount) {
			this->dishes.insert({ d,amount });
		}

		void Show() {
			cout << "\n================Your order================\n\n";

			int temp = 0, price = 0;
			for (auto i : dishes) {
				cout << 1 << ".";
				cout << "name: " << setw(15) << left << i.first->GetName() << "amount: " << i.second << endl;
				price += i.first->GetPrice();
			}
			cout << endl;
			cout << setw(40) << left << "Total price: " << price << endl << endl;
		}

		void ShowIngr(int check) {
			cout << "\n";
			for (auto i : this->dishes) {
				i.first->Info(check);
			}
		}

		map<Cooking::Dish*, int> GetDishes() {
			return this->dishes;
		}

		Cooking::Dish* GetDishByName(string name) {
			for (auto i : this->dishes) {
				if (i.first->GetName() == name)
					return i.first;
			}
			
		}

		void AddIngredientToDish(string name, Cooking::Ingredient* i, int amount) {
			for (auto it : this->dishes)
			{
				if (it.first->GetName() == name)
					it.first->AddIngredient(i, amount);
			}
		}

		double Prepare() {
			cout << "\nPreparing your food...\n\n";
			for (auto i : this->dishes)
			{
				Sleep(i.first->GetCookingTime() * 1000);
				cout << "\n" << i.first->GetName() << " is ready\n\n";
			}
			cout << "Your order is ready!\n\n";

			cout << "Here is the cheque: \n\n";

			cout << "===============================\n";
			double price = 0;
			cout << setw(20) << left << "\nName" << setw(20) << left << "Amount" << "Price\n\n";
			for (auto i : this->dishes) {
				cout << setw(20) << left << i.first->GetName() << setw(20) << left << i.second << setw(20) << left << i.first->GetPrice() << "\n";
				price += i.first->GetPrice();
			}
			cout << "\n\n" << setw(40) << "Total price : " << price << endl;
			return price;
		}

		~Order() {}
	};
}