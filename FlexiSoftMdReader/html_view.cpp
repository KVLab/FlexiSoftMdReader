#include "html_view.h"

#include <mshtml.h>
#include <oleauto.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib")

namespace
{
const UINT_PTR kDocumentTimerId = 0x4D52;
}

HtmlView::HtmlView()
    : m_parentWindow(NULL),
      m_browser(NULL),
      m_oleObject(NULL),
      m_inPlaceObject(NULL),
      m_referenceCount(1),
      m_documentWritten(false)
{
}

HtmlView::~HtmlView()
{
    Destroy();
}

bool HtmlView::Create(HWND parentWindow)
{
    RECT rect;
    HRESULT result;
    VARIANT location;

    m_parentWindow = parentWindow;
    result = CoCreateInstance(CLSID_WebBrowser, NULL, CLSCTX_INPROC_SERVER,
                              IID_IWebBrowser2, reinterpret_cast<void**>(&m_browser));
    if (FAILED(result))
    {
        Destroy();
        return false;
    }

    result = m_browser->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&m_oleObject));
    if (FAILED(result))
    {
        Destroy();
        return false;
    }

    result = m_oleObject->SetClientSite(static_cast<IOleClientSite*>(this));
    if (FAILED(result))
    {
        Destroy();
        return false;
    }
    OleSetContainedObject(m_oleObject, TRUE);

    GetClientRect(m_parentWindow, &rect);
    result = m_oleObject->DoVerb(OLEIVERB_SHOW, NULL, static_cast<IOleClientSite*>(this),
                                 0, m_parentWindow, &rect);
    if (FAILED(result))
    {
        Destroy();
        return false;
    }

    m_browser->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_inPlaceObject));
    m_browser->put_Silent(VARIANT_TRUE);
    m_browser->put_RegisterAsBrowser(VARIANT_FALSE);
    m_browser->put_RegisterAsDropTarget(VARIANT_FALSE);
    ApplyObjectRect();

    VariantInit(&location);
    location.vt = VT_BSTR;
    location.bstrVal = SysAllocString(L"about:blank");
    result = location.bstrVal == NULL ? E_OUTOFMEMORY :
             m_browser->Navigate2(&location, NULL, NULL, NULL, NULL);
    VariantClear(&location);
    if (FAILED(result))
    {
        Destroy();
        return false;
    }

    SetTimer(m_parentWindow, kDocumentTimerId, 50, NULL);
    return true;
}

void HtmlView::Destroy()
{
    if (m_parentWindow != NULL)
        KillTimer(m_parentWindow, kDocumentTimerId);

    if (m_inPlaceObject != NULL)
    {
        m_inPlaceObject->InPlaceDeactivate();
        m_inPlaceObject->Release();
        m_inPlaceObject = NULL;
    }
    if (m_oleObject != NULL)
    {
        m_oleObject->Close(OLECLOSE_NOSAVE);
        m_oleObject->SetClientSite(NULL);
        m_oleObject->Release();
        m_oleObject = NULL;
    }
    if (m_browser != NULL)
    {
        m_browser->Release();
        m_browser = NULL;
    }

    m_pendingHtml.clear();
    m_documentWritten = false;
    m_parentWindow = NULL;
}

void HtmlView::Resize()
{
    ApplyObjectRect();
}

void HtmlView::SetHtml(const std::wstring& html)
{
    m_pendingHtml = html;
    m_documentWritten = false;
    if (m_browser != NULL && TryWriteHtml())
    {
        m_documentWritten = true;
        KillTimer(m_parentWindow, kDocumentTimerId);
    }
}

bool HtmlView::OnTimer(UINT_PTR timerId)
{
    if (timerId != kDocumentTimerId)
        return false;

    if (m_browser != NULL && !m_documentWritten && TryWriteHtml())
    {
        m_documentWritten = true;
        KillTimer(m_parentWindow, kDocumentTimerId);
    }
    return true;
}

