#pragma once

#include "Station.h"

namespace Library
{
	//класс станций с пересадкой на одноимённую
	class StationUT : public Station
	{
	protected:
		static const size_t MAX_COUNT = 3; //максимальное число пересадок
		size_t m_count;                    //число пересадок
		std::string m_lines[MAX_COUNT];    //массив из трёх названий линии

		virtual std::ostream& print(std::ostream&) const; //перегружаемая функция для вывода
	public:
		//StationUT() :Station("") , count (0) {} //пустой конструктор
		StationUT(std::string name, size_t count, const std::string* lines); //конструктор по имени и массиву линий
		virtual Station* clone() const { return new StationUT(*this); } //функция клонирования

		virtual bool check(std::string name) const; //проверка отсутствия пересадок на линии с таким именем

		//добавить пересадку, возвращает указатель на новую станцию другого типа, которой нужно заменить старую 
		virtual Station* addTransfer(std::string station, std::string line) const;
		virtual std::string getType() const;              //возвращает строку типа станции
		virtual Station* deleteTransfer(size_t position) const; //удаляет пересадку (генерирует исключение для простой станции)
		virtual Station* setName(std::string name) const; //изменить имя станции - возвращает указатель на новую станцию 
													//(т.к. может измениться тип станции)
		virtual void save(std::ofstream& o) const; //сохранение в файл
		virtual bool correct() const; //проверяет корректность количества пересадок
	};
}