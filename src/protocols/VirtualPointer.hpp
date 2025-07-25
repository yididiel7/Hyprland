#pragma once

#include <vector>
#include <cstdint>
#include <array>
#include "WaylandProtocol.hpp"
#include "wlr-virtual-pointer-unstable-v1.hpp"
#include "../helpers/signal/Signal.hpp"
#include "../devices/IPointer.hpp"
#include "../helpers/Monitor.hpp"

class CVirtualPointerV1Resource {
  public:
    CVirtualPointerV1Resource(SP<CZwlrVirtualPointerV1> resource_, PHLMONITORREF boundOutput_);
    ~CVirtualPointerV1Resource();

    struct {
        CSignalT<>                               destroy;
        CSignalT<IPointer::SMotionEvent>         move;
        CSignalT<IPointer::SMotionAbsoluteEvent> warp;
        CSignalT<IPointer::SButtonEvent>         button;
        CSignalT<IPointer::SAxisEvent>           axis;
        CSignalT<>                               frame;

        CSignalT<IPointer::SSwipeBeginEvent>     swipeBegin;
        CSignalT<IPointer::SSwipeUpdateEvent>    swipeUpdate;
        CSignalT<IPointer::SSwipeEndEvent>       swipeEnd;

        CSignalT<IPointer::SPinchBeginEvent>     pinchBegin;
        CSignalT<IPointer::SPinchUpdateEvent>    pinchUpdate;
        CSignalT<IPointer::SPinchEndEvent>       pinchEnd;

        CSignalT<IPointer::SHoldBeginEvent>      holdBegin;
        CSignalT<IPointer::SHoldEndEvent>        holdEnd;
    } m_events;

    bool          good();
    wl_client*    client();

    std::string   m_name;

    PHLMONITORREF m_boundOutput;

  private:
    SP<CZwlrVirtualPointerV1>           m_resource;

    uint32_t                            m_axis = 0;

    std::array<IPointer::SAxisEvent, 2> m_axisEvents;
};

class CVirtualPointerProtocol : public IWaylandProtocol {
  public:
    CVirtualPointerProtocol(const wl_interface* iface, const int& ver, const std::string& name);

    virtual void bindManager(wl_client* client, void* data, uint32_t ver, uint32_t id);

    struct {
        CSignalT<SP<CVirtualPointerV1Resource>> newPointer;
    } m_events;

  private:
    void onManagerResourceDestroy(wl_resource* res);
    void destroyResource(CVirtualPointerV1Resource* pointer);
    void onCreatePointer(CZwlrVirtualPointerManagerV1* pMgr, wl_resource* seat, uint32_t id, PHLMONITORREF output);

    //
    std::vector<UP<CZwlrVirtualPointerManagerV1>> m_managers;
    std::vector<SP<CVirtualPointerV1Resource>>    m_pointers;

    friend class CVirtualPointerV1Resource;
};

namespace PROTO {
    inline UP<CVirtualPointerProtocol> virtualPointer;
};
