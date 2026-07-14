#include <windows.h>
#include <shellapi.h>
#include <string>
#include <vector>

#include "markdown.h"
#include "html_view.h"

#pragma comment(lib, "gdi32.lib")

namespace
{
const wchar_t kWindowClassName[] = L"FlexiSoftMdReaderWindow";
const wchar_t kApplicationName[] = L"FlexiSoftMdReader";
const wchar_t kDefaultFontFamily[] = L"Tahoma";

struct FontFamilyList
{
    std::vector<std::wstring> names;
};

bool ContainsFontFamily(const FontFamilyList& list, const std::wstring& name)
{
    std::vector<std::wstring>::size_type index;
    for (index = 0; index < list.names.size(); ++index)
    {
        if (lstrcmpiW(list.names[index].c_str(), name.c_str()) == 0)
            return true;
    }
    return false;
}

int CALLBACK CollectFontFamily(const LOGFONTW* logFont, const TEXTMETRICW* textMetric,
                               DWORD fontType, LPARAM parameter)
{
    FontFamilyList* list = reinterpret_cast<FontFamilyList*>(parameter);
    UNREFERENCED_PARAMETER(textMetric);
    UNREFERENCED_PARAMETER(fontType);
    if (list != NULL && !ContainsFontFamily(*list, logFont->lfFaceName))
        list->names.push_back(logFont->lfFaceName);
    return 1;
}

void GetFontFamilies(FontFamilyList* list)
{
    HDC deviceContext;
    LOGFONTW logFont;
    if (list == NULL) return;
    deviceContext = GetDC(NULL);
    if (deviceContext == NULL) return;
    ZeroMemory(&logFont, sizeof(logFont));
    logFont.lfCharSet = DEFAULT_CHARSET;
    EnumFontFamiliesExW(deviceContext, &logFont, CollectFontFamily,
                        reinterpret_cast<LPARAM>(list), 0);
    ReleaseDC(NULL, deviceContext);
}

bool HasFontFileExtension(const std::wstring& value)
{
    const wchar_t* extensions[] = { L".ttf", L".otf", L".ttc", L".fon" };
    std::vector<std::wstring>::size_type index;
    std::wstring::size_type extensionLength;

    for (index = 0; index < sizeof(extensions) / sizeof(extensions[0]); ++index)
    {
        extensionLength = lstrlenW(extensions[index]);
        if (value.length() >= extensionLength &&
            lstrcmpiW(value.c_str() + value.length() - extensionLength, extensions[index]) == 0)
            return true;
    }
    return false;
}

bool LooksLikeFontFilePath(const std::wstring& value)
{
    return value.find_first_of(L"\\/:") != std::wstring::npos || HasFontFileExtension(value);
}

static HICON CreateMdIcon(COLORREF color)
{
    HDC hdc = GetDC(NULL);
    HDC mem = CreateCompatibleDC(hdc);

    HBITMAP bmpColor = CreateCompatibleBitmap(hdc, 16, 16);
    HBITMAP bmpMask = CreateBitmap(16, 16, 1, 1, NULL);

    HBITMAP oldBmp = static_cast<HBITMAP>(SelectObject(mem, bmpColor));

    RECT rc = { 0, 0, 16, 16 };

    HBRUSH background = CreateSolidBrush(color);
    FillRect(mem, &rc, background);
    DeleteObject(background);

    SetBkMode(mem, TRANSPARENT);

    HFONT font = CreateFontW(
        -11,
        4, 0, 0,
        FW_HEAVY,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Tahoma"
    );

    HFONT oldFont = static_cast<HFONT>(SelectObject(mem, font));

    RECT textRect = { 0, 0, 16, 16 };

    SetTextColor(mem, RGB(0, 0, 0));

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0)
                continue;

            RECT shadowRect = textRect;
            OffsetRect(&shadowRect, dx, dy);

