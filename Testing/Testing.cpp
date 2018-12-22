#include "stdafx.h"
#include "gtest\gtest.h"
#include "Line.h"
#include "Station.h"
#include "StationUT.h"
#include "StationNT.h"

using namespace std;
using namespace Library;

#undef deque

TEST(StationTesting, Constructor)
{
	Station S2("station");
	ASSERT_STREQ("station", S2.name().c_str());
	Station* S3 = S2.clone();
	ASSERT_EQ("station", S3->name());
	ASSERT_EQ("Station", S3->getType());
	delete S3;
}

TEST(StationTesting, Print)
{
	Station S("name");
	ostringstream o;
	o << S;
	ASSERT_STREQ("name                    Type is: Station", o.str().c_str());
}

TEST(StationTesting, Check)
{
	Station S("station");
	ASSERT_TRUE(S.check("name"));
}

TEST(StationTesting, AddTransfer)
{
	Station S = Station("station");
	Station* S2 = S.addTransfer("station 2", "line");
	ASSERT_EQ(typeid(*S2), typeid(StationNT));
	delete S2;
	S2 = S.addTransfer("station", "line");
	ASSERT_EQ(typeid(*S2), typeid(StationUT));
	delete S2;
}

TEST(Testing, CheckAddTransfer)
{
	Station S = Station("S");
	Station* S2 = S.addTransfer("S2", "L1");
	ASSERT_EQ(typeid(*S2), typeid(StationNT));
	S = Station("S");
	S2 = S.addTransfer("S", "L2");
	ASSERT_EQ(typeid(*S2), typeid(StationUT));
	S2 = S2->addTransfer("S2", "L3");
	ASSERT_EQ(typeid(*S2), typeid(StationNT));
}

TEST(Testing, ChechDeleteTransfer)
{
	Station S = Station("S");
	Station* S1 = S.addTransfer("S", "L");
	Station* S2 = S1->deleteTransfer(0);
	ASSERT_EQ(typeid(*S2), typeid(Station));
	S = Station("S");
	S1 = S.addTransfer("S", "L");
	S1 = S1->addTransfer("S2", "L");
	ASSERT_EQ(typeid(*S1), typeid(StationNT));
	S1 = S1->deleteTransfer(1);
	ASSERT_EQ(typeid(*S1), typeid(StationUT));
	S1 = S1->deleteTransfer(0);
	ASSERT_EQ(typeid(*S1), typeid(Station));
}

TEST(Testing, ChangeName)
{
	Station* S = new Station("S");
	S = S->addTransfer("S2", "L");
	ASSERT_EQ(typeid(*S), typeid(StationNT));
	S = S->setName("S2");
	ASSERT_EQ(typeid(*S), typeid(StationUT));
}

TEST(LineTesting, Constructor)
{
	Line L("line");
	ASSERT_STREQ("line", L.name().c_str());
}

TEST(DequeTesting, CheckConstructor)				//тест конструкторов
{
	Deque <int> D = Deque <int>();
	ASSERT_EQ(D.size(), 0);
}

TEST(DequeTesting, CheckGet)
{
	deque <int> d = deque <int>();
	Deque <int> D = Deque <int>();
	for (int i = 0; i < 20; i++)
	{
		d.push_back(i);
		D.push_back(i);
		ASSERT_EQ(d.size(), D.size());
	}
}

TEST(DequeTesting, CheckIndex)
{
	Deque <int> D = Deque <int>();
	for (int i = 0; i < 20; i++)
	{
		D.push_back(i);
		ASSERT_EQ(D[i], i);
	}
	for (int i = 0; i < 20; i++)
		D[i] = i * 2;
	for (int i = 0; i < 20; i++)
	{
		ASSERT_EQ(D[i], i * 2);
	}
	ASSERT_ANY_THROW(D[-1]);
	ASSERT_ANY_THROW(D[D.size()]);
}

TEST(DequeTesting, CheckIterator)
{
	deque <int> d = deque <int>();
	Deque <int> D = Deque <int>();
	for (int i = 0; i < 20; i++)
	{
		d.push_back(i);
		D.push_back(i);
	}

	int i = 0;
	for (Deque<int>::iterator it = D.begin(); it != D.end(); it++, i++)
		ASSERT_EQ(*it, i);
}

TEST(DequeTesting, CheckInsertErase)
{
	Deque <int> D = Deque <int>();
	ASSERT_EQ(D.size(), 0);
	for (int i = 0; i < 20; i++)
		D.push_back(i);
	D.clear();
	ASSERT_EQ(D.size(), 0);
	for (int i = 0; i < 20; i++)
		D.push_back(i);
	Deque<int>::iterator it = D.end() + -1;
	for (int i = 19; i >= 0; i--)
	{
		it = D.erase(it);
		it = it + -1;
		ASSERT_EQ(D.size(), i);
		if (i)
		{
			ASSERT_EQ(*it, i - 1);
		}
	}
	for (int i = 0; i < 20; i++)
		D.push_back(i);
	it = D.begin();
	for (int i = 0; i < 20; i++)
	{
		it = D.erase(it);
		ASSERT_EQ(D.size(), 19 - i);
		if (i != 19)
		{
			ASSERT_EQ(*it, i + 1);
		}
	}
	D.clear();
	for (int i = 0; i < 20; i++)
	{
		D.push_front(i);
		ASSERT_EQ(D.size(), i + 1);
		ASSERT_EQ(*(D.begin()), i);
		ASSERT_EQ(*(D.end() + -1), 0);
	}
	for (unsigned int i = 0; i < 20; i++)
	{
		it = D.insert(D.begin() + 2 * i, 20 + i);
		ASSERT_EQ(D.size(), 21 + i);
		ASSERT_EQ(*it, 20 + i);
	}
	for (unsigned int i = 0; i < 20; i++)
	{
		ASSERT_EQ(D[2 * i], 20 + i);
		ASSERT_EQ(D[2 * i + 1], 19 - i);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}