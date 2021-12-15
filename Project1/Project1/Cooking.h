#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "Exceptions.h"
#include <fstream>

using namespace std;

namespace Cooking {
	class Ingredient
	{
		string name;
		double kCal = 0;
		double protein = 0;
		double price = 0;
	public:
		Ingredient() {}

		Ingredient(string name, double kCal, double protein, double price) {
			if (name.empty()) {
				throw Exceptions::InvalidArgumentException("Ingredient name can't be empty\n");
			}
			if (kCal < 0)
				throw Exceptions::InvalidArgumentException("Ingredient kCal can't be negative\n");
			if (protein < 0)
				throw Exceptions::InvalidArgumentException("Ingredient protein can't be negative\n");
			if (price < 0)
				throw Exceptions::InvalidArgumentException("Ingredient price can't be negative\n");
			this->name = name;
			this->kCal = kCal;
			this->protein = protein;
			this->price = price;
		}

		void SetName(string name) {
			if (name.empty())
				throw Exceptions::InvalidArgumentException("name can't be empty\n");
			this->name = name;
		}

		void SetPrice(double price) {
			if (price < 0)
				throw Exceptions::InvalidArgumentException("price can't be negative\n");
			this->price = price;
		}

		void SetKCal(double kCal) {
			if(kCal < 0)
				throw Exceptions::InvalidArgumentException("kcal can't be negative\n");
			this->kCal = kCal;
		}

		void SetProtein(double protein) {
			if(protein < 0)
				throw Exceptions::InvalidArgumentException("protein can't be negative\n");
			this->protein = protein;
		}

		double GetProtein() const {
			return this->protein;
		}

		double GetPrice() const {
			return this->price;
		}

		double GetKCal() const {
			return this->kCal;
		}

		void Info() {
			cout << setw(15) << left << "product name: " << this->name << endl;
			cout << setw(15) << left << "product protein: " << this->protein << endl;
			cout << setw(15) << left << "product kCal: " << this->kCal << endl << endl;
		}

		string GetName() const {
			return this->name;
		}

		~Ingredient() {}
	};

	class Dish
	{
		string name;
		map<Ingredient*, int> dishIngredients;
		double price = 0;
	public:
		Dish() {}

		Dish(string name, map<Ingredient*, int> ingredients) {
			if (name.empty())
				throw Exceptions::InvalidArgumentException("Dish name can't be empty\n");
			if (ingredients.empty())
				throw Exceptions::InvalidArgumentException("Ingredients can't be empty(dish)\n");
			this->dishIngredients = ingredients;
			this->name = name;
			for (auto i : ingredients)
			{
				price += i.first->GetPrice() * i.second;
			}
		}

		//bool HasEnoughIngredients(ProductStock*& p) {
		//	//map<Ingredient*,int> j = p->get
		//	for (auto i : this->dishIngredients) {
		//		bool check = false;
		//		for (auto j: p->GetAllIngredients()) {
		//			if (j.first->GetName() == i.first->GetName())
		//				if (j.second >= i.second)
		//					check = true;
		//		}
		//		if (!check)
		//			return false;
		//	}
		//	return true;
		//}

		map<Ingredient*, int>& GetDishIngredients() {
			return this->dishIngredients;
		}

		int GetCookingTime() {
			return this->GetTotalKCal() / 10;
			
		}

		string GetName() const{
			return this->name;
		}

		double GetPrice() const{
			return this->price;
		}

		void showIngredients() {
			int count = 0;
			for (auto i : this->dishIngredients) {
				cout << ++count << ")name: " << left << setw(15) << i.first->GetName() << left << setw(5) << "count: " << left << setw(15) << i.second << endl;
			}
		}

		void Info(int check) {
			cout << this->name <<   endl << endl;


			if (check) {
				cout << "products: " << left << endl << endl;
				showIngredients();
			}
			cout << "\n----------------------------\n";
		}

		double GetTotalKCal() {
			double temp = 0;
			for (auto i : dishIngredients) {
				temp += i.first->GetKCal();
			}
			return temp;
		}

		void AddIngredient(Ingredient* i,int amount) {
			bool check = false;
			for (auto it : this->dishIngredients) {
				if (it.first->GetName() == i->GetName()) {
					this->dishIngredients[it.first] = it.second + amount;
					return;
				}
			}
			this->dishIngredients.insert({ i,amount });
		}

		~Dish() {}
	};

	class ProductStock
	{
		map<Ingredient*, int> allIngredients;
	public:
		ProductStock() {}

		ProductStock(map<Ingredient*, int> allIngredients) {
			if (allIngredients.empty())
				throw Exceptions::InvalidArgumentException("Ingredients can't be empty(product stock)\n");
			this->allIngredients = allIngredients;
		}

		void WriteToFile() {
			fstream file("ProductStock.txt", ios::out);

			if (file.is_open()) {
				file.setf(ios::left);
				file << setw(30) << "Name" << setw(30) << "Amount" << setw(30) << "KCal" << setw(30) << "Price for one" << endl;
				for (auto i : this->allIngredients)
				{
					file << setw(30) << i.first->GetName() << setw(30) << i.second << setw(30) << i.first->GetKCal() << setw(30) << i.first->GetPrice() << endl;
				}
			}
			else {
				throw Exceptions::FileWriteException("File can't be opened(product stock)\n");
			}
			file.close();
		}

		map<Ingredient*, int>& GetAllIngredients() {
			return this->allIngredients;
		}

		void AddProduct(Ingredient* p,int amount) {
			for (auto i: this->allIngredients) {
				if (i.first->GetName() == p->GetName()) {
					this->allIngredients[i.first] = amount + i.second;
					//cout << this->allIngredients[i.first] << endl;
					//cout << amount + i.second << endl;
					//Show();
					//system("pause");
					return;
				}
			}
			if(p == nullptr)
				throw Exceptions::InvalidArgumentException("ingredient can't be empty\n");
			this->allIngredients.insert({p,amount});
		}

		Ingredient* GetProductByName(string prodName) {
			for (auto i : this->allIngredients) {
				if (i.first->GetName() == prodName)
					return i.first;
			}
			throw Exceptions::InvalidArgumentException("no product with such name(product stock)\n");
		}

		void Show() {
			int temp = 0;
			for (auto i : allIngredients) {
				cout << ++temp;
				cout << setw(15) << left << ".product name: " << i.first->GetName() << endl;
				cout << setw(15) << left << "  product count: " << i.second << endl << endl;
				/*cout << setw(15) << left << "product name: " << i.first->GetName() << endl;
				cout << setw(15) << left << "product price: " << i.first->GetPrice() << endl;
				cout << setw(15) << left << "product protein: " << i.first->GetProtein() << endl;
				cout << setw(15) << left << "product kCal: " << i.first->GetKCal() << endl << endl;*/
			}
		}

		
		~ProductStock() {}
	};


}