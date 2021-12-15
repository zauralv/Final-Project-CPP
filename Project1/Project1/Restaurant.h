#pragma once

#include <iostream>
#include <vector>
#include "Users.h"
#include "Kitchen.h"
#include "Cooking.h"
#include <string>


class Restaurant
{
	double budget = 100;
	Control::Admin* admin;
	Control::Client* client = nullptr;
public:
	Restaurant() {}

	Restaurant(double budget, Control::Admin* admin, vector<Control::Client*> clients) : budget(budget), admin(admin), client(client) {}

	double GetBudget() const {
		return this->budget;
	}

	void Spend(double money) {
		this->budget -= money;
	}

	void Gain(double money) {
		this->budget += money;
	}

	void SetAdmin(Control::Admin* a) {
		this->admin = a;
	}

	void SetClient(Control::Client* c) {
		this->client = c;
	}

	Control::Admin* GetAdmin() {
		return this->admin;
	}

	Control::Client* GetClient() {
		return this->client;
	}

	~Restaurant() {}

};

void UseIngredientsFromStack(Cooking::ProductStock* ps, Cooking::Dish* d) {
	for (auto dIngredient : d->GetDishIngredients()) {
		for (auto sIngredient : ps->GetAllIngredients()) {
			if (dIngredient.first->GetName() == sIngredient.first->GetName()) {
				if (dIngredient.second > sIngredient.second)
					throw Exceptions::InvalidArgumentException("You can't take more than the amount that is in the stock\n");
				ps->GetAllIngredients()[sIngredient.first] = sIngredient.second -= dIngredient.second;
			}
		}
	}

	ps->WriteToFile();
}

void UseIngredientsFromStack(Cooking::ProductStock* ps, string name, int amount) {
	for (auto sIngredient : ps->GetAllIngredients()) {
		if (name == sIngredient.first->GetName()) {
			if (amount > sIngredient.second)
				throw Exceptions::InvalidArgumentException("You can't take more than the amount that is in the stock\n");
			ps->GetAllIngredients()[sIngredient.first] = sIngredient.second -= amount;
		}
	}

	ps->WriteToFile();
}

bool HasEnoughIngredients(Cooking::ProductStock* p, Cooking::Dish* dish) {
	for (auto i : dish->GetDishIngredients()) {
		bool check = false;
		for (auto j : p->GetAllIngredients()) {
			if (j.first->GetName() == i.first->GetName())
				if (j.second >= i.second)
					check = true;
		}
		if (!check)
			return false;
	}
	return true;
}

void ShowMenu(int check, vector<Cooking::Dish*> dishes, Cooking::ProductStock* p) {
	cout << "\n===================Menu===================\n\n";

	int count = 0;
	for (auto i : dishes)
	{
		count++;
		if (HasEnoughIngredients(p, i)) {
			cout << count << ".";
			i->Info(check);
			cout << endl;
		}
	}

}

class ControlRestaurant
{
	ControlRestaurant();
public:
	static void Start();
};

