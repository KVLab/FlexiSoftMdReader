#include "markdown.h"

#include <windows.h>
#include <shlwapi.h>
#include <string>
#include <vector>

#pragma comment(lib, "shlwapi.lib")

namespace
{
const DWORD kMaximumMarkdownFileSize = 64 * 1024 * 1024;

enum ListType { ListNone, ListUnordered, ListOrdered };

bool IsSpace(wchar_t value)
{
    return value == L' ' || value == L'\t' || value == L'\r';
}

bool IsFence(const std::wstring& value)
{
    const wchar_t tick = static_cast<wchar_t>(96);
    return value.length() >= 3 && value[0] == tick && value[1] == tick && value[2] == tick;
}

std::wstring Trim(const std::wstring& value)
{
    std::wstring::size_type first = 0;
    std::wstring::size_type last = value.length();
    while (first < last && IsSpace(value[first])) ++first;
    while (last > first && IsSpace(value[last - 1])) --last;
    return value.substr(first, last - first);
}

std::wstring RemoveTrailingCarriageReturn(const std::wstring& value)
{
    if (!value.empty() && value[value.length() - 1] == L'\r')
        return value.substr(0, value.length() - 1);
    return value;
}

bool IsHorizontalRule(const std::wstring& value)
{
    std::wstring line = Trim(value);
    std::wstring::size_type index;
    if (line.length() < 3) return false;
    for (index = 0; index < line.length(); ++index)
    {
        if (line[index] != L'-') return false;
    }
    return true;
}

bool IsPipeDelimitedRow(const std::wstring& value)
{
    std::wstring line = Trim(value);
    return line.length() >= 2 && line[0] == L'|' && line[line.length() - 1] == L'|';
}
bool StartsWith(const std::wstring& value, const wchar_t* prefix)
{
    std::wstring::size_type index = 0;
    while (prefix[index] != L'\0')
    {
        if (index >= value.length() || value[index] != prefix[index]) return false;
        ++index;
    }
    return true;
}

void AppendEscaped(std::wstring* result, wchar_t value)
{
    switch (value)
    {
    case L'&': result->append(L"&amp;"); break;
    case L'<': result->append(L"&lt;"); break;
    case L'>': result->append(L"&gt;"); break;
    case L'"': result->append(L"&quot;"); break;
    case L'\'': result->append(L"&#39;"); break;
    default: result->push_back(value); break;
    }
}

std::wstring EscapeHtml(const std::wstring& value)
{
    std::wstring result;
    std::wstring::size_type index;
    for (index = 0; index < value.length(); ++index) AppendEscaped(&result, value[index]);
    return result;
}

std::wstring EscapeCssFontFamily(const std::wstring& value)
{
    std::wstring result;
    std::wstring::size_type index;
    for (index = 0; index < value.length(); ++index)
    {
        switch (value[index])
        {
        case L'\\': result.append(L"\\\\"); break;
        case L'\'': result.append(L"\\'"); break;
        case L'\r':
        case L'\n': break;
        default: result.push_back(value[index]); break;
        }
    }
    return result;
}

std::wstring MakeCssFontStack(const std::wstring& fontFamily)
{
    return std::wstring(L"'") + EscapeCssFontFamily(fontFamily) +
           L"',Tahoma,Arial,sans-serif";
}
std::vector<std::wstring> SplitLines(const std::wstring& value)
{
    std::vector<std::wstring> result;
    std::wstring::size_type start = 0;
    std::wstring::size_type end;
    while (start <= value.length())
    {
        end = value.find(L'\n', start);
        if (end == std::wstring::npos)
        {
            result.push_back(value.substr(start));
            break;
        }
        result.push_back(value.substr(start, end - start));
        start = end + 1;
    }
    return result;
}

std::wstring MarkdownDirectory(const std::wstring& filePath)
{
    wchar_t fullPath[MAX_PATH];
    DWORD length = GetFullPathNameW(filePath.c_str(), MAX_PATH, fullPath, NULL);
    std::wstring path = (length == 0 || length >= MAX_PATH) ? filePath : fullPath;
    std::wstring::size_type slash = path.find_last_of(L"\\/");
    return slash == std::wstring::npos ? L"." : path.substr(0, slash);
}

bool IsSupportedImage(const std::wstring& path)
{
    std::wstring::size_type dot = path.find_last_of(L'.');
    if (dot == std::wstring::npos) return false;
    const wchar_t* extension = path.c_str() + dot;
    return lstrcmpiW(extension, L".png") == 0 || lstrcmpiW(extension, L".jpg") == 0 ||
           lstrcmpiW(extension, L".jpeg") == 0 || lstrcmpiW(extension, L".gif") == 0 ||
           lstrcmpiW(extension, L".bmp") == 0;
}

bool IsDrivePath(const std::wstring& value)
{
    return value.length() >= 3 &&
        ((value[0] >= L'A' && value[0] <= L'Z') || (value[0] >= L'a' && value[0] <= L'z')) &&
        value[1] == L':' && (value[2] == L'\\' || value[2] == L'/');
}

bool IsSafeLocalPath(const std::wstring& value)
{
    std::wstring::size_type colon = value.find(L':');
    if (value.empty() || StartsWith(value, L"//") || StartsWith(value, L"\\\\") ||
        StartsWith(value, L"file:") || StartsWith(value, L"FILE:"))
        return false;
    if (colon != std::wstring::npos && !IsDrivePath(value)) return false;
    return value[0] != L'\\' && value[0] != L'/';
}

bool ResolveImagePath(const std::wstring& directory, const std::wstring& target,
                      std::wstring* localPath)
{
    wchar_t combined[MAX_PATH];
    wchar_t canonical[MAX_PATH];
    std::wstring value = Trim(target);
    if (value.length() >= 2 && value[0] == L'<' && value[value.length() - 1] == L'>')
        value = value.substr(1, value.length() - 2);
    if (!IsSafeLocalPath(value) ||
        PathCombineW(combined, directory.c_str(), value.c_str()) == NULL ||
        !PathCanonicalizeW(canonical, combined))
        return false;
    *localPath = canonical;
    return true;
}

bool FileExists(const std::wstring& filePath)
{
    DWORD attributes = GetFileAttributesW(filePath.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

std::wstring Placeholder(const std::wstring& target, const wchar_t* reason)
{
    return std::wstring(L"<span class=\"image-missing\">[") + reason + L": " +
           EscapeHtml(target) + L"]</span>";
}

std::wstring RenderImage(const std::wstring& alt, const std::wstring& target,
                         const std::wstring& directory)
{
    wchar_t url[MAX_PATH * 3];
    DWORD urlLength = sizeof(url) / sizeof(url[0]);
    std::wstring path;

    if (!ResolveImagePath(directory, target, &path)) return Placeholder(target, L"Image blocked");
    if (!IsSupportedImage(path)) return Placeholder(target, L"Image unsupported");
    if (!FileExists(path)) return Placeholder(target, L"Image missing");
    if (FAILED(UrlCreateFromPathW(path.c_str(), url, &urlLength, 0)))
        return Placeholder(target, L"Image unavailable");

    return std::wstring(L"<img class=\"markdown-image\" src=\"") + EscapeHtml(url) +
           L"\" alt=\"" + EscapeHtml(alt) + L"\" />";
}

std::wstring InlineHtml(const std::wstring& value, const std::wstring& directory)
{
    std::wstring result;
    std::wstring::size_type index = 0;
    const wchar_t tick = static_cast<wchar_t>(96);

    while (index < value.length())
    {
        if (index + 2 < value.length() && value[index] == L'!' && value[index + 1] == L'[')
        {
            std::wstring::size_type closeBracket = value.find(L']', index + 2);
            if (closeBracket != std::wstring::npos && closeBracket + 1 < value.length() &&
                value[closeBracket + 1] == L'(')
            {
                std::wstring::size_type closeParen = value.find(L')', closeBracket + 2);
                if (closeParen != std::wstring::npos)
                {
                    result.append(RenderImage(value.substr(index + 2, closeBracket - index - 2),
                                              value.substr(closeBracket + 2, closeParen - closeBracket - 2),
                                              directory));
                    index = closeParen + 1;
                    continue;
                }
            }
        }

        if (value[index] == L'[')
        {
            std::wstring::size_type closeBracket = value.find(L']', index + 1);
            if (closeBracket != std::wstring::npos && closeBracket + 1 < value.length() &&
                value[closeBracket + 1] == L'(')
            {
                std::wstring::size_type closeParen = value.find(L')', closeBracket + 2);
                if (closeParen != std::wstring::npos)
                {
                    result.append(EscapeHtml(value.substr(index + 1, closeBracket - index - 1)));
                    result.append(L" (");
                    result.append(EscapeHtml(value.substr(closeBracket + 2, closeParen - closeBracket - 2)));
                    result.append(L")");
                    index = closeParen + 1;
                    continue;
                }
            }
        }

        if (index + 3 < value.length() && value[index] == L'*' && value[index + 1] == L'*')
        {
            std::wstring::size_type closeBold = value.find(L"**", index + 2);
            if (closeBold != std::wstring::npos && closeBold > index + 2)
            {
                result.append(L"<strong>");
                result.append(InlineHtml(value.substr(index + 2, closeBold - index - 2), directory));
                result.append(L"</strong>");
                index = closeBold + 2;
                continue;
            }
        }

        if (value[index] == tick)
        {
            std::wstring::size_type closeTick = value.find(tick, index + 1);
            if (closeTick != std::wstring::npos)
            {
                result.append(L"<code>");
                result.append(EscapeHtml(value.substr(index + 1, closeTick - index - 1)));
                result.append(L"</code>");
                index = closeTick + 1;
                continue;
            }
        }

        AppendEscaped(&result, value[index++]);
    }
    return result;
}

bool UnorderedItem(const std::wstring& value, std::wstring* item)
{
    if (value.length() >= 2 && (value[0] == L'-' || value[0] == L'*') && IsSpace(value[1]))
    {
        *item = Trim(value.substr(2));
        return true;
    }
    return false;
}

bool OrderedItem(const std::wstring& value, std::wstring* item)
{
    std::wstring::size_type index = 0;
    while (index < value.length() && value[index] >= L'0' && value[index] <= L'9') ++index;
    if (index == 0 || index + 1 >= value.length() || value[index] != L'.' || !IsSpace(value[index + 1]))
        return false;
    *item = Trim(value.substr(index + 2));
    return true;
}

bool TableSeparator(const std::wstring& value)
{
    std::wstring line = Trim(value);
    std::wstring::size_type index;
    bool dash = false;
    if (line.find(L'|') == std::wstring::npos) return false;
    for (index = 0; index < line.length(); ++index)
    {
        if (line[index] == L'-') dash = true;
        else if (line[index] != L'|' && line[index] != L':' && line[index] != L' ' && line[index] != L'\t')
            return false;
    }
    return dash;
}

std::vector<std::wstring> SplitTableCells(const std::wstring& value)
{
    std::wstring line = Trim(RemoveTrailingCarriageReturn(value));
    std::vector<std::wstring> cells;
    std::wstring::size_type start;
    std::wstring::size_type separator;

    if (!line.empty() && line[0] == L'|') line.erase(0, 1);
    if (!line.empty() && line[line.length() - 1] == L'|') line.erase(line.length() - 1);
    start = 0;
    while (start <= line.length())
    {
        separator = line.find(L'|', start);
        if (separator == std::wstring::npos)
        {
            cells.push_back(Trim(line.substr(start)));
            break;
        }
        cells.push_back(Trim(line.substr(start, separator - start)));
        start = separator + 1;
    }
    return cells;
}

void AppendTableRow(std::wstring* html, const std::wstring& line,
                    const std::wstring& directory, bool header)
{
    const std::vector<std::wstring> cells = SplitTableCells(line);
    const wchar_t* tag = header ? L"th" : L"td";
    std::vector<std::wstring>::size_type index;

    html->append(L"<tr>");
    for (index = 0; index < cells.size(); ++index)
    {
        html->append(L"<");
        html->append(tag);
        html->append(L">");
        html->append(InlineHtml(cells[index], directory));
        html->append(L"</");
        html->append(tag);
        html->append(L">");
    }
    html->append(L"</tr>\n");
}

bool IsTableDataLine(const std::wstring& value)
{
    return !Trim(value).empty() && value.find(L'|') != std::wstring::npos;
}
struct HeadingEntry
{
    int level;
    std::wstring text;
    std::wstring anchorId;
};

bool ParseHeading(const std::wstring& value, int* level, std::wstring* text)
{
    std::wstring line = Trim(value);
    std::wstring::size_type markerCount = 0;

    while (markerCount < line.length() && line[markerCount] == L'#')
        ++markerCount;
    if (markerCount < 1 || markerCount > 3 || markerCount >= line.length() ||
        !IsSpace(line[markerCount]))
        return false;

    *level = static_cast<int>(markerCount);
    *text = Trim(line.substr(markerCount + 1));
    return true;
}

std::wstring MakeHeadingAnchor(std::vector<HeadingEntry>::size_type index)
{
    wchar_t buffer[32];
    wsprintfW(buffer, L"heading-%u", static_cast<unsigned int>(index + 1));
    return buffer;
}

std::vector<HeadingEntry> CollectHeadings(const std::vector<std::wstring>& lines)
{
    std::vector<HeadingEntry> headings;
    std::vector<std::wstring>::size_type index;
    bool insideFence = false;

    for (index = 0; index < lines.size(); ++index)
    {
        int level;
        std::wstring text;
        std::wstring trimmed = Trim(lines[index]);

        if (IsFence(trimmed))
        {
            insideFence = !insideFence;
            continue;
        }
        if (insideFence || !ParseHeading(trimmed, &level, &text))
            continue;

        HeadingEntry entry;
        entry.level = level;
        entry.text = text;
        entry.anchorId = MakeHeadingAnchor(headings.size());
        headings.push_back(entry);
    }
    return headings;
}

void AppendContentsMenu(std::wstring* html, const std::vector<HeadingEntry>& headings,
                        const std::wstring& directory)
{
    std::vector<HeadingEntry>::size_type index;

    html->append(L"<div class=\"contents-title\">Contents</div><ul class=\"contents-list\">");
    for (index = 0; index < headings.size(); ++index)
    {
        html->append(L"<li class=\"contents-level-");
        html->push_back(static_cast<wchar_t>(L'0' + headings[index].level));
        html->append(L"\"><a href=\"#");
        html->append(headings[index].anchorId);
        html->append(L"\">");
        html->append(InlineHtml(headings[index].text, directory));
        html->append(L"</a></li>");
    }
    html->append(L"</ul>");
}
void CloseList(std::wstring* html, ListType* type)
{
    if (*type == ListUnordered) html->append(L"</ul>\n");
    if (*type == ListOrdered) html->append(L"</ol>\n");
    *type = ListNone;
}
}

namespace markdown
{
bool LoadUtf8File(const std::wstring& filePath, std::wstring* text, std::wstring* errorMessage)
{
    HANDLE file;
    DWORD high = 0;
    DWORD low;
    DWORD read = 0;
    std::vector<char> bytes;
    std::vector<char>::size_type offset = 0;
    int wideLength;

    if (text == NULL || errorMessage == NULL) return false;
    text->clear();
    errorMessage->clear();

    file = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        *errorMessage = L"The Markdown file could not be opened.";
        return false;
    }

    low = GetFileSize(file, &high);
    if ((low == INVALID_FILE_SIZE && GetLastError() != NO_ERROR) || high != 0 || low > kMaximumMarkdownFileSize)
    {
        CloseHandle(file);
        *errorMessage = L"The Markdown file is too large or its size could not be read.";
        return false;
    }

    bytes.resize(low);
    if (low != 0 && (!ReadFile(file, &bytes[0], low, &read, NULL) || read != low))
    {
        CloseHandle(file);
        *errorMessage = L"The Markdown file could not be read completely.";
        return false;
    }
    CloseHandle(file);

    if (bytes.size() >= 3 && static_cast<unsigned char>(bytes[0]) == 0xEF &&
        static_cast<unsigned char>(bytes[1]) == 0xBB && static_cast<unsigned char>(bytes[2]) == 0xBF)
        offset = 3;
    if (bytes.size() == offset) return true;

    wideLength = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &bytes[offset],
                                     static_cast<int>(bytes.size() - offset), NULL, 0);
    if (wideLength == 0)
    {
        *errorMessage = L"The Markdown file is not valid UTF-8.";
        return false;
    }

    text->resize(wideLength);
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &bytes[offset],
                            static_cast<int>(bytes.size() - offset), &(*text)[0], wideLength) == 0)
    {
        text->clear();
        *errorMessage = L"The Markdown file is not valid UTF-8.";
        return false;
    }
    return true;
}

