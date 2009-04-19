/*****************************************************************************
 * LibraryItemModel.h: Data Model for Library
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

#ifndef LIBRARYITEMMODEL_H
#define LIBRARYITEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QAbstractTableModel>

class LibraryItemModel : public QAbstractItemModel
{
    Q_OBJECT
    public:
        LibraryItemModel();
        virtual ~LibraryItemModel();

        QModelIndex     index( int row, int column, const QModelIndex& parent = QModelIndex() );
        QModelIndex     parent( const QModelIndex& child);
        int             rowCount( const QModelIndex& parent = QModelIndex() );
        int             columnCount( const QModelIndex& parent = QModelIndex() );
        QVariant        data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
        Qt::ItemFlags   flags( const QModelIndex& index ) const;
        QVariant        headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
        bool            insertRows(int row, int count, const QModelIndex &parent = QModelIndex() );
        bool            removeRows(int row, int count, const QModelIndex& parent = QModelIndex() );
        bool            insertColumns(int column, int count, const QModelIndex &parent = QModelIndex() );
        bool            removeColumns(int column, int count, const QModelIndex& parent = QModelIndex() );
};

#endif /* !LIBRARYITEMMODEL_H */
