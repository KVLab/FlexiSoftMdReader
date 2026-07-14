#ifndef FLEXISOFT_MD_READER_HTML_VIEW_H
#define FLEXISOFT_MD_READER_HTML_VIEW_H

#include <windows.h>
#include <oleidl.h>
#include <exdisp.h>
#include <mshtmhst.h>
#include <string>

class HtmlView : public IOleClientSite,
                 public IOleInPlaceSite,
                 public IDocHostUIHandler
{
public:
    HtmlView();
    ~HtmlView();

    bool Create(HWND parentWindow);
    void Destroy();
    void Resize();
    void SetHtml(const std::wstring& html);
    bool OnTimer(UINT_PTR timerId);

    STDMETHODIMP QueryInterface(REFIID interfaceId, void** object);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    STDMETHODIMP SaveObject();
    STDMETHODIMP GetMoniker(DWORD assign, DWORD whichMoniker, IMoniker** moniker);
    STDMETHODIMP GetContainer(IOleContainer** container);
    STDMETHODIMP ShowObject();
    STDMETHODIMP OnShowWindow(BOOL show);
    STDMETHODIMP RequestNewObjectLayout();

    STDMETHODIMP GetWindow(HWND* window);
    STDMETHODIMP ContextSensitiveHelp(BOOL enterMode);
    STDMETHODIMP CanInPlaceActivate();
    STDMETHODIMP OnInPlaceActivate();
    STDMETHODIMP OnUIActivate();
    STDMETHODIMP GetWindowContext(IOleInPlaceFrame** frame, IOleInPlaceUIWindow** document,
                                  LPRECT positionRect, LPRECT clipRect,
                                  LPOLEINPLACEFRAMEINFO frameInfo);
    STDMETHODIMP Scroll(SIZE scrollExtent);
    STDMETHODIMP OnUIDeactivate(BOOL undoable);
    STDMETHODIMP OnInPlaceDeactivate();
    STDMETHODIMP DiscardUndoState();
    STDMETHODIMP DeactivateAndUndo();
    STDMETHODIMP OnPosRectChange(LPCRECT positionRect);

    STDMETHODIMP ShowContextMenu(DWORD menuId, POINT* point, IUnknown* commandTarget, IDispatch* dispatchObjectHit);
    STDMETHODIMP GetHostInfo(DOCHOSTUIINFO* hostInfo);
    STDMETHODIMP ShowUI(DWORD dialogType, IOleInPlaceActiveObject* activeObject,
                        IOleCommandTarget* commandTarget, IOleInPlaceFrame* frame,
                        IOleInPlaceUIWindow* document);
    STDMETHODIMP HideUI();
    STDMETHODIMP UpdateUI();
    STDMETHODIMP EnableModeless(BOOL enable);
    STDMETHODIMP OnDocWindowActivate(BOOL activate);
    STDMETHODIMP OnFrameWindowActivate(BOOL activate);
    STDMETHODIMP ResizeBorder(LPCRECT border, IOleInPlaceUIWindow* uiWindow, BOOL frameWindow);
    STDMETHODIMP TranslateAccelerator(LPMSG message, const GUID* commandGroup, DWORD commandId);
    STDMETHODIMP GetOptionKeyPath(LPOLESTR* keyPath, DWORD reserved);
    STDMETHODIMP GetDropTarget(IDropTarget* dropTarget, IDropTarget** newDropTarget);
    STDMETHODIMP GetExternal(IDispatch** dispatch);
    STDMETHODIMP TranslateUrl(DWORD translate, OLECHAR* urlIn, OLECHAR** urlOut);
    STDMETHODIMP FilterDataObject(IDataObject* dataObject, IDataObject** filteredDataObject);

private:
    HtmlView(const HtmlView&);
    HtmlView& operator=(const HtmlView&);

    bool TryWriteHtml();
    void ApplyObjectRect();

    HWND m_parentWindow;
    IWebBrowser2* m_browser;
    IOleObject* m_oleObject;
    IOleInPlaceObject* m_inPlaceObject;
    std::wstring m_pendingHtml;
    ULONG m_referenceCount;
    bool m_documentWritten;
};

#endif
