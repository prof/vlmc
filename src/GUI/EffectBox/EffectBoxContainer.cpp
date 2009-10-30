/*****************************************************************************
 * EffectBoxContainer.cpp: Effect Box Container Item Class
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

#include "EffectBoxContainer.h"
#include <QDebug>


#define EFFECT_CONTAINER_WIDTH 100
#define EFFECT_CONTAINER_HEIGHT 100

EffectBoxContainer::EffectBoxContainer( QString name, int inslots, int outslots,
                                        QWidget* parent ) : m_name( name )
{
    setFlag( QGraphicsItem::ItemIsMovable, true );
    m_margin = EFFECT_CONTAINER_HEIGHT / 20;
    m_inStep = ( EFFECT_CONTAINER_HEIGHT - 2 * m_margin ) / ( inslots + 1 );
    m_outStep = ( EFFECT_CONTAINER_HEIGHT - 2 * m_margin )  / ( outslots + 1 );
    m_inslots = inslots;
    m_outslots = outslots;
}

void    EffectBoxContainer::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* parent )
{
    painter->setRenderHint( QPainter::Antialiasing );
    painter->drawRect( m_margin, m_margin, EFFECT_CONTAINER_HEIGHT - 2 * m_margin, EFFECT_CONTAINER_WIDTH - 2 * m_margin );
    painter->setPen( QPen( Qt::red ) );
    painter->drawText( boundingRect(), Qt::AlignCenter, m_name );
    drawSlots( painter, 0, m_inslots, m_inStep );
    drawSlots( painter, EFFECT_CONTAINER_WIDTH - 2 * m_margin, m_outslots, m_outStep );
}

void    EffectBoxContainer::drawSlots( QPainter* painter, int lineHPos, int slotNumber, int step )
{
    for ( int i = 1; i <= slotNumber; ++i )
        painter->drawLine( lineHPos, m_margin + i * step, lineHPos + m_margin * 2 , m_margin + i * step );
}

QRectF  EffectBoxContainer::boundingRect() const
{
    return QRectF(0, 0, EFFECT_CONTAINER_WIDTH + m_margin * 2, EFFECT_CONTAINER_HEIGHT + m_margin * 2 );
}

QPointF  EffectBoxContainer::isOnInSlot( QPointF pos )
{
    for ( int i = 1; i <= m_inslots; ++i )
    {
        if ( ( pos.x() >= 0 && pos.x() <= m_margin * 2 ) &&
             ( pos.y() >= m_margin + i * m_inStep - m_inStep / 2 &&
               pos.y() <= m_margin + i * m_inStep + m_inStep / 2 ) )
        return mapToScene( QPointF( m_margin, m_margin + i * m_inStep ) );
    }
    return QPointF();
}

QPointF  EffectBoxContainer::isOnOutSlot( QPointF pos )
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
