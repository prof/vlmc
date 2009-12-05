/*****************************************************************************
 * EffectEditorContainer.cpp: Effect Box Container Item Class
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Christophe Courtaut <christophe.courtaut@gmail.com>
 * Authors: Clement CHAVANCE <kinder@vlmc.org>
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

#include "EffectEditorContainer.h"
#include <QDebug>


#define EFFECT_CONTAINER_WIDTH 100
#define EFFECT_CONTAINER_HEIGHT 100

EffectEditorContainer::EffectEditorContainer( QString name, int inslots, int outslots,
                                        QWidget* parent ) : m_name( name )
{
    setFlag( QGraphicsItem::ItemIsMovable, true );
    m_margin = EFFECT_CONTAINER_HEIGHT / 20;
    m_inStep = qRound( ( EFFECT_CONTAINER_HEIGHT - 2 * m_margin ) / ( inslots + 1 ) );
    m_outStep = qRound( ( EFFECT_CONTAINER_HEIGHT - 2 * m_margin )  / ( outslots + 1 ) );
    m_inslots = inslots;
    m_outslots = outslots;
}

EffectEditorContainer::~EffectEditorContainer()
{
    QGraphicsLineItem* line;
    foreach( line, m_inSlotLines.values() )
        delete line;
    foreach( line, m_outSlotLines.values() )
        delete line;
}

void    EffectEditorContainer::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* parent )
{
    painter->setRenderHint( QPainter::Antialiasing );
    painter->drawRect( m_margin, m_margin, EFFECT_CONTAINER_HEIGHT - 2 * m_margin, EFFECT_CONTAINER_WIDTH - 2 * m_margin );
    painter->setPen( QPen( Qt::red ) );
    painter->drawText( boundingRect(), Qt::AlignCenter, m_name );
    drawSlots( painter, 0, m_inslots, m_inStep );
    drawSlots( painter, EFFECT_CONTAINER_WIDTH - 2 * m_margin, m_outslots, m_outStep );
}

void    EffectEditorContainer::drawSlots( QPainter* painter, int lineHPos, int slotNumber, int step )
{
    for ( int i = 1; i <= slotNumber; ++i )
        painter->drawLine( lineHPos, m_margin + i * step, lineHPos + m_margin * 2 , m_margin + i * step );
}

QRectF  EffectEditorContainer::boundingRect() const
{
    return QRectF(0, 0, EFFECT_CONTAINER_WIDTH + m_margin * 2, EFFECT_CONTAINER_HEIGHT + m_margin * 2 );
}

QMap<int, QGraphicsLineItem*>&  EffectEditorContainer::getLines( SlotType type )
{
    if ( type == InSlot )
        return m_inSlotLines;
    else
        return m_outSlotLines;
}

void    EffectEditorContainer::addSlotLine( int slot, QGraphicsLineItem* line, SlotType type )
{
    if ( ( type == InSlot && slot < 0 && slot >= m_inslots ) ||
         ( type == OutSlot && slot < 0 && slot >= m_outslots ))
        return;
    getLines( type )[slot] = line;
}

QPointF EffectEditorContainer::getSlotPos( int slot, SlotType type )
{
    if ( type == InSlot )
        return getInSlotPos( slot );
    return getOutSlotPos( slot );
}

QPointF EffectEditorContainer::getInSlotPos( int slot )
{
    if ( slot < 0 && slot >= m_inslots )
        return QPointF();
    return QPointF( m_margin, m_margin + ( slot + 1 ) * m_inStep );
}

QPointF EffectEditorContainer::getOutSlotPos( int slot )
{
    if ( slot < 0 && slot >= m_outslots )
        return QPointF();
    return QPointF( EFFECT_CONTAINER_WIDTH - m_margin, m_margin + ( slot + 1 ) * m_outStep );
}

int EffectEditorContainer::getInSlotNumber( QPointF point )
{
    for ( int i = 1; i <= m_inslots; ++i )
        if ( point == QPointF( m_margin, m_margin + i * m_inStep ) )
            return i - 1;
    return -1;
}

int EffectEditorContainer::getOutSlotNumber( QPointF point )
{
    for ( int i = 1; i <= m_outslots; ++i )
        if ( point == QPointF( EFFECT_CONTAINER_WIDTH - m_margin, m_margin + i * m_outStep ) )
            return i - 1;
    return -1;
}

QPointF  EffectEditorContainer::isOnInSlot( QPointF pos )
{
    for ( int i = 1; i <= m_inslots; ++i )
    {
        if ( ( pos.x() >= 0 && pos.x() <= m_margin * 2 ) &&
             ( pos.y() >= m_margin + i * m_inStep - m_inStep / 2 &&
               pos.y() <= m_margin + i * m_inStep + m_inStep / 2 ) )
        {
            qDebug() << mapToScene( QPointF( m_margin, m_margin + i * m_inStep ) );
            return mapToScene( QPointF( m_margin, m_margin + i * m_inStep ) );
        }
    }
    return QPointF();
}

QPointF  EffectEditorContainer::isOnOutSlot( QPointF pos )
{
    for ( int i = 1; i <= m_outslots; ++i )
    {
        if ( ( pos.x() >= EFFECT_CONTAINER_WIDTH - m_margin * 2 && pos.x() <= EFFECT_CONTAINER_WIDTH ) &&
             ( pos.y() >= m_margin + i * m_outStep - m_outStep / 2 &&
               pos.y() <= m_margin + i * m_outStep + m_outStep / 2 ) )
        return mapToScene( QPointF( EFFECT_CONTAINER_WIDTH - m_margin, m_margin + i * m_outStep ) );
    }
    return QPointF();
}

QVariant EffectEditorContainer::itemChange( GraphicsItemChange change, const QVariant &value )
{
    if ( change == ItemPositionChange )
    {

        int i = 0;
        QPointF diffPos = value.toPointF() - pos();
        moveSlotsLines( m_inSlotLines, m_inslots, diffPos, InSlot );
        moveSlotsLines( m_outSlotLines, m_outslots, diffPos, OutSlot );
    }
    return QGraphicsItem::itemChange(change, value);
}

void    EffectEditorContainer::moveSlotsLines( QMap<int, QGraphicsLineItem*>& slotsMap, int slotCount, QPointF movement, SlotType type )
{
    for ( int i = 0; i < slotCount; ++i )
    {
        if ( !slotsMap.contains(i) )
            continue;
        if ( mapFromScene( slotsMap[i]->line().p1() ) == getSlotPos( i, type ) )
        {
            slotsMap[i]->setLine( slotsMap[i]->line().p2().x(),
                                       slotsMap[i]->line().p2().y(),
                                       ( slotsMap[i]->line().p1() + movement ).x(),
                                       ( slotsMap[i]->line().p1() + movement ).y());
        }
        if ( mapFromScene( slotsMap[i]->line().p2() ) == getSlotPos( i, type ) )
        {
            slotsMap[i]->setLine( slotsMap[i]->line().p1().x(),
                                  slotsMap[i]->line().p1().y(),
                                  ( slotsMap[i]->line().p2() + movement ).x(),
                                  ( slotsMap[i]->line().p2() + movement ).y() );
        }
    }
}

QGraphicsLineItem*  EffectEditorContainer::getLine( int slot, SlotType type )
{
    qDebug() << slot << type;
    if ( type == InSlot && m_inSlotLines.contains( slot ) )
        return m_inSlotLines[slot];
    else if ( type == OutSlot && m_outSlotLines.contains( slot ) )
        return m_outSlotLines[slot];
    qDebug() << "You got no line on this slot";
    return NULL;
}

void    EffectEditorContainer::removeLine( QGraphicsLineItem* line, SlotType type )
{
    if ( type == InSlot && m_inSlotLines.values().contains( line ) )
        m_inSlotLines.remove( m_inSlotLines.key( line ) );
    if ( type == OutSlot && m_outSlotLines.values().contains( line ) )
        m_outSlotLines.remove( m_inSlotLines.key( line ) );
}
