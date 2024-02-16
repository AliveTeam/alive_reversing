#pragma once

#include "Model.hpp"
#include <QGraphicsItem>

class IReflector;

class IGraphicsItem
{
public:
    virtual ~IGraphicsItem() { }
    virtual void SyncInternalObject() = 0;
    virtual void Visit(IReflector& f) = 0;

    static void SetTransparency(QGraphicsItem* pItem, int transparency)
    {
        qreal v = static_cast<qreal>(transparency) / 100.0; // 10-100 -> 0.1 -> 1.0
        pItem->setOpacity(v);
    }
};