bool HtmlView::TryWriteHtml()
{
    IDispatch* dispatch = NULL;
    IHTMLDocument2* document = NULL;
    SAFEARRAY* arguments;
    VARIANT* argument = NULL;
    HRESULT result;
    BSTR designMode;

    if (m_browser == NULL)
        return false;

    result = m_browser->get_Document(&dispatch);
    if (FAILED(result) || dispatch == NULL)
        return false;

    result = dispatch->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void**>(&document));
    dispatch->Release();
    if (FAILED(result) || document == NULL)
        return false;

    arguments = SafeArrayCreateVector(VT_VARIANT, 0, 1);
    if (arguments == NULL)
    {
        document->Release();
        return false;
    }

    result = SafeArrayAccessData(arguments, reinterpret_cast<void**>(&argument));
    if (FAILED(result) || argument == NULL)
    {
        SafeArrayDestroy(arguments);
        document->Release();
        return false;
    }

    VariantInit(argument);
    argument->vt = VT_BSTR;
    argument->bstrVal = SysAllocStringLen(m_pendingHtml.c_str(),
                                          static_cast<UINT>(m_pendingHtml.length()));
    SafeArrayUnaccessData(arguments);
    if (argument->bstrVal == NULL)
    {
        SafeArrayDestroy(arguments);
        document->Release();
        return false;
    }

    result = document->write(arguments);
    SafeArrayDestroy(arguments);
    if (SUCCEEDED(result))
    {
        document->close();
        designMode = SysAllocString(L"Off");
        if (designMode != NULL)
        {
            document->put_designMode(designMode);
            SysFreeString(designMode);
        }
    }
    document->Release();
    return SUCCEEDED(result);
}

void HtmlView::ApplyObjectRect()
{
    RECT rect;
    if (m_parentWindow == NULL || m_inPlaceObject == NULL)
        return;
    GetClientRect(m_parentWindow, &rect);
    m_inPlaceObject->SetObjectRects(&rect, &rect);
}

STDMETHODIMP HtmlView::QueryInterface(REFIID interfaceId, void** object)
{
    if (object == NULL)
        return E_POINTER;
    *object = NULL;

    if (IsEqualIID(interfaceId, IID_IUnknown) || IsEqualIID(interfaceId, IID_IOleClientSite))
        *object = static_cast<IOleClientSite*>(this);
    else if (IsEqualIID(interfaceId, IID_IOleWindow) || IsEqualIID(interfaceId, IID_IOleInPlaceSite))
        *object = static_cast<IOleInPlaceSite*>(this);
    else if (IsEqualIID(interfaceId, IID_IDocHostUIHandler))
        *object = static_cast<IDocHostUIHandler*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG) HtmlView::AddRef()
{
    return ++m_referenceCount;
}

STDMETHODIMP_(ULONG) HtmlView::Release()
{
    if (m_referenceCount > 1)
        --m_referenceCount;
    return m_referenceCount;
}

STDMETHODIMP HtmlView::SaveObject() { return E_NOTIMPL; }
STDMETHODIMP HtmlView::GetMoniker(DWORD assign, DWORD whichMoniker, IMoniker** moniker)
{
    UNREFERENCED_PARAMETER(assign);
    UNREFERENCED_PARAMETER(whichMoniker);
    if (moniker != NULL) *moniker = NULL;
    return E_NOTIMPL;
}
STDMETHODIMP HtmlView::GetContainer(IOleContainer** container)
{
    if (container != NULL) *container = NULL;
    return E_NOINTERFACE;
}
STDMETHODIMP HtmlView::ShowObject() { return S_OK; }
STDMETHODIMP HtmlView::OnShowWindow(BOOL show)
{
    UNREFERENCED_PARAMETER(show);
    return S_OK;
}
STDMETHODIMP HtmlView::RequestNewObjectLayout() { return E_NOTIMPL; }

STDMETHODIMP HtmlView::GetWindow(HWND* window)
{
    if (window == NULL) return E_POINTER;
    *window = m_parentWindow;
    return S_OK;
}
STDMETHODIMP HtmlView::ContextSensitiveHelp(BOOL enterMode)
{
    UNREFERENCED_PARAMETER(enterMode);
    return E_NOTIMPL;
}
STDMETHODIMP HtmlView::CanInPlaceActivate() { return S_OK; }
STDMETHODIMP HtmlView::OnInPlaceActivate() { return S_OK; }
STDMETHODIMP HtmlView::OnUIActivate() { return S_OK; }
STDMETHODIMP HtmlView::GetWindowContext(IOleInPlaceFrame** frame, IOleInPlaceUIWindow** document,
                                         LPRECT positionRect, LPRECT clipRect,
                                         LPOLEINPLACEFRAMEINFO frameInfo)
{
    RECT rect;
    if (frame == NULL || document == NULL || positionRect == NULL || clipRect == NULL || frameInfo == NULL)
        return E_POINTER;

    *frame = NULL;
    *document = NULL;
    GetClientRect(m_parentWindow, &rect);
    *positionRect = rect;
    *clipRect = rect;
    ZeroMemory(frameInfo, sizeof(*frameInfo));
    frameInfo->cb = sizeof(*frameInfo);
    frameInfo->hwndFrame = m_parentWindow;
    return S_OK;
}
STDMETHODIMP HtmlView::Scroll(SIZE scrollExtent)
{
    UNREFERENCED_PARAMETER(scrollExtent);
    return E_NOTIMPL;
}
STDMETHODIMP HtmlView::OnUIDeactivate(BOOL undoable)
{
    UNREFERENCED_PARAMETER(undoable);
    return S_OK;
}
STDMETHODIMP HtmlView::OnInPlaceDeactivate() { return S_OK; }
STDMETHODIMP HtmlView::DiscardUndoState() { return S_OK; }
STDMETHODIMP HtmlView::DeactivateAndUndo() { return S_OK; }
STDMETHODIMP HtmlView::OnPosRectChange(LPCRECT positionRect)
{
    if (positionRect == NULL) return E_POINTER;
    return m_inPlaceObject == NULL ? S_OK :
           m_inPlaceObject->SetObjectRects(positionRect, positionRect);
}

