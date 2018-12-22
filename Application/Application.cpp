#include "Line.h"

#include <iostream>
#include <string>
#include <exception>
#include <fstream>

using namespace Library;
using namespace std;

string msgs[] = { "Exit", "Print menu", "Print", "Set line name", "Add station","Add transfer", "Find station", "Delete station", "Find transfer", 
	"Get type of station", "Delete transfer", "Change station name", "Save file", "Read file", "Check stations"};

const int amount = 15;

Line CreateNew(); //создание нового 

bool Exit(Line&);              //функция выхода
bool Menu(Line&);              //меню
bool Print(Line&);             //вывод всей информации
bool SetName(Line&);           //изменить имя линии
bool AddStation(Line&);        //добавить станцию
bool AddTransfer(Line&);       //добавить пересадку
bool FindStation(Line&);       //поиск станции на линии
bool DeleteStation(Line&);     //удаление станиции
bool FindTransfer(Line&);      //поиск станции, с которой есть переход на данную станцию
bool GetType(Line&);           //вывод типа станций
bool DeleteTransfer(Line&);    //удаление пересадок
bool ChangeStationName(Line&); //изменение имени станции
bool SaveFile(Line&);          //запись в файл
bool ReadFile(Line&);          //чтение из файла
bool CheckStations(Line&);     //проверить корректность описания станций (конкретно - проверка правильности кол-ва пересадок)

// ввод числа из входного потока i
template <class T>
void getNum(T &a)
{
	cin >> a;
	while (!cin.good())
	{
		getchar();
		cin.clear();
		cin >> a;
	}
}

string getStr(); //чтение строки с пробелами

int main()
{
	Line L = CreateNew();
	bool ex = false;
	size_t choice = 0;
	Menu(L);
	bool(*func[amount]) (Line&) = { Exit, Menu, Print, SetName, AddStation, AddTransfer, FindStation, DeleteStation, FindTransfer, 
		GetType, DeleteTransfer, ChangeStationName, SaveFile, ReadFile, CheckStations};					//массив функций

	while (!ex)
	{																//цикл выбора пукнта
		cout << "Make your choice: ";
		getNum(choice);
		while (choice >= amount) //пока не ввели корректный номер пункта
			getNum(choice);
		try
		{
			ex = (*func[choice])(L);
		}
		catch (const exception& e)
		{
			cout << "Error: " << e.what() << endl;
		}
		catch (...)
		{
			cout << "Unknown error" << endl;
		}
	}

	return 0;
}

//создание новой линии
Line CreateNew()
{
	cout << "Enter name of line: ";
	string name = getStr();
	return Line(name);
}

//завершение программы
bool Exit(Line& L)
{
	return true;
}

//вывод меню
bool Menu(Line&)
{
	for (size_t i = 0; i < amount; ++i)
		cout << i << " " << msgs[i] << endl;
	return false;
}

//вывод плана
bool Print(Line& L)
{
	cout << L;
	return false;
}

//изменение имени
bool SetName(Line& L)
{
	cout << "Enter new name of line: ";
	string name = getStr();
	L.setName(name);
	cout << "Name changed successfull" << endl;
	return false;
}

//добавление станции
bool AddStation(Line& L)
{
	cout << "Enter position of new station: ";
	size_t position;
	getNum(position);
	cout << "Enter name of station: ";
	string name = getStr();
	L.addStation(name, position);
	cout << "Station added successfull " << endl;
	return false;
}

//добавление пересадки
bool AddTransfer(Line& L)
{
	cout << "Enter position of station: ";			
	size_t position; //ввод позиции станции
	getNum(position);
	cout << "Enter name of station: ";
	string station = getStr(); //ввод имени станции
	cout << "Enter name of line: ";
	string line = getStr(); //ввод имени линии, куда будет происходить персадка
	L.addTransfer(position, station, line);
	cout << "Station added successfull" << endl;
	return false;
}

//поиск станции
bool FindStation(Line& L)
{
	cout << "Enter name of station: ";
	string station = getStr();
	const Station* S = L.find(station);
	if (S)
		cout << *S;
	else
		cout << "Station not found" << endl;
	return false;
}

//удаление станции
bool DeleteStation(Line& L)
{
	cout << "Enter name of station: ";			
	string station = getStr(); //удаление станции
	L.deleteStation(station);
	cout << "Station deleted" << endl;
	return false;
}

//поиск пересадки на даннцю станцию
bool FindTransfer(Line& L)
{
	cout << "Enter name of station: ";			
	string station = getStr();
	const Station* S = L.findTransfer(station);
	if (S)
		cout << *S;
	else
		cout << "Station not found" << endl;
	return false;
}

//получение имени типа данной станции
bool GetType(Line& L)
{
	cout << "Enter name of station: ";
	string station = getStr();
	cout << "Type is: " << L.getType(station) << endl;
	return false;
}

//удалить заданную пересадку с заданной номером станции
bool DeleteTransfer(Line& L)
{
	cout << "Enter position of station: ";
	size_t stationPosition;
	getNum(stationPosition);
	cout << "Enter position of transfer: ";
	size_t transferPosition;
	getNum(transferPosition);
	L.deleteTransfer(stationPosition, transferPosition);
	cout << "Transfer deleted" << endl;
	return false;
}

//изменение имени станции
bool ChangeStationName(Line& L)
{
	cout << "Enter position of station: ";
	size_t position;
	getNum(position);
	cout << "Enter name of station: ";
	string name = getStr();
	L.changeStationName(position, name);
	cout << "Name changed" << endl;
	return false;
}

//сохранить в файл
bool SaveFile(Line& L)
{
	cout << "Enter file name: ";
	string file = getStr();
	ofstream o;
	o.open(file);
	L.save(o);
	cout << "File wrote successfull" << endl;
	return false;
}

//считать из файла
bool ReadFile(Line& L)
{
	cout << "Enter file name: ";
	string file = getStr();
	ifstream i;
	i.open(file);
	if (i)
	{
		L.read(i);
		cout << "File read" << endl;
	}
	else
	{
		cout << "File not found" << endl;
	}
	return false;
}

//проверка корректности количествапересадок на станции с помощью итератора
bool CheckStations(Line& L)
{
	for (Line::const_iterator it = L.begin(); it != L.end(); it++)
		if ((*it)->correct())
			cout << (*it)->name() << " is correct." << endl;
		else
			cout << (*it)->name() << " is incorrect." << endl;
	return false;
}

string getStr()																//чтение строки
{
	string s = "";
	getline(cin, s);
	if (s.length() == 0)
	{
		cin.clear();
		getline(cin, s);
	}
	return s;
}
