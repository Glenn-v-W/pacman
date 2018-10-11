#pragma once
#include <string>
#include <fstream>
#include <vector>

class BinaryReader
{
public:
	explicit BinaryReader();
	~BinaryReader();

	bool Open(const std::string& fileName);
	void Close();

	template <typename T>
	void Read(T& toRead);
	template <typename T>
	void Read(std::basic_string<T>& toRead);
	template <typename T>
	void Read(std::vector<T>& toRead);

private:
	std::ifstream m_In;

};

template <typename T>
void BinaryReader::Read(T& toRead)
{
	if (m_In.is_open())
		m_In.read((char*)&toRead, sizeof(toRead));
}

template <typename T>
void BinaryReader::Read(std::basic_string<T>& toRead)
{
	if (m_In.is_open())
	{
		char toResize;
		m_In.read((char*)&toResize, sizeof(toResize));
		toRead.resize(toResize);
		m_In.read((char*)toRead.data(), sizeof(toRead.data()[0]) * toResize);
	}
}

template <typename T>
void BinaryReader::Read(std::vector<T>& toRead)
{
	if (m_In.is_open())
	{
		char toResize;
		m_In.read((char*)&toResize, sizeof(toResize));
		toRead.resize(toResize);
		for (int i{}; i < toResize; ++i)
			Read(toRead[i]);
	}
}