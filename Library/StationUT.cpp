#include "StationUT.h"
#include "StationNT.h"

#include <fstream>

using namespace Library;
using namespace std;

StationUT::StationUT(string name, size_t count, const string* lines)
	: Station(name)
{
	if (count > 3)
		throw overflow_error("too much transfers");
	m_count = count;
	for (size_t i = 0; i < count; i++)
		m_lines[i] = lines[i];
}

std::ostream& StationUT::print(std::ostream& o) const
{
	o.width(20);
	o << left << m_name << "Type is: StationUT" << endl;
	o << " " << m_count << " transfers: " << endl;
	for (size_t i = 0; i < m_count; i++)
		o << " Line: " << m_lines[i] << ", station: " << m_name << "." << endl;
	return o;
}

//проверяем наличие пересадок на линию с заданным именем
bool StationUT::check(string name) const
{
	for (size_t i = 0; i < m_count; i++)
		if (m_lines[i] == name)
			return false;
	return true;
}

//добавить пересадку, вернёт указатель на объект, в котором будет добалена пересадка
Station* StationUT::addTransfer(std::string station, std::string line) const
{
	//при попытке добавить 4 пересадку - ошибка
	if (m_count == 3)
		throw overflow_error("too much transfers");
	//если добавление на одноименную станцию, то возвращем объект этого же класса,
	//но с большим числом пересадок
	//копируем массив линий, его передадим в конструктор
	string lines[MAX_COUNT];
	for (size_t i = 0; i < m_count; ++i)
		lines[i] = m_lines[i];
	lines[m_count] = line;
	if (m_name == station)
	{
		return new StationUT(m_name, m_count + 1, lines);
	}
	else //если же имя другое, то формируем пересадочный узел
	{
		string stations[MAX_COUNT];
		for (size_t i = 0; i < m_count; ++i)
			stations[i] = m_name;
		stations[m_count] = station;
		StationNT* s = new StationNT(m_name, m_count + 1, m_lines, stations);
		return s;
	}
}

string StationUT::getType() const
{
	return "StationUT";
}

//удалить пересадку
Station* StationUT::deleteTransfer(size_t position) const
{
	if (position >= m_count)
		throw out_of_range("incorrect transfer position");
	if (m_count == 1) //если это последняя пересадка, то вернём обыкновенную станцию
	{
		return new Station(m_name);
	}
	else //иначе - сдвигаем все линии и возвращаем станцию с пересадками
	{
		string lines[MAX_COUNT]; //формируем массив названий линий
		for (size_t i = 0; i < position; ++i)
			lines[i] = m_lines[i];
		for (size_t i = position; i < m_count - 1; ++i)
			lines[i] = m_lines[i + 1];
		return new StationUT(m_name, m_count - 1, lines);
	}
}

//изменить имя - в результате вернём пересадочный узел, т.к. имя станций, на которые осуществляем
//пересадки остались старые, а имя этой станции изменилось
Station* StationUT::setName(std::string name) const
{
	string stations[MAX_COUNT];
	for (size_t i = 0; i < m_count; i++)
		stations[i] = m_name;
	return new StationNT(name, m_count, m_lines, stations);
}

//сохранение в файл
void StationUT::save(ofstream& o) const
{
	o << m_name << endl << m_count << endl;
	for (size_t i = 0; i < m_count; i++)
		o << m_name << endl << m_lines[i] << endl;
}

bool StationUT::correct() const
{ 
	return m_count >= 1 && m_count <= 3; 
}