std::wstring ConvertToHtml(const std::wstring& markdownText, const std::wstring& markdownFilePath,
                               const std::wstring& fontFamily)
{
    const std::vector<std::wstring> lines = SplitLines(markdownText);
    const std::wstring directory = MarkdownDirectory(markdownFilePath);
    const std::wstring cssFontStack = MakeCssFontStack(fontFamily);
    const std::vector<HeadingEntry> headings = CollectHeadings(lines);
    const bool hasContents = !headings.empty();
    std::wstring html;
    std::vector<std::wstring>::size_type index = 0;
    std::vector<HeadingEntry>::size_type headingIndex = 0;
    ListType listType = ListNone;

    html.append(L"<!DOCTYPE html><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">");
    html.append(L"<style>body{font-family:");
    html.append(cssFontStack);
    html.append(L";margin:0;color:#202020;background:#fff;}"
                L"h1,h2,h3{color:#163a5f;}h1{font-size:1.7em;}h2{font-size:1.4em;}h3{font-size:1.15em;}"
                L"p{line-height:1.45;}hr{border:0;border-top:1px solid #9a9a9a;margin:1em 0;}code{font-family:Consolas,'Courier New',");
    html.append(cssFontStack);
    html.append(L";background:#f1f1f1;padding:1px 3px;}"
                L"pre.code{font-family:Consolas,'Courier New',");
    html.append(cssFontStack);
    html.append(L";white-space:pre-wrap;background:#f4f4f4;border:1px solid #d0d0d0;padding:10px;}"
                L"table.md-table{border-collapse:collapse;margin:1em 0;}table.md-table th,table.md-table td{border:1px solid #c8c8c8;padding:6px 8px;text-align:left;vertical-align:top;}table.md-table th{background:#e8e8e8;font-weight:bold;}"
                L"html{height:100%;}body.with-contents{height:100%;overflow:hidden;}.contents-column{position:absolute;left:0;top:0;bottom:0;width:220px;overflow:auto;background:#f3f3f3;border-right:1px solid #c8c8c8;padding:16px 12px;}.document-column{position:absolute;left:245px;right:0;top:0;bottom:0;overflow:auto;}.document-content{padding:20px;}.contents-title{font-weight:bold;color:#163a5f;text-transform:uppercase;margin-bottom:10px;}.contents-list{list-style-type:none;margin:0;padding:0;}.contents-list li{margin:0 0 6px 0;line-height:1.25;}.contents-list a{color:#202020;text-decoration:none;}.contents-list a:hover{text-decoration:underline;}.contents-level-2{margin-left:14px !important;}.contents-level-3{margin-left:28px !important;}"
                L".markdown-image{max-width:100%;height:auto;vertical-align:middle;}.image-missing{display:inline-block;color:#8a1f11;background:#fff1ef;border:1px solid #d9a39c;padding:3px 5px;font-family:Consolas,'Courier New',");
    html.append(cssFontStack);
    if (hasContents)
    {
        html.append(L";}</style></head><body class=\"with-contents\"><div class=\"contents-column\">");
        AppendContentsMenu(&html, headings, directory);
        html.append(L"</div><div class=\"document-column\"><div class=\"document-content\">\n");
    }
    else
    {
        html.append(L";}</style></head><body><div class=\"document-content\">\n");
    }

    while (index < lines.size())
    {
        std::wstring line = lines[index];
        std::wstring trimmed = Trim(line);
        std::wstring item;

        if (IsFence(trimmed))
        {
            CloseList(&html, &listType);
            ++index;
            html.append(L"<pre class=\"code\">");
            while (index < lines.size() && !IsFence(Trim(lines[index])))
            {
                html.append(EscapeHtml(RemoveTrailingCarriageReturn(lines[index])));
                if (index + 1 < lines.size() && !IsFence(Trim(lines[index + 1]))) html.append(L"\n");
                ++index;
            }
            html.append(L"</pre>\n");
            if (index < lines.size()) ++index;
            continue;
        }

        if (IsHorizontalRule(trimmed))
        {
            CloseList(&html, &listType);
            html.append(L"<hr />\n");
            ++index;
            continue;
        }

        if ((index + 1 < lines.size() && line.find(L'|') != std::wstring::npos &&
             TableSeparator(lines[index + 1])) || IsPipeDelimitedRow(line))
        {
            bool hasHeader = index + 1 < lines.size() && line.find(L'|') != std::wstring::npos &&
                             TableSeparator(lines[index + 1]);
            CloseList(&html, &listType);
            html.append(L"<table class=\"md-table\">");
            if (hasHeader)
            {
                html.append(L"<thead>");
                AppendTableRow(&html, line, directory, true);
                html.append(L"</thead>");
                index += 2;
            }
            html.append(L"<tbody>\n");
            while (index < lines.size() && IsTableDataLine(lines[index]))
            {
                AppendTableRow(&html, lines[index], directory, false);
                ++index;
            }
            html.append(L"</tbody></table>\n");
            continue;
        }

        if (trimmed.empty())
        {
            CloseList(&html, &listType);
            ++index;
            continue;
        }

        {
            int headingLevel;
            std::wstring headingText;
            if (ParseHeading(trimmed, &headingLevel, &headingText))
            {
                std::wstring anchorId = headingIndex < headings.size() ?
                    headings[headingIndex].anchorId : MakeHeadingAnchor(headingIndex);
                CloseList(&html, &listType);
                html.append(L"<h");
                html.push_back(static_cast<wchar_t>(L'0' + headingLevel));
                html.append(L" id=\"");
                html.append(anchorId);
                html.append(L"\">");
                html.append(InlineHtml(headingText, directory));
                html.append(L"</h");
                html.push_back(static_cast<wchar_t>(L'0' + headingLevel));
                html.append(L">\n");
                ++headingIndex;
                ++index;
                continue;
            }
        }

        if (UnorderedItem(trimmed, &item))
        {
            if (listType != ListUnordered)
            {
                CloseList(&html, &listType);
                html.append(L"<ul>\n");
                listType = ListUnordered;
            }
            html.append(L"<li>");
            html.append(InlineHtml(item, directory));
            html.append(L"</li>\n");
            ++index;
            continue;
        }

        if (OrderedItem(trimmed, &item))
        {
            if (listType != ListOrdered)
            {
                CloseList(&html, &listType);
                html.append(L"<ol>\n");
                listType = ListOrdered;
            }
            html.append(L"<li>");
            html.append(InlineHtml(item, directory));
            html.append(L"</li>\n");
            ++index;
            continue;
        }

        CloseList(&html, &listType);
        html.append(L"<p>");
        html.append(InlineHtml(trimmed, directory));
        html.append(L"</p>\n");
        ++index;
    }

    CloseList(&html, &listType);
    if (hasContents)
        html.append(L"</div></div>");
    else
        html.append(L"</div>");
    html.append(L"</body></html>");
    return html;
}
}
