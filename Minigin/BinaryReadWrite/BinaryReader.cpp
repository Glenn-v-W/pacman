#include "MiniginPCH.h"
#include "BinaryReader.h"

BinaryReader::BinaryReader()
	:m_In{}
{
}
BinaryReader::~BinaryReader()
{
	if (m_In.is_open())
		Close();
}

bool BinaryReader::Open(const std::string& fileName)
{
	m_In.open(fileName.c_str(), std::ios::in | std::ios::binary);
	return m_In.is_open();
}
void BinaryReader::Close()
{
	m_In.close();
}