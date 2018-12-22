#pragma once

#include "Station.h"

#include <string>

#include <deque>

//пространтсво имён библиотеки
namespace Library
{
	class Line	//класс линии, состоит из имени линии и deque - 
		//контейнер на базе массива с быстрой вставкой в конец и начало 
		//(поскольку станции строят либо в конце, либо в начале линии)
	{
		std::string m_name;              //имя
		std::deque<Station*> m_stations; //дек станция
	public:
		class const_iterator;  //вложенный класс итератора
		friend const_iterator; //он дружествен

		Line(std::string name); //конструктор по имени
		Line(const Line& L);    //копирующий конструктор
		Line(Line&& L);         //перемещающий конструктор
		~Line();                //деструктор
		
		Line& operator= (Line&& L);     //операторы =
		Line& operator= (const Line& L);											
		friend std::ostream& operator<< (std::ostream&, const Line&); //вывод

		void setName(std::string name);  //изменить имя линии (проверяет корректность всех пересадок)
		std::string name() const { return m_name; }          //получить имя линии
		size_t length() const { return m_stations.size(); }  //получить длину линии (количество станций)
		void addStation(std::string name, size_t position);  //добавить станцию (имя станции и позиция на линии)
		const Station* find(std::string name) const;         //поиск станции по имени
		const Station* findTransfer(std::string name) const; //поиск пересадки по имени станции, на котрую происходит пересадка
		void addTransfer(size_t position, std::string station, std::string line); //добавить пересадку 
		                                                     //со станции на position, на station на линии line
		void deleteStation(std::string name);                //удалить станцию по имени
		std::string getType(std::string) const;              //получить тип станции по её имени
		void deleteTransfer(size_t station, size_t transfer);//удалить transfer пересадку со станции на заданной позиции
		void changeStationName(size_t position, std::string name); //изменить имя на заданное
		void save(std::ofstream& file) const;           //сохранение в файл
		void read(std::ifstream& file);                 //чтение из файла

		const_iterator begin() const { return const_iterator(*this, 0); } //получить итератор, указывающий на начало
		const_iterator end() const { return const_iterator(*this, m_stations.size()); } //итератор, указывающий на конец

		//вложенный класс итератора
		class const_iterator
		{
		private:
			const Line& L; //ссылка на линию
			size_t cur;    //текущее положение
		public:
			const_iterator(const Line& L, size_t cur) : L(L), cur(cur) {}
			const_iterator& operator++() //оператор ++
			{
				++cur;
				return *this;
			}
			const_iterator operator++(int)
			{
				const_iterator it(*this);
				++cur;
				return it;
			}
			bool operator!= (const const_iterator& I) const //неравенство
			{
				return cur != I.cur;
			}

			const Station* operator*() //получение значения по итератору
			{
				return L.m_stations[cur];
			}

		};
	};

	std::string getStr(std::ifstream& i); //получение строки из файла
}