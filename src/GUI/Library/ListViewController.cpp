/*****************************************************************************
 * ListViewController.h:
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Thomas Boquet <thomas.boquet@gmail.com>
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

#include <QPushButton>

#include "ListViewController.h"
#include "MediaCellView.h"
#include "Library.h"

ListViewController::ListViewController( StackViewController* nav ) : m_nav( nav ),
    m_currentUuid( 0 )
{
    m_title      = new QString( "Media List" );
    m_scrollArea = new QScrollArea();
    m_layout     = new QVBoxLayout();
    m_container  = new QWidget();

    m_layout->setAlignment( Qt::AlignTop );
    m_layout->setSpacing( 0 );
    m_layout->setContentsMargins( 0, 0, 0, 0 );
    m_container->setLayout( m_layout );
    m_scrollArea->setWidget( m_container );
    m_scrollArea->setWidgetResizable( true );
    m_scrollArea->setAcceptDrops( true );

}

ListViewController::~ListViewController()
{
    delete m_title;
    delete m_scrollArea;
    delete m_layout;
    delete m_container;
}

// ViewController method implementation

const QString&      ListViewController::title() const
{
    return *m_title;
}

QWidget*            ListViewController::view() const
{
    return m_scrollArea;
}

void                ListViewController::addCell( QWidget* cell )
{
    m_layout->addWidget( cell );
}

void            ListViewController::removeCell( QWidget* cell )
{
    m_layout->removeWidget( cell );
    delete cell;
}

void                ListViewController::cellSelected( const QUuid& uuid )
{
    if ( m_currentUuid == 0 || *m_currentUuid != uuid )
    {
        if ( m_currentUuid != 0 )
            delete m_currentUuid;
        m_currentUuid = new QUuid( uuid );
        emit selectedClipChanged( Library::getInstance()->getClip( uuid ) );
    }
}