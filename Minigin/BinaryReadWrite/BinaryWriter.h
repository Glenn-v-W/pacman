#pragma once
#include <string>
#include <fstream>
#include <vector>

class BinaryWriter
{
public:
	explicit BinaryWriter();
	~BinaryWriter();

	bool Open(const std::string& fileName);
	void Close();

	template <typename T>
	void Write(const T& toWrite);
	template <typename T>
	void Write(const std::basic_string<T>& toWrite);
	template <typename T>
	void Write(const std::vector<T>& toWrite);

private:
	std::ofstream m_Out;

};

template <typename T>
void BinaryWriter::Write(const T& toWrite)
{
	if(m_Out.is_open())
		m_Out.write((const char*)&toWrite, sizeof(toWrite));
}

template <typename T>
void BinaryWriter::Write(const std::basic_string<T>& toWrite)
{
	if (m_Out.is_open())
	{
		const char size = (const char)toWrite.size();
		m_Out.write(&size, sizeof(size));
		m_Out.write((const char*)toWrite.data(), sizeof(toWrite.data()[0]) * size);
	}
}

template <typename T>
void BinaryWriter::Write(const std::vector<T>& toWrite)
{
	if (m_Out.is_open())
	{
		const char size = (const char)toWrite.size();
		m_Out.write(&size, sizeof(size));
		for (auto item : toWrite)
			Write(item);
	}
}