#include "Line.h"

#include <fstream>

using namespace Library;
using namespace std;

Line::Line(string name)
{
	m_name = name;
}

Line::Line(const Line& L)
{
	m_name = L.m_name;
	try
	{
		for (auto s : L.m_stations) //клонируем каждый элемент из контейнера другой линии
			m_stations.push_back(s->clone());
	}
	catch (const exception& e) //если при копировании возникло исключение
	{
		for (auto s : m_stations) //s имеет тип Station*
			delete s; //очищаем что накопировали
		throw e; //и выкидываем исключение дальше
	}
}

Line::Line(Line&& L)
{
	m_name = L.m_name;
	m_stations = L.m_stations;
	L.m_stations.clear();
}

Line::~Line()
{
	for(auto s : m_stations)
		delete s; //удаляем все станции в контейнере
}

Line& Line::operator= (const Line& L)
{
	if (this != &L)
	{
		m_name = L.m_name;
		deque<Station*> stations;
		try
		{
			for (auto s : L.m_stations) //клонируем каждый элемент из контейнера другой линии
				stations.push_back(s->clone());
			for (auto s : m_stations)
				delete s; //удаляем все станции, которые хранились
			m_stations.clear();
			m_stations = stations;
		}
		catch (const exception& e) //если при копировании возникло исключение
		{
			for (auto s : stations) //s - имеет тип Station*
				delete s; //очищаем что накопировали
			throw e; //и выкидываем исключение дальше
		}
	}
	return *this;
}

Line& Line::operator= (Line&& L)
{
	if (this != &L)
	{
		m_name = L.m_name;
		m_stations = L.m_stations;
		L.m_stations.clear();
	}
	return *this;
}


std::ostream& Library::operator<< (std::ostream& o, const Line& L)
{
	o << "Line: " << L.m_name << endl << endl;
	size_t i = 0;
	for (auto s : L.m_stations) //выводим все станции в линии (s имеет тип Station*)
		o << i++ << " " << *s << endl;
	return o;
}

//назначить имя линии - необходимо проверить, что новое имя не совпадает с именем линии
//на которую делаем пересадку
void Line::setName(std::string name)
{
	for (auto s : m_stations)
		if (!s->check(name))
			throw runtime_error("incorrect line name (name used as other line name)");
	m_name = name;
}

//добавить станцию
void Line::addStation(std::string name, size_t position)
{
	if (find(name))
		throw runtime_error("station exists");
	if (position < 0 || position > m_stations.size())
		throw out_of_range("incorrect position of station");
	m_stations.insert(m_stations.begin() + position, new Station(name)); 
	//вставляем новый элемент на нужную позицию со сдвигом остальных
}

//поиск линии
const Station* Line::find(std::string name) const
{
	for(auto s : m_stations)
		if (s->name() == name)
			return s;
	return nullptr;
}

//добавить пересадку
void Line::addTransfer(size_t position, std::string station, std::string line)
{
	if (line == m_name)
		throw runtime_error("incorrect line name");
	if (position < 0 || position >= m_stations.size())
		throw out_of_range("incorrect station position");
	Station* s = m_stations[position];
	m_stations[position] = s->addTransfer(station, line); //перезаписываем новой станцией
	delete s;
}

//удалить станцию
void Line::deleteStation(std::string name)
{
	for (auto it = m_stations.begin(); it != m_stations.end(); ++it)
		if ((*it)->name() == name)
		{
			delete *it;
			m_stations.erase(it);
			return;
		}
	throw runtime_error("station not found");
}

//поиск станции с пересадкой на заданную
const Station* Line::findTransfer(std::string name) const
{
	for(auto s : m_stations)
		if (s->consistTransfer(name))
			return s;
	return nullptr;
}

//возвращение типа станции
std::string Line::getType(std::string station) const
{
	if (find(station))
		return find(station)->getType();
	else
		throw runtime_error("station not found");
}

//удаление пересадки
void Line::deleteTransfer(size_t positionStation, size_t positionTransfer)
{
	if (positionStation >= m_stations.size())
		throw out_of_range("incorrect position");
	Station* s = m_stations[positionStation];
	m_stations[positionStation] = s->deleteTransfer(positionTransfer);
	delete s;
}

//изменить имя станции
void Line::changeStationName(size_t position, std::string name)
{
	if (position >= length())
		throw out_of_range("incorrect position");
	for(auto s : m_stations)
		if (s->name() == name)
			throw runtime_error("name exist");
	Station* s = m_stations[position];
	m_stations[position] = s->setName(name);
	delete s;
}

//сохранить в файл
void Line::save(ofstream& o) const
{
	o << m_name << endl;
	o << m_stations.size() << endl;
	for(auto s : m_stations)
		s->save(o);
}

//чтение из файла
void Line::read(ifstream& i)
{
	for(auto s : m_stations)
		delete s;
	m_stations.clear();
	m_name = getStr(i);
	size_t count;
	i >> count;
	for (size_t j = 0; j < count; ++j)
	{
		string name = getStr(i);
		addStation(name, j);
		size_t transfers;
		i >> transfers;
		for (size_t k = 0; k < transfers; ++k)
		{
			string station = getStr(i);
			string line = getStr(i);
			addTransfer(j, station, line);
		}
	}
}

string Library::getStr(ifstream& i)
{
	string s = "";
	getline(i, s);
	if (s.length() == 0)
	{
		i.clear();
		getline(i, s);
	}
	return s;
}