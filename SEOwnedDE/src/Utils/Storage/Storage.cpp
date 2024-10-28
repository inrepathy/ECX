#include "Storage.h"
#include <Windows.h>
#include <shlobj.h>

#include "../Assert/Assert.h"

bool AssureDirectory(const std::filesystem::path& path)
{
	if (!exists(path))
	{
		return create_directories(path);
	}

	return true;
}

void CStorage::Init(const std::string& folderName)
{
	m_WorkFolder = std::filesystem::current_path() / folderName;
	Assert(AssureDirectory(m_WorkFolder))

	m_ConfigFolder = m_WorkFolder / "Configs";
	Assert(AssureDirectory(m_ConfigFolder))
}