STDMETHODIMP HtmlView::ShowContextMenu(DWORD menuId, POINT* point, IUnknown* commandTarget,
                                        IDispatch* dispatchObjectHit)
{
    UNREFERENCED_PARAMETER(menuId);
    UNREFERENCED_PARAMETER(point);
    UNREFERENCED_PARAMETER(commandTarget);
    UNREFERENCED_PARAMETER(dispatchObjectHit);
    return S_OK;
}
STDMETHODIMP HtmlView::GetHostInfo(DOCHOSTUIINFO* hostInfo)
{
    if (hostInfo == NULL) return E_POINTER;
    hostInfo->cbSize = sizeof(*hostInfo);
    hostInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_DISABLE_HELP_MENU;
    hostInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
    hostInfo->pchHostCss = NULL;
    hostInfo->pchHostNS = NULL;
    return S_OK;
}
STDMETHODIMP HtmlView::ShowUI(DWORD dialogType, IOleInPlaceActiveObject* activeObject,
                               IOleCommandTarget* commandTarget, IOleInPlaceFrame* frame,
                               IOleInPlaceUIWindow* document)
{
    UNREFERENCED_PARAMETER(dialogType);
    UNREFERENCED_PARAMETER(activeObject);
    UNREFERENCED_PARAMETER(commandTarget);
    UNREFERENCED_PARAMETER(frame);
    UNREFERENCED_PARAMETER(document);
    return S_FALSE;
}
STDMETHODIMP HtmlView::HideUI() { return S_OK; }
STDMETHODIMP HtmlView::UpdateUI() { return S_OK; }
STDMETHODIMP HtmlView::EnableModeless(BOOL enable)
{
    UNREFERENCED_PARAMETER(enable);
    return S_OK;
}
STDMETHODIMP HtmlView::OnDocWindowActivate(BOOL activate)
{
    UNREFERENCED_PARAMETER(activate);
    return S_OK;
}
STDMETHODIMP HtmlView::OnFrameWindowActivate(BOOL activate)
{
    UNREFERENCED_PARAMETER(activate);
    return S_OK;
}
STDMETHODIMP HtmlView::ResizeBorder(LPCRECT border, IOleInPlaceUIWindow* uiWindow, BOOL frameWindow)
{
    UNREFERENCED_PARAMETER(border);
    UNREFERENCED_PARAMETER(uiWindow);
    UNREFERENCED_PARAMETER(frameWindow);
    return S_OK;
}
STDMETHODIMP HtmlView::TranslateAccelerator(LPMSG message, const GUID* commandGroup, DWORD commandId)
{
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(commandGroup);
    UNREFERENCED_PARAMETER(commandId);
    return S_FALSE;
}
STDMETHODIMP HtmlView::GetOptionKeyPath(LPOLESTR* keyPath, DWORD reserved)
{
    UNREFERENCED_PARAMETER(reserved);
    if (keyPath != NULL) *keyPath = NULL;
    return E_NOTIMPL;
}
STDMETHODIMP HtmlView::GetDropTarget(IDropTarget* dropTarget, IDropTarget** newDropTarget)
{
    UNREFERENCED_PARAMETER(dropTarget);
    if (newDropTarget != NULL) *newDropTarget = NULL;
    return E_NOTIMPL;
}
STDMETHODIMP HtmlView::GetExternal(IDispatch** dispatch)
{
    if (dispatch == NULL) return E_POINTER;
    *dispatch = NULL;
    return S_FALSE;
}
STDMETHODIMP HtmlView::TranslateUrl(DWORD translate, OLECHAR* urlIn, OLECHAR** urlOut)
{
    UNREFERENCED_PARAMETER(translate);
    UNREFERENCED_PARAMETER(urlIn);
    if (urlOut != NULL) *urlOut = NULL;
    return E_NOTIMPL;
}
STDMETHODIMP HtmlView::FilterDataObject(IDataObject* dataObject, IDataObject** filteredDataObject)
{
    UNREFERENCED_PARAMETER(dataObject);
    if (filteredDataObject != NULL) *filteredDataObject = NULL;
    return E_NOTIMPL;
}
