#ifndef FLEXISOFT_MD_READER_MARKDOWN_H
#define FLEXISOFT_MD_READER_MARKDOWN_H

#include <string>

namespace markdown
{
    bool LoadUtf8File(const std::wstring& filePath,
                      std::wstring* text,
                      std::wstring* errorMessage);
    std::wstring ConvertToHtml(const std::wstring& markdownText,
                               const std::wstring& markdownFilePath,
                               const std::wstring& fontFamily);
}

#endif