void ControlRestaurant::Start()
{
	Restaurant* r = new Restaurant();
	vector<Cooking::Ingredient*> additionalIngredients;
	additionalIngredients.push_back(new Cooking::Ingredient("lettuce", 10, 5, 1.2));
	additionalIngredients.push_back(new Cooking::Ingredient("mushroom", 15, 7, 3.2));
	additionalIngredients.push_back(new Cooking::Ingredient("cucumber", 8, 6, 0.9));

	Control::Admin* admin = new Control::Admin("admin", "admin");
	r->SetAdmin(admin);
	r->GetAdmin()->WriteToFile();

	map<Cooking::Ingredient*, int> allIngredients;
	allIngredients.insert({ new Cooking::Ingredient("cheese",30,10,2.7),10 });
	allIngredients.insert({ new Cooking::Ingredient("tomato",15,8,3.1),10 });
	allIngredients.insert({ new Cooking::Ingredient("dough",20,9,0.5),10 });
	allIngredients.insert({ new Cooking::Ingredient("macaroni",18,7,1.5),10 });
	allIngredients.insert({ new Cooking::Ingredient("beef",40,10,10.7),10 });
	allIngredients.insert({ new Cooking::Ingredient("pork",40,10,6.7),10 });
	allIngredients.insert({ new Cooking::Ingredient("chicken",40,15,5.7),10 });
	allIngredients.insert({ new Cooking::Ingredient("cabbage",10,6,2.2),10 });
	allIngredients.insert({ new Cooking::Ingredient("potato",10,6,2.2),10 });

	Cooking::ProductStock* ps = new Cooking::ProductStock(allIngredients);
	ps->WriteToFile();

	map<Cooking::Ingredient*, int> pizzaIngredients;
	pizzaIngredients.insert({ ps->GetProductByName("cheese"),3 });
	pizzaIngredients.insert({ ps->GetProductByName("dough"),2 });
	pizzaIngredients.insert({ ps->GetProductByName("tomato"),3 });
	Cooking::Dish* dish1 = new Cooking::Dish("pizza", pizzaIngredients);

	map<Cooking::Ingredient*, int> pastaIngredients;
	pastaIngredients.insert({ ps->GetProductByName("macaroni"),3 });
	pastaIngredients.insert({ ps->GetProductByName("tomato"),3 });
	pastaIngredients.insert({ ps->GetProductByName("cheese"),2 });
	Cooking::Dish* dish2 = new Cooking::Dish("pasta", pastaIngredients);

	map<Cooking::Ingredient*, int> dolmaIngredients;
	dolmaIngredients.insert({ ps->GetProductByName("beef"),4 });
	dolmaIngredients.insert({ ps->GetProductByName("tomato"),3 });
	dolmaIngredients.insert({ ps->GetProductByName("potato"),3 });
	dolmaIngredients.insert({ ps->GetProductByName("cabbage"),3 });
	Cooking::Dish* dish3 = new Cooking::Dish("badimcan dolma", dolmaIngredients);

	vector<Cooking::Dish*> dishes;
	dishes.push_back(dish1);
	dishes.push_back(dish2);
	dishes.push_back(dish3);

	Kitchen::Menu* menu = new Kitchen::Menu(dishes);

	fstream f1("Statistics.txt", ios::out);
	f1.close();

	fstream f("Statistics.txt", ios::app);
	if (f.is_open())
		f << setw(10) << left << "current budget: " << r->GetBudget() << endl << endl;
	f.close();

	while (true)
	{
	login:
		system("pause");
		system("cls");
		cout << "\n(admin user: admin, pass:admin)\n";
		cout << "\n1.admin\n";
		cout << "2.user\n";
		int ch;
		cout << "\nselect one: "; cin >> ch;

		if (ch == 1) {
			cout << endl;
		adminSignIn:
			system("cls");
			try {
				r->GetAdmin()->SignIn();
			}
			catch (Exceptions::InvalidArgumentException ex) {
				ex.showError();
				system("pause");
				goto adminSignIn;
			}
		adminChoice:
			//system("pause");
			system("cls");
			cout << "\n1.see menu\n";
			cout << "2.see products\n";
			cout << "3.see statistics\n";
			cout << "4.back\n\n";

			int ch2;
			cout << "select one: "; cin >> ch2;

			if (ch2 == 1) {
				system("cls");
				ShowMenu(1, menu->GetDishes(), ps);

			menuChange:
				char ch3;
				cout << "Do you want to make changes to menu?(y/n)"; cin >> ch3;

				if (ch3 == 'y') {
				menuChangeChoice:
					system("cls");
					cout << "\n1.Change meal\n";
					cout << "2.Add meal\n";
					cout << "3.Delete meal\n";
					cout << "4.Back\n\n";

					int ch4;
					cout << "select one: "; cin >> ch4;

					if (ch4 == 1) {
					mealIndex1:
						system("cls");
						cout << "\n1.Add ingredient\n";
						cout << "2.Delete ingredient\n";
						cout << "3.Back\n\n";

						int ch5;
						cout << "select one: "; cin >> ch5;

						system("cls");
						ShowMenu(0, menu->GetDishes(), ps);
						int dI;
						cout << "\nEnter index of the meal: "; cin >> dI;
						cout << endl << endl;
						menu->GetDishes()[dI - 1]->Info(1);
						if (ch5 == 1) {
							ps->Show();
						productName:
							string pName;
							cout << "\ntypethe name of the product from the list: "; cin >> pName;
							int pCount;
							cout << "enter amount: "; cin >> pCount;
							try {
								menu->AddIngredient(dI - 1, ps->GetProductByName(pName), pCount);
							}
							catch (Exceptions::InvalidArgumentException ex) {
								ex.showError();
								system("pause");
								goto productName;
							}
							cout << "\nthis ingredient has been succesfully added\n";
							menu->WriteToFile();
							goto adminChoice;
						}
						else if (ch5 == 2) {
						deleteProduct:
							string pI;
							cout << "\nenter name of product you want to delete: "; cin >> pI;
							try {
								menu->DeleteIngredient(dI, pI);
							}
							catch (Exceptions::InvalidArgumentException ex) {
								ex.showError();
								system("pause");
								goto deleteProduct;
							}
							cout << "\nthis product has been succesfully deleted\n";
							menu->WriteToFile();
							goto adminChoice;
						}
						else if (ch5 == 3) {
							goto menuChangeChoice;
						}
						else {
							try {
								throw Exceptions::InvalidArgumentException("wrong input\n");
							}
							catch (Exceptions::InvalidArgumentException ex) {
								ex.showError();
								system("pause");
								goto mealIndex1;
							}
						}
					}
					else if (ch4 == 2) {
					dishName:
						string dName;
						map<Cooking::Ingredient*, int> pMap;
						cout << "\nenter dish name: "; getline(cin, dName);
						cout << "\nenter products: \n\n";
						ps->Show();
						string lpName;
						int pCount;
						cout << "\nchoose the product from list(enter name): "; cin >> lpName;
						cout << "enter amount: "; cin >> pCount;
						try {
							pMap.insert({ ps->GetProductByName(lpName),pCount });
						}
						catch (Exceptions::InvalidArgumentException ex) {
							ex.showError();
							system("pause");
							goto dishName;
						}
						char ch6;
						while (true)
						{
						addCont:
							cout << "do you want to continue adding?(y/n): "; cin >> ch6;
							if (ch6 == 'y') {
							listProduct:
								cout << "\nchoose the product from list(enter name): "; cin >> lpName;
								cout << "enter amount: "; cin >> pCount;
								try {
									pMap.insert({ ps->GetProductByName(lpName),pCount });
								}
								catch (Exceptions::InvalidArgumentException ex) {
									ex.showError();
									system("pause");
									goto listProduct;
								}
								continue;
							}
							else if (ch6 == 'n')
								break;
							else {
								try {
									throw Exceptions::InvalidArgumentException("wrong input\n");
								}
								catch (Exceptions::InvalidArgumentException ex) {
									ex.showError();
									system("pause");
									goto addCont;
								}
							}
						}

						Cooking::Dish* d = new Cooking::Dish(dName, pMap);
						menu->AddMeal(d);
						cout << "\n the meal has been succesfully added\n\n";
						menu->WriteToFile();
						goto adminChoice;
					}
					else if (ch4 == 3) {
						system("cls");
						ShowMenu(0, menu->GetDishes(), ps);

					mealIndex:
						int dIn;
						cout << "\Enter index of the meal you want to delete: "; cin >> dIn;
						try {
							menu->DeleteMeal(dIn - 1);
						}
						catch (Exceptions::InvalidArgumentException ex) {
							ex.showError();
							system("pause");
							goto mealIndex;
						}
						cout << "\n the meal has been succesfully deleted\n\n";
						menu->WriteToFile();
						goto adminChoice;
					}
					else if (ch4 == 4) {
						goto adminChoice;
					}
					else {
						try {
							throw Exceptions::InvalidArgumentException("wrong input\n");
						}
						catch (Exceptions::InvalidArgumentException ex) {
							ex.showError();
							system("pause");
							goto menuChangeChoice;
						}
					}
				}
				else if (ch3 == 'n') {
					goto adminChoice;
				}
				else {
					try {
						throw Exceptions::InvalidArgumentException("invalid input\n");
					}
					catch (Exceptions::InvalidArgumentException ex) {
						ex.showError();
						goto menuChange;
					}
				}
			}
			else if (ch2 == 2) {
				system("cls");
				char ch7;
				ps->Show();
				cout << "\nDo you want to add product to stock?(y/n): "; cin >> ch7;
				cout << endl;

				if (ch7 == 'y') {
					cout << "\n1.Existing product\n";
					cout << "2.New product\n\n";
				ProductChoice:
					int ch12;
					cout << "select one: "; cin >> ch12;
					if (ch12 == 1) {
					StockProduct:
						string stockAddN;
						int stockAddA;
						cout << "\nenter name of the product from stock: "; cin >> stockAddN;
						cout << "enter amount: "; cin >> stockAddA;
						try {
							ps->AddProduct(ps->GetProductByName(stockAddN), stockAddA);
						}
						catch (Exceptions::InvalidArgumentException ex) {
							ex.showError();
							system("pause");
							goto StockProduct;
						}
						r->Spend(stockAddA * ps->GetProductByName(stockAddN)->GetPrice());
						fstream f("Statistics.txt", ios::app);
						if (f.is_open())
							f << stockAddA * ps->GetProductByName(stockAddN)->GetPrice() << " was spent for products" << "\n\ncurrent balance is " << r->GetBudget() << endl;
						f.close();
						cout << "\nproduct has been succesfully added\n";
						system("pause");
						//UseIngredientsFromStack(ps, stockAddN, stockAddA);
						goto adminChoice;
					}
					else if (ch12 == 2) {
						system("cls");
						int tempC = 0;
						for (auto i : additionalIngredients)
						{
							cout << ++tempC << ".";
							i->Info();
						}
					addProdChoice:
						int ch8, pA;
						cout << "select one: "; cin >> ch8;
						cout << "enter amount: "; cin >> pA;
						try {
							ps->AddProduct(additionalIngredients[ch8 - 1], pA);
						}
						catch (Exceptions::InvalidArgumentException ex) {
							ex.showError();
							system("pause");
							goto addProdChoice;
						}
						cout << "\nproduct has been succesfully added\n";
						system("pause");
						r->Spend(pA * additionalIngredients[ch8 - 1]->GetPrice());
						fstream f("Statistics.txt", ios::app);
						if (f.is_open())
							f << pA * additionalIngredients[ch8 - 1]->GetPrice() << " was spent for products" << "\n\ncurrent balance is " << r->GetBudget() << endl;
						f.close();
						goto adminChoice;
					}
					else {
						try {
							throw Exceptions::InvalidArgumentException("invalid input\n");
						}
						catch (Exceptions::InvalidArgumentException ex) {
							ex.showError();
							system("cls");
							goto ProductChoice;
						}
					}
				}
				else {
					goto adminChoice;
				}
			}
			else if (ch2 == 3) {
				system("cls");
				cout << "\nStatistics: \n\n";
				fstream f("Statistics.txt", ios::in);
				string line;
				if (f.is_open())
					while (!f.eof()) {
						getline(f, line);
						cout << line << endl;
					}
				f.close();
				system("pause");
				goto adminChoice;
			}
			else if (ch2 == 4) {
				goto login;
			}
			else {
				try {
					throw Exceptions::InvalidArgumentException("invalid input\n");
				}
				catch (Exceptions::InvalidArgumentException ex) {
					ex.showError();
					goto adminChoice;
				}
			}
		}
		else if (ch == 2) {

			if (r->GetClient() == nullptr) {
				string user;
				string pass = "";

				cout << "\n\nusername rules(more than 4 char)\npassword rules(one capital,one num, more than one char)\n\n";
				system("pause");

			UserSignUp:

				cout << "\ncreate username: "; cin >> user;
				cout << "create password: ";

				while (true)
				{
					char ch_ = _getch();
					if (ch_ == 13)
						break;
					if (ch_ != 8 && ch_ != 13 && ch_ != 32) {
						_putch('*');
						pass += ch_;
					}
				}

				try {
					if (user.empty()) {
						throw Exceptions::InvalidArgumentException("username can't be empty\n");
					}
					if (user.size() <= 4) {
						throw Exceptions::InvalidArgumentException("username length should be more than 4\n");
					}
					if (pass.size() <= 4) {
						throw Exceptions::InvalidArgumentException("password length should be more than 4\n");
					}
					if (pass.empty()) {
						throw Exceptions::InvalidArgumentException("password can't be empty\n");
					}
					bool check1 = true, check2 = true;
					for (auto i : pass)
					{
						if (check1)
							if (i >= 65 && i <= 90) {
								check1 = false;
							}
						if (check2)
							if (i >= 48 && i <= 57) {
								check2 = false;
							}
					}

					if (check1 || check2) {
						throw Exceptions::InvalidArgumentException("follow the rules\n");
					}
				}
				catch (Exceptions::InvalidArgumentException ex) {
					ex.showError();
					goto UserSignUp;
				}
				Control::Client* u = new Control::Client(user, pass);
				r->SetClient(u);
				cout << "\nyou have succesfully signed up\n";
				r->GetClient()->WriteToFile();
			}
			else {
			UserSignIn:
				try {
					r->GetClient()->SignIn();
				}
				catch (Exceptions::InvalidArgumentException ex) {
					ex.showError();
					goto UserSignIn;
				}
				cout << "\nyou have succesfully logged in\n";
			}

			ShowMenu(0, menu->GetDishes(), ps);
			Kitchen::Order* o = new Kitchen::Order();

			int ch9;
			int dA;
			while (true)
			{
				cout << "\nchoose meal you want to add to cart: "; cin >> ch9;
				cout << "enter amount: "; cin >> dA;
				cout << endl;
				o->AddDish(menu->GetDishes()[ch9 - 1], dA);

			Adding:
				char ch10;
				cout << "do you want to continue adding?(y/n): "; cin >> ch10;
				if (ch10 == 'y')
					continue;
				else if (ch10 == 'n')
					break;
				else {
					try {
						throw Exceptions::InvalidArgumentException("invalid input\n");
					}
					catch (Exceptions::InvalidArgumentException ex) {
						ex.showError();
						goto Adding;
					}
				}
			}

			o->Show();
			char ch11;
			cout << "do you want to make changes?(y/n): "; cin >> ch11;
			if (ch11 == 'y') {
				string mI;
				cout << "\nenter name of the meal you want to modify: ";
				cin.ignore(100, '\n');
				getline(cin, mI);

				o->GetDishByName(mI)->Info(1);
				ps->Show();

			AddProduct:
				string pN;
				int dpCount;
				cout << "type the name of the product you want to add: "; cin >> pN;
				cout << "enter amount: "; cin >> dpCount;
				cout << endl;

				try {
					o->AddIngredientToDish(mI, ps->GetProductByName(pN), dpCount);
				}
				catch (Exceptions::InvalidArgumentException ex) {
					ex.showError();
					goto AddProduct;
				}

				cout << "product has been succesfully added to dish\n";
				o->ShowIngr(1);
			}
			double orderPrice = o->Prepare();

			for (auto i : o->GetDishes())
			{
				UseIngredientsFromStack(ps, i.first);
			}

			r->Gain(orderPrice);
			fstream f("Statistics.txt", ios::app);
			if (f.is_open())
				f << orderPrice << "was gained by the order\n\n" << "current budget is " << r->GetBudget() << endl << endl;
			f.close();
			char orderAgain;
			cout << "\nDo you want to order again?(y/n): "; cin >> orderAgain;
			if (orderAgain == 'y')
				continue;
			else
				break;
		}
		else {
			try {
				throw Exceptions::InvalidArgumentException("invalid input\n");
			}
			catch (Exceptions::InvalidArgumentException ex) {
				ex.showError();
				goto login;
			}
		}
	}
	system("cls");
	cout << "\n\nThank you for visiting us :)\n\n";
}

