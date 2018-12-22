#include "StationNT.h"

#include <fstream>

using namespace Library;
using namespace std;

StationNT::StationNT(std::string name, size_t count, const std::string* lines, const std::string* stations)
	: StationUT(name, count, lines)
{
	for (size_t i = 0; i < count; ++i)
		m_stations[i] = stations[i];
}

std::ostream& StationNT::print(std::ostream& o) const
{
	o.width(20);
	o << left << m_name << "Type is: StationNT" << endl;
	o << " " << m_count << " transfers: " << endl;
	for (size_t i = 0; i < m_count; ++i)
		o << " Line: " << m_lines[i] << ", station: " << m_stations[i] << "." << endl;
	return o;
}

//добавление пересадки
Station* StationNT::addTransfer(std::string station, std::string line) const
{
	if (m_count == 3)
		throw overflow_error("too much transfers");
	string lines[MAX_COUNT];
	string stations[MAX_COUNT];
	for (size_t i = 0; i < m_count; ++i)
	{
		lines[i] = m_lines[i];
		stations[i] = m_stations[i];
	}
	lines[m_count] = line;
	stations[m_count] = station;
	return new StationNT(m_name, m_count + 1, m_lines, m_stations);
}

bool StationNT::consistTransfer(std::string name) const
{
	for (size_t i = 0; i < m_count; ++i)
		if (m_stations[i] == name)
			return true;
	return false;
}

std::string StationNT::getType() const 
{ 
	return "StationNT"; 
}

//удаление пересадки, возможны возвраты разных типов обеъектов
Station* StationNT::deleteTransfer(size_t position) const
{
	if (position >= m_count)
		throw out_of_range("incorrect transfer");
	if (m_count == 1) //если это была последняя пересадка
	{
		return new Station(m_name); //то возвращаем обычную станцию
	}
	else
	{
		string stations[MAX_COUNT];
		string lines[MAX_COUNT];
		//копируем начало спсика пересадок
		for (size_t i = 0; i < position; ++i)
		{
			stations[i] = m_stations[i];
			lines[i] = m_lines[i];
		}
		//копируем со сдвигом часть после удалённой станции
		for (size_t i = position; i < m_count - 1; ++i)
		{
			stations[i] = m_stations[i + 1];
			lines[i] = m_lines[i + 1];
		}
		//теперь надо проверить, осталась ли станция пересадочным узлом 
		//(когда есть пересадки на станции с другими именами), или же теперь 
		//это просто станция с пересадками на одноимённые станции
		for (size_t i = 0; i < m_count - 1; ++i)
			if (stations[i] != m_name) //если есть хотя бы одна станция с отличным именем - то это пересадочный узел
				return new StationNT(m_name, m_count - 1, lines, stations);
		//если все пересадки на одноимённые, то вернули станцию с пересадками на одноимённые
		return new StationUT(m_name, m_count - 1, lines);
	}
}

Station* StationNT::setName(string name) const
{
	//если есть хотя бы одна станция с отличным именем пересадки, 
	//то объект останется пересадочным узлом
	for (size_t i = 0; i < m_count; ++i)
		if (m_stations[i] != name)
			return new StationNT(name, m_count, m_lines, m_stations);
	//иначе вернём станцию с одноимёнными пересадками, т.к. все имена теперь совпадают с новым
	//именем станции
	return new StationUT(name, m_count, m_lines);
}

void StationNT::save(ofstream& o) const
{
	o << m_name << endl << m_count << endl;
	for (size_t i = 0; i < m_count; ++i)
		o << m_stations[i] << endl << m_lines[i] << endl;
}