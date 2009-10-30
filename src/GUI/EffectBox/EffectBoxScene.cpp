/*****************************************************************************
 * EffectBoxScene.cpp: Effect Box Scene
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

#include "EffectBoxScene.h"
#include "EffectBoxContainer.h"

#include <QPointF>
#include <QDebug>

EffectBoxScene::EffectBoxScene( QWidget* parent ) : m_currentLine( NULL ), m_box( NULL )
{

}

void    EffectBoxScene::addEffect( QString title )
{
    EffectBoxContainer* container = new EffectBoxContainer( title, 4, 2 );
    addItem( container );
    container = new EffectBoxContainer( "Poulpe", 6, 8 );
    addItem( container );
}

void    EffectBoxScene::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
    EffectBoxContainer* box = NULL;
    QGraphicsItem* item = itemAt( event->scenePos() );
    if ( !item )
        goto NoEffectBoxContainer;
    box = qgraphicsitem_cast<EffectBoxContainer*>( item );
    if ( !box )
        goto NoEffectBoxContainer;
    qDebug() << event->pos() << event->scenePos();
    m_box = box;
    m_currentInSlot = box->isOnInSlot( box->mapFromScene( event->scenePos() ) );
    m_currentOutSlot = box->isOnOutSlot( box->mapFromScene( event->scenePos() ) );
    if ( m_currentInSlot.isNull() && m_currentOutSlot.isNull() )
        goto NoEffectBoxContainer;
    return;
    NoEffectBoxContainer:
    QGraphicsScene::mousePressEvent(event);
    return;
}

void    EffectBoxScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
    QPointF origin;
    qDebug() << "test";
    if ( m_currentInSlot.isNull() && m_currentOutSlot.isNull() )
        goto NoEffect;
    origin = m_currentInSlot.isNull() ? m_currentOutSlot : m_currentInSlot;
    if ( !m_currentLine )
    {
        m_currentLine = new QGraphicsLineItem();
        addItem( m_currentLine );
    }
    m_currentLine->setLine( origin.x(), origin.y(), event->scenePos().x(), event->scenePos().y() );
    NoEffect:
    QGraphicsScene::mouseMoveEvent( event );
    return;
}

void    EffectBoxScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
    EffectBoxContainer* box = NULL;
    QGraphicsItem* item = itemAt( event->scenePos() );
    if ( !item )
        goto NoEffectBoxContainer;
    box = qgraphicsitem_cast<EffectBoxContainer*>( item );
    if ( !box )
        goto NoEffectBoxContainer;
    if ( box == m_box )
    {
        m_box = NULL;
        goto NoEffectBoxContainer;
    }
    if ( m_currentInSlot.isNull() )
        m_currentInSlot = box->isOnInSlot( box->mapFromScene( event->scenePos() ) );
    if ( m_currentOutSlot.isNull() )
        m_currentOutSlot = box->isOnOutSlot( box->mapFromScene( event->scenePos() ) );
    if ( m_currentInSlot.isNull() || m_currentOutSlot.isNull() )
        goto NoEffectBoxContainer;
    addItem( m_currentLine );
    NoEffectBoxContainer:
    if ( m_currentLine && ( m_currentInSlot.isNull() || m_currentOutSlot.isNull() ) )
    {
        removeItem( m_currentLine );
        delete m_currentLine;
    }
    m_currentInSlot = QPointF();
    m_currentOutSlot = QPointF();
    m_currentLine = NULL;
    QGraphicsScene::mouseReleaseEvent( event );
    return;
}
