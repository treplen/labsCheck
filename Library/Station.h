#pragma once

#include <string>
#include <exception>

namespace Library
{
	//базовый класс обычной станции
	class Station
	{
	protected:
		std::string m_name; //имя станции

		virtual std::ostream& print(std::ostream&) const; //перегружаемая функция для вывода
	public:
		Station(std::string name) : m_name(name) {} //конструктор по имени
		std::string name() const { return m_name; } //получение имени станции
		virtual Station* clone() const { return new Station(*this); } //метод получения копии объекта

		friend std::ostream& operator<< (std::ostream&, const Station&); //оператор вывода

		virtual bool check(std::string name) const { return true; } //проверяет отсутствие пересадок на линию с именем name

		//добавить пересадку, возвращает указатель на новую станцию другого типа, которой нужно заменить старую 
		virtual Station* addTransfer(std::string station, std::string line) const;
		virtual bool consistTransfer(std::string name) const;   //проверяет наличии пересадки на станцию с заданным именем
		virtual std::string getType() const;                    //возвращает строку типа станции
		virtual Station* deleteTransfer(size_t position) const; //удаляет пересадку (генерирует исключение для простой станции)
		virtual Station* setName(std::string name) const; //изменить имя станции - возвращает указатель на новую станцию 
		                                            //(т.к. может измениться тип станции)
		virtual void save(std::ofstream& o) const; //сохранение в файл
		virtual bool correct() const; //функция проверки корректности количества пересадок (для простой станции всегда вернёт true)
	};
}