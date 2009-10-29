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

class EffectBoxContainer : public QGraphicsItem
{
public:
    EffectBoxContainer( QString name, int inslots, int outslots, QWidget* parent = NULL );
protected:
    void    paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = NULL );
    QRectF  boundingRect() const;
private:
    void    drawSlots( QPainter* painter, int lineHPos, int slotNumber, int step );
private:
    QString     m_name;
    int         m_inslots;
    int         m_outslots;
    qreal       m_margin;
    qreal       m_inStep;
    qreal       m_outStep;
};

#endif // EFFECTBOXCONTAINER_H
