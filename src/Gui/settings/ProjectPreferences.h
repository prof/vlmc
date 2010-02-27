/*****************************************************************************
 * ProjectPreferences.h: VLMC project preferences class
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
 *
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

#ifndef PROJECTPREFERENCES_H
#define PROJECTPREFERENCES_H

#include "PreferenceWidget.h"
#include "SettingsManager.h"
#include "ui_ProjectPreferences.h"

class QWidget;

class   ProjectPreferences : public PreferenceWidget
{
    //Q_OBJECT
    public:
        ProjectPreferences( QWidget* parent = 0 );
        virtual ~ProjectPreferences();
        void    load();
        void    save();
    protected:
        virtual void changeEvent( QEvent *e );
    private:
        Ui::ProjectPreferences      m_ui;
        const SettingsManager::Type m_type;
};

#endif