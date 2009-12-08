/*****************************************************************************
 * KeyboardShortcut.cpp: Keyboard shortcut preferences
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Hugo Beauzee-Luyssen <hugo@vlmc.org>
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

#include <QtDebug>

#include "KeyboardShortcut.h"
#include "KeyboardShortcutInput.h"
#include "SettingsManager.h"

KeyboardShortcut::KeyboardShortcut( QWidget* parent ) :
        PreferenceWidget( parent )
{
    m_layout = new QFormLayout( this );
    const SettingsPart*   parts = SettingsManager::getInstance()->getConfigPart( "keyboard_shortcut" );
    Q_ASSERT( parts != NULL );

    SettingsPart::ConfigPair::const_iterator    it = parts->m_data.begin();
    SettingsPart::ConfigPair::const_iterator    ite = parts->m_data.end();
    while ( it != ite )
    {
        m_keySeq[it.key()] = new QKeySequence( it.value()->get().toString() );
        m_layout->addRow( it.key(), new KeyboardShortcutInput( m_keySeq[it.key()]->toString(), this ) );
        ++it;
    }
    setLayout( m_layout );
}

KeyboardShortcut::~KeyboardShortcut()
{
}

void        KeyboardShortcut::load()
{
}

void        KeyboardShortcut::save()
{
}