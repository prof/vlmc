/*****************************************************************************
 * EffectEditorScene.cpp: Effect Box Scene
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

#include "EffectEditorScene.h"
#include "EffectEditorContainer.h"

#include <QPointF>
#include <QtDebug>

EffectEditorScene::EffectEditorScene( QWidget* parent ) : m_currentLine( NULL ), m_box( NULL )
{
}

void    EffectEditorScene::addEffect( QString title )
{
    EffectEditorContainer* container = new EffectEditorContainer( title, 4, 2 );
    addItem( container );
    connect( container, SIGNAL( magnify( const QString& ) ),
             SIGNAL( magnifyContainer( const QString& ) ) );
    //container = new EffectEditorContainer( "Poulpe", 6, 8 );
    //addItem( container );
}

void    EffectEditorScene::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
    EffectEditorContainer* box = NULL;
    int inSlotNumber = -1;
    int outSlotNumber = -1;
    QGraphicsItem* item = itemAt( event->scenePos() );
    if ( !item )
        goto NoEffectBoxContainer;
    box = qgraphicsitem_cast<EffectEditorContainer*>( item );
    if ( !box )
        goto NoEffectBoxContainer;
    // We got a valid EffectEditorContainer
    m_box = box;
    // Get position of clicked slot
    m_currentInSlot = box->isOnInSlot( box->mapFromScene( event->scenePos() ) );
    m_currentOutSlot = box->isOnOutSlot( box->mapFromScene( event->scenePos() ) );
    // Has a slot been clicked?
    if ( m_currentInSlot.isNull() && m_currentOutSlot.isNull() )
        goto NoEffectBoxContainer;
    // Does the slots already have a connection?
    if ( !m_currentInSlot.isNull() )
        m_currentLine = box->getLine( box->getInSlotNumber( box->mapFromScene( m_currentInSlot ) ), EffectEditorContainer::InSlot );
    else if ( !m_currentOutSlot.isNull() )
        m_currentLine = box->getLine( box->getOutSlotNumber( box->mapFromScene( m_currentOutSlot ) ), EffectEditorContainer::OutSlot );
    // Doesn't he?
    if ( !m_currentLine )
    {
        m_currentLine = new QGraphicsLineItem();
        m_currentLine->setFlags( QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
        //m_currentLine->setFlag( QGraphicsItem::ItemIsSelectable ,true);
        // Add the connection
        if ( m_currentInSlot.isNull() )
            m_box->addSlotLine( m_box->getOutSlotNumber(
                    m_box->mapFromScene( m_currentOutSlot ) ), m_currentLine, EffectEditorContainer::OutSlot );
        else
            m_box->addSlotLine( m_box->getInSlotNumber(
                    m_box->mapFromScene( m_currentInSlot ) ), m_currentLine, EffectEditorContainer::InSlot );
        addItem( m_currentLine );
    }
    return;
NoEffectBoxContainer:
    QGraphicsScene::mousePressEvent(event);
    return;
}

void    EffectEditorScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
    QPointF origin;
    if ( m_currentInSlot.isNull() && m_currentOutSlot.isNull() )
        goto NoEffect;
    origin = m_currentInSlot.isNull() ? m_currentOutSlot : m_currentInSlot;
    m_currentLine->setLine( origin.x(), origin.y(), event->scenePos().x(), event->scenePos().y() );
NoEffect:
    QGraphicsScene::mouseMoveEvent( event );
    return;
}

void    EffectEditorScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
    EffectEditorContainer* box = NULL;
    QPointF target;
    QGraphicsItem* item = itemAt( event->scenePos() );
    if ( !item )
        goto NoEffectBoxContainer;
    box = qgraphicsitem_cast<EffectEditorContainer*>( item );
    if ( !box )
        goto NoEffectBoxContainer;
    // We got a EffectEditorContainer
    // Get Target Slot Position
    if ( m_currentInSlot.isNull() )
        m_currentInSlot = target = box->isOnInSlot( box->mapFromScene( event->scenePos() ) );
    if ( m_currentOutSlot.isNull() )
        m_currentOutSlot = target = box->isOnOutSlot( box->mapFromScene( event->scenePos() ) );
    // Have we got a inSlot and an outSlot
    if ( m_currentInSlot.isNull() || m_currentOutSlot.isNull() )
        goto NoEffectBoxContainer;
    // Add the connection in the EffectEditorContainer
    if ( target == m_currentInSlot )
        box->addSlotLine( box->getInSlotNumber( box->mapFromScene( target ) ), m_currentLine, EffectEditorContainer::InSlot );
    else
        box->addSlotLine( box->getOutSlotNumber( box->mapFromScene( target ) ), m_currentLine, EffectEditorContainer::OutSlot );
    m_currentLine->setLine( m_currentOutSlot.x(), m_currentOutSlot.y(), m_currentInSlot.x(), m_currentInSlot.y() );
NoEffectBoxContainer:
    if ( m_currentLine && ( m_currentInSlot.isNull() || m_currentOutSlot.isNull() ) )
    {
        if ( m_box->getInSlotNumber( m_box->mapFromScene( m_currentInSlot ) ) != -1 )
            m_box->removeLine( m_currentLine, EffectEditorContainer::InSlot );
        if ( m_box->getOutSlotNumber( m_box->mapFromScene( m_currentOutSlot ) ) != -1 )
            m_box->removeLine( m_currentLine, EffectEditorContainer::OutSlot );
        if ( items().contains( m_currentLine ))
            removeItem( m_currentLine );
        delete m_currentLine;
    }
    m_currentInSlot = QPointF();
    m_currentOutSlot = QPointF();
    m_currentLine = NULL;
    m_box = NULL;
    QGraphicsScene::mouseReleaseEvent( event );
    return;
}