            DrawTextW(mem, L"MD", -1, &shadowRect,
                      DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    SetTextColor(mem, RGB(255, 255, 255));
    DrawTextW(mem, L"MD", -1, &textRect,
              DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(mem, oldFont);
    DeleteObject(font);

    SelectObject(mem, oldBmp);
    DeleteDC(mem);
    ReleaseDC(NULL, hdc);

    ICONINFO iconInfo;
    ZeroMemory(&iconInfo, sizeof(iconInfo));
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = bmpColor;
    iconInfo.hbmMask = bmpMask;

    HICON icon = CreateIconIndirect(&iconInfo);

    DeleteObject(bmpColor);
    DeleteObject(bmpMask);

    return icon;
}
class PrivateFont
{
public:
    PrivateFont() : m_registered(false) {}
    ~PrivateFont() { Release(); }

    void Select(const std::wstring& requestedFont, std::wstring* fontFamily)
    {
        DWORD attributes;
        FontFamilyList before;
        FontFamilyList after;
        std::vector<std::wstring>::size_type index;

        Release();
        *fontFamily = kDefaultFontFamily;
        if (requestedFont.empty()) return;

        attributes = GetFileAttributesW(requestedFont.c_str());
        if (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            GetFontFamilies(&before);
            if (AddFontResourceExW(requestedFont.c_str(), FR_PRIVATE, NULL) == 0)
                return;

            m_path = requestedFont;
            m_registered = true;
            GetFontFamilies(&after);
            for (index = 0; index < after.names.size(); ++index)
            {
                if (!ContainsFontFamily(before, after.names[index]))
                {
                    *fontFamily = after.names[index];
                    return;
                }
            }

            Release();
            return;
        }

        if (!LooksLikeFontFilePath(requestedFont))
            *fontFamily = requestedFont;
    }

private:
    PrivateFont(const PrivateFont&);
    PrivateFont& operator=(const PrivateFont&);

    void Release()
    {
        if (m_registered)
        {
            RemoveFontResourceExW(m_path.c_str(), FR_PRIVATE, NULL);
            m_registered = false;
            m_path.clear();
        }
    }

    std::wstring m_path;
    bool m_registered;
};

struct ApplicationWindow
{
    PrivateFont documentFont;
    HtmlView view;
    std::wstring html;
};

void ShowError(const wchar_t* message)
{
    MessageBoxW(NULL, message, kApplicationName, MB_OK | MB_ICONERROR);
}

bool IsExistingFile(const std::wstring& filePath)
{
    DWORD attributes = GetFileAttributesW(filePath.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    ApplicationWindow* applicationWindow =
        reinterpret_cast<ApplicationWindow*>(GetWindowLongW(window, GWL_USERDATA));

    switch (message)
    {
    case WM_CREATE:
        {
            CREATESTRUCTW* create = reinterpret_cast<CREATESTRUCTW*>(lParam);
            applicationWindow = reinterpret_cast<ApplicationWindow*>(create->lpCreateParams);
            SetWindowLongW(window, GWL_USERDATA, reinterpret_cast<LONG>(applicationWindow));
            if (applicationWindow == NULL || !applicationWindow->view.Create(window))
            {
                MessageBoxW(window,
                            L"The embedded HTML viewer could not be created. Microsoft Internet Explorer components are required.",
                            kApplicationName, MB_OK | MB_ICONERROR);
                return -1;
            }
            applicationWindow->view.SetHtml(applicationWindow->html);
        }
        return 0;

    case WM_SIZE:
        if (applicationWindow != NULL)
            applicationWindow->view.Resize();
        return 0;

    case WM_TIMER:
        if (applicationWindow != NULL &&
            applicationWindow->view.OnTimer(static_cast<UINT_PTR>(wParam)))
            return 0;
        break;

    case WM_DESTROY:
        if (applicationWindow != NULL)
            applicationWindow->view.Destroy();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(window, message, wParam, lParam);
}
}

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE previousInstance, LPWSTR commandLine, int showCommand)
{
    LPWSTR* arguments;
    int argumentCount;
    std::wstring markdownPath;
    std::wstring fontOption;
    std::wstring fontFamily;
    std::wstring markdownText;
    std::wstring errorMessage;
    HRESULT oleResult;
    WNDCLASSEXW windowClass;
    HWND window;
    HICON applicationIcon;
    MSG message;
    ApplicationWindow applicationWindow;

    UNREFERENCED_PARAMETER(previousInstance);
    UNREFERENCED_PARAMETER(commandLine);

    arguments = CommandLineToArgvW(GetCommandLineW(), &argumentCount);
    if (arguments == NULL || argumentCount < 2)
    {
        if (arguments != NULL) LocalFree(arguments);
        ShowError(L"No Markdown file was specified.\n\nUsage: FlexiSoftMdReader.exe <file.md>");
        return 1;
    }

    markdownPath = arguments[1];
    if (argumentCount >= 3)
        fontOption = arguments[2];
    LocalFree(arguments);
    if (!IsExistingFile(markdownPath))
    {
        ShowError(L"The specified Markdown file does not exist or is not a file.");
        return 1;
    }

    if (!markdown::LoadUtf8File(markdownPath, &markdownText, &errorMessage))
    {
        ShowError(errorMessage.c_str());
        return 1;
    }

    applicationWindow.documentFont.Select(fontOption, &fontFamily);
    applicationWindow.html = markdown::ConvertToHtml(markdownText, markdownPath, fontFamily);
    oleResult = OleInitialize(NULL);
    if (FAILED(oleResult))
    {
        ShowError(L"COM could not be initialized for the embedded HTML viewer.");
        return 1;
    }

    ZeroMemory(&windowClass, sizeof(windowClass));
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = instance;
    applicationIcon = CreateMdIcon(RGB(112, 112, 112));
    windowClass.hIcon = applicationIcon;
    windowClass.hIconSm = applicationIcon;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    windowClass.lpszClassName = kWindowClassName;

    if (RegisterClassExW(&windowClass) == 0)
    {
        ShowError(L"The application window class could not be registered.");
        if (applicationIcon != NULL) DestroyIcon(applicationIcon);
        OleUninitialize();
        return 1;
    }

    window = CreateWindowExW(0, kWindowClassName, kApplicationName,
                             WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                             CW_USEDEFAULT, CW_USEDEFAULT, 900, 700,
                             NULL, NULL, instance, &applicationWindow);
    if (window == NULL)
    {
        UnregisterClassW(kWindowClassName, instance);
        if (applicationIcon != NULL) DestroyIcon(applicationIcon);
        OleUninitialize();
        return 1;
    }

    ShowWindow(window, SW_SHOWMAXIMIZED);
    UpdateWindow(window);
    while (GetMessageW(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    UnregisterClassW(kWindowClassName, instance);
    if (applicationIcon != NULL) DestroyIcon(applicationIcon);
    OleUninitialize();
    return static_cast<int>(message.wParam);
}
