#include "Station.h"
#include "StationUT.h"
#include "StationNT.h"

#include <fstream>

using namespace Library;
using namespace std;

ostream& Library::operator<< (ostream& o, const Station& S)
{
	return S.print(o);
}

ostream& Station::print(ostream& o) const
{
	o.width(20);
	o << left << m_name << "Type is: Station" << endl;
	return o;
}

Station* Station::addTransfer(string station, string line) const
{
	if (m_name == station)
		return new StationUT(m_name, 1, &line);
	else
		return new StationNT(m_name, 1, &line, &station);
}

bool Station::consistTransfer(string name) const 
{ 
	return false; 
}

string Station::getType() const
{
	return "Station";
}

Station* Station::deleteTransfer(size_t position) const
{ 
	throw runtime_error("no transfers."); 
}

Station* Station::setName(std::string name) const
{ 
	return new Station(name); 
}

void Station::save(ofstream& o) const
{
	o << m_name << endl << 0 << endl;
}

bool Station::correct() const 
{ 
	return true; 
}