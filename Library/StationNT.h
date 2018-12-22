#pragma once

#include "StationUT.h"


namespace Library
{
	//класс станций с пересадкой на станции с другим названием
	class StationNT : public StationUT
	{
		std::string m_stations[3]; //массив названий станций пересадок (массив линий наследуется от родителя)

		virtual std::ostream& print(std::ostream&) const; //перегружаемая функция для вывода
	public:
		StationNT(std::string name, size_t count, const std::string* lines, const std::string* stations); //конструктор с аргументами
		virtual Station* clone() const { return new StationNT(*this); } //копирование

		//добавить пересадку, возвращает указатель на новую станцию другого типа, которой нужно заменить старую 
		virtual Station* addTransfer(std::string station, std::string line) const;
		virtual bool consistTransfer(std::string name) const; //проверяет наличии пересадки на станцию с заданным именем
		virtual std::string getType() const;              //возвращает строку типа станции
		virtual Station* deleteTransfer(size_t position) const; //удаляет пересадку (возвращает указатель на новый объект)
		virtual Station* setName(std::string name) const; //изменить имя станции - возвращает указатель на новую станцию 
													//(т.к. может измениться тип станции)
		virtual void save(std::ofstream& o) const; //сохранение в файл
	};
}