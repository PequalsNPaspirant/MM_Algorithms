#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert> //for assert()
#include <fstream>
using namespace std;

#include "Others/Others_GTS.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	struct packetHeader
	{
		uint16_t packetLen;
		uint16_t numMarketUpdates;
	};

	struct commonData
	{
		uint16_t len;
		char type;
		char symbol[5];
	};

	struct quote //total length = 20 bytes + dynamic data
	{
		//uint16_t quoteLen;
		//char type;
		//char symbol[5];
		//commonData cdata;
		uint16_t priceLevel;
		uint16_t priceLevelSize;
		uint64_t priceLevelPrice;
		//dynamic data of lenth (quoteLen - 20 bytes) will be ignored.
	};

	struct trade //total length = 18 + dynamic data
	{
		//uint16_t tradeLen;
		//char type;
		//char symbol[5];
		//commonData cdata;
		uint16_t tradeSize;
		uint64_t tradePrice;
		//dynamic data of lenth (tradeLen - 18 bytes) will be ignored.
	};

	uint16_t readMarketUpdate(unsigned char* start)
	{
		//commonData cdata;
		commonData* cdata = reinterpret_cast<commonData*>(start);
		cdata->len = (((uint16_t)start[0]) << 8) | ((uint16_t)start[1]);
cout << "\n" << "***DEBUGING*** Address: " << (void*)(&cdata->len) << " cdata.len: " << cdata->len;
cout << "\n" << "***DEBUGING*** Address: " << (void*)(&cdata->type) << " cdata.type: " << cdata->type;
cout << "\n" << "***DEBUGING*** Address: " << (void*)(&cdata->symbol) << " cdata.symbol: " << cdata->symbol;

		start += sizeof(commonData);

		if (cdata->type == 'T')
		{
			//trade* t = reinterpret_cast<trade*>(start);
			trade t;
			//t.cdata.len = (((uint16_t)start[0]) << 8) | ((uint16_t)start[1]);
			//cout << "\n" << "Address: " << (void*)(&t.cdata.len) << " len: " << t.cdata.len;
			//cout << "\n" << "Address: " << (void*)(&t.cdata.type) << " type: " << t.cdata.type;
			//cout << "\n" << "Address: " << (void*)(&t.cdata.symbol) << " symbol: " << t.cdata.symbol;
			t.tradeSize = (((uint16_t)start[0]) << 8) | ((uint16_t)start[1]);
			//cout << "\n***DEBUGING*** " << (0 + start[0]) << " " << (0 + start[1]);
cout << "\n" << "***DEBUGING*** Address: " << (void*)(&t.tradeSize) << " tradeSize: " << t.tradeSize;
			t.tradePrice = (((uint64_t)start[2]) << 56) |
				(((uint64_t)start[3]) << 48) |
				(((uint64_t)start[4]) << 40) |
				(((uint64_t)start[5]) << 32) |
				(((uint64_t)start[6]) << 24) |
				(((uint64_t)start[7]) << 16) |
				(((uint64_t)start[8]) << 8) |
				(((uint64_t)start[9]));

cout << "\n" << "***DEBUGING*** Address: " << (void*)(&t.tradePrice) << " tradePrice: " << t.tradePrice;

			string symbol(cdata->symbol, cdata->symbol + 5);
			auto it = symbol.find_first_of(' ');
			if (it != string::npos)
				symbol.erase(it);
			const unsigned int volumeLotSize = 100;
			unsigned int volume = t.tradeSize * volumeLotSize;
			double actualPrice = t.tradePrice / 10000.0;

			cout << "\nOutput: " << volume << " " << symbol << " @ " << std::fixed << std::setprecision(2) << actualPrice;
		}
		else if (cdata->type == 'T')
			cout << "\nThe current market data is quote. ignoring...";
		else
			cout << "\nERROR: unknown type: " << cdata->type;

		return cdata->len;
	}

	uint16_t readPacket(unsigned char* start)
	{
		//packetHeader* header = reinterpret_cast<packetHeader*>(start);
		packetHeader header;
		header.packetLen = (((uint16_t)start[0]) << 8) | ((uint16_t)start[1]);
		cout << "\n" << "***DEBUGING*** Address: " << (void*)(&header.packetLen) << " packetLen: " << header.packetLen;
		header.numMarketUpdates = (((uint16_t)start[2]) << 8) | ((uint16_t)start[3]);
		cout << "\n" << "***DEBUGING*** Address: " << (void*)(&header.numMarketUpdates) << " numMarketUpdates: " << header.numMarketUpdates;
		start += sizeof(packetHeader);

		for (int i = 0; i < header.numMarketUpdates; ++i)
		{
			//Read market data
			uint16_t length = readMarketUpdate(start);
			start += length;
			cin.get();
		}

		return header.packetLen == 0 ? sizeof(packetHeader) : header.packetLen;
	}

	void test_GTS()
	{
		cout << "\nStarted test...\n";

		string filePath("C:/@_Programming/GTS/input.dat");
		ifstream inputFile;

		try
		{
			inputFile.open(filePath, ios::binary | ios::ate);
		}
		catch (std::ifstream::failure &readErr)
		{
			cout << "\nERROR: Can not open file: " << filePath << endl;
			return;
		}
		catch (...)
		{
			cout << "\nUNKNOWN ERROR while opening file: " << filePath << endl;
			return;
		}

		if (!inputFile.is_open())
		{
			cout << "\nUNKNOWN ERROR while opening file: " << filePath << endl;
			return;
		}

		streampos size = inputFile.tellg();
		inputFile.seekg(0, ios::beg);
		//string streamData(size);
		char* data = new char[size];
		inputFile.read(data, size);

		//char packetLength[2];
		//packetHeader header;
		//while (inputFile.read(reinterpret_cast<char*>(&header), sizeof(header)))
		int position = 0;
		while (position < size)
		{
			uint16_t length = readPacket((unsigned char*)&data[position]);
			position += length;
		}

		delete[] data;
	}

	MM_DECLARE_FLAG(Others_GTS);

	MM_UNIT_TEST(Others_GTS_sanity, Others_GTS)
	{
		test_GTS();
	}
}