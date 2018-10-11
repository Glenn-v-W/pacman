#include "MiniginPCH.h"
#include "BinaryWriter.h"

BinaryWriter::BinaryWriter()
	:m_Out{}
{
}
BinaryWriter::~BinaryWriter()
{
	if (m_Out.is_open())
		Close();
}

bool BinaryWriter::Open(const std::string& fileName)
{
	m_Out.open(fileName.c_str(), std::ios::out | std::ios::binary | std::ios::app);
	return m_Out.is_open();
}
void BinaryWriter::Close()
{
	m_Out.close();
}