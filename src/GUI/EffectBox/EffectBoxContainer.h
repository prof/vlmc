/*****************************************************************************
 * EffectBoxContainer.h: Effect Box Container Item Class
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Christophe Courtaut <christophe.courtaut@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef EFFECTBOXCONTAINER_H
#define EFFECTBOXCONTAINER_H

#include <QGraphicsItemGroup>
#include <QPainter>
#include <QMap>

class EffectBoxContainer : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };
    enum SlotType { InSlot = 0, OutSlot };
    EffectBoxContainer( QString name, int inslots, int outslots, QWidget* parent = NULL );
    virtual             ~EffectBoxContainer();
    QPointF             isOnInSlot( QPointF pos );
    QPointF             isOnOutSlot( QPointF pos );
    void                addSlotLine( int slot, QGraphicsLineItem* line, SlotType type );
    QPointF             getInSlotPos( int slot );
    QPointF             getOutSlotPos( int slot );
    QGraphicsLineItem*  getLine( int slot, SlotType type );
    int                 getInSlotNumber( QPointF point );
    int                 getOutSlotNumber( QPointF point );
    int                 type() const { return Type; }
protected:
    void                paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = NULL );
    QVariant            itemChange( GraphicsItemChange change, const QVariant &value );
    QRectF              boundingRect() const;
private:
    void                moveSlotsLines( QMap<int, QGraphicsLineItem*>& slotsMap, int slotCount, QPointF movement, SlotType );
    void                drawSlots( QPainter* painter, int lineHPos, int slotNumber, int step );
    QPointF             getSlotPos( int slot, SlotType type );
    QMap<int, QGraphicsLineItem*>&  getLines( SlotType type );
private:
    QString             m_name;
    QMap<int, QGraphicsLineItem*>   m_inSlotLines;
    QMap<int, QGraphicsLineItem*>   m_outSlotLines;
    int         m_inslots;
    int         m_outslots;
    qreal       m_margin;
    qreal       m_inStep;
    qreal       m_outStep;
};

#endif // EFFECTBOXCONTAINER_H
