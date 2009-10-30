/*****************************************************************************
 * EffectBoxScene.h: Effect Box Scene
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

#ifndef EFFECTBOXSCENE_H
#define EFFECTBOXSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "EffectBoxContainer.h"

class EffectBoxScene : public QGraphicsScene
{
public:
    EffectBoxScene( QWidget* parent = NULL );

    void    addEffect(QString title);
    void    mousePressEvent( QGraphicsSceneMouseEvent* event );
    void    mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
    void    mouseMoveEvent( QGraphicsSceneMouseEvent* event );
private:
    QPointF m_currentInSlot;
    QPointF m_currentOutSlot;
    QGraphicsLineItem* m_currentLine;
    EffectBoxContainer* m_box;
};

#endif // EFFECTBOXSCENE_H
