/*****************************************************************************
 * MainWindow.h: VLMC MainWindow
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Ludovic Fauvet <etix@l0cal.com>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QSlider>
#include <QToolButton>
#include <QButtonGroup>

#include "vlmc.h"
#include "ui_MainWindow.h"

#include "DockWidgetManager.h"
#include "LanguagePreferences.h"
#include "Timeline.h"
#include "WorkflowRenderer.h"
#include "WorkflowFileRenderer.h"
#include "PreviewWidget.h"
#include "Settings.h"
#include "EffectsEngine.h"
#include "MainWorkflow.h"
#include "ProjectWizard.h"
#include "EffectEditorView.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY( MainWindow )

public:

    explicit MainWindow( QWidget *parent = 0 );
    void registerWidgetInWindowMenu( QDockWidget* widget );
    ~MainWindow();

public slots:
    void        zoomIn();
    void        zoomOut();

protected:
    virtual void changeEvent( QEvent *e );

private:
    void        initializeDockWidgets( void );
    void        setupLibrary();
    void        setupMainWidget( WorkflowRenderer* wRenderer );
    void        createStatusBar();
    void        createGlobalPreferences();
    void        createProjectPreferences();

    Ui::MainWindow          m_ui;
    QSlider*                m_zoomSlider;
    Timeline*               m_timeline;
    EffectEditorView*       m_effectEditor;
    QStackedWidget*         m_mainWidget;
    PreviewWidget*          m_clipPreview;
    PreviewWidget*          m_projectPreview;
    WorkflowFileRenderer*   m_renderer;
    Settings*               m_globalPreferences;
    Settings*               m_DefaultProjectPreferences;
    Settings*               m_projectPreferences;
    ProjectWizard*          m_pWizard;

private slots:
    void on_actionBypass_effects_engine_toggled(bool );
    void on_actionFullscreen_triggered( bool checked );
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionPreferences_triggered();
    void on_actionTranscode_triggered();
    void on_actionRender_triggered();
    void on_actionNew_Project_triggered();
    void on_actionLoad_Project_triggered();
    void on_actionSave_triggered();
    void on_actionImport_triggered();
    void on_actionHelp_triggered();
    void on_actionProject_Preferences_triggered();
    void on_actionProject_Wizard_triggered();
    void toolButtonClicked( int id );
    void switchMainWidget();

signals:
    void translateDockWidgetTitle();
    void toolChanged( ToolButtons );

};

#endif // MAINWINDOW_H
