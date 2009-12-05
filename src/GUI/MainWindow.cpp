/*****************************************************************************
 * MainWindow.cpp: VLMC MainWindow
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

#include <QLabel>
#include <QSizePolicy>
#include <QPalette>
#include <QDockWidget>
#include <QFileDialog>
#include <QSlider>
#include <QUndoView>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <QStringList>

#include "MainWindow.h"
#include "Library.h"
#include "Timeline.h"
#include "About.h"
#include "WorkflowRenderer.h"
#include "ClipRenderer.h"
#include "UndoStack.h"
#include "ClipProperty.h"
#include "PreviewWidget.h"
#include "PreferenceWidget.h"
#include "ProjectPreferences.h"
#include "ProjectManager.h"
#include "AudioProjectPreferences.h"
#include "VideoProjectPreferences.h"
#include "VLMCSettingsDefault.h"
#include "ProjectSettingsDefault.h"
#include "VLMCPreferences.h"
#include "Import.h"
#include "MediaLibraryWidget.h"
#include "LanguagePreferences.h"
#include "EffectBoxView.h"
#include "EffectBoxScene.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ), m_renderer( NULL )
{
    m_ui.setupUi( this );
    qRegisterMetaType<MainWorkflow::TrackType>( "MainWorkflow::TrackType" );
    qRegisterMetaType<MainWorkflow::FrameChangedReason>( "MainWorkflow::FrameChangedReason" );
    DockWidgetManager::instance( this )->setMainWindow( this );
    initializeDockWidgets();
    createStatusBar();
    VLMCSettingsDefault::load( "default" );
    VLMCSettingsDefault::load( "VLMC" );
    m_pWizard = new ProjectWizard( this );
    createGlobalPreferences();
    createProjectPreferences();

    // Translations
    connect( this, SIGNAL( translateDockWidgetTitle() ),
             DockWidgetManager::instance(), SLOT( transLateWidgetTitle() ) );

    // Zoom
    connect( m_zoomSlider, SIGNAL( valueChanged( int ) ),
             m_timeline, SLOT( changeZoom( int ) ) );
    connect( m_timeline->tracksView(), SIGNAL( zoomIn() ),
             this, SLOT( zoomIn() ) );
    connect( m_timeline->tracksView(), SIGNAL( zoomOut() ),
             this, SLOT( zoomOut() ) );
    connect( this, SIGNAL( toolChanged( ToolButtons ) ),
             m_timeline, SLOT( setTool( ToolButtons ) ) );

    QSettings s;
    // Restore the geometry
    restoreGeometry( s.value( "MainWindowGeometry" ).toByteArray() );
    // Restore the layout
    restoreState( s.value( "MainWindowState" ).toByteArray() );
}

MainWindow::~MainWindow()
{
    QSettings s;
    // Save the current geometry
    s.setValue( "MainWindowGeometry", saveGeometry() );
    // Save the current layout
    s.setValue( "MainWindowState", saveState() );
    s.sync();

    if ( m_renderer )
        delete m_renderer;
    LibVLCpp::Instance::destroyInstance();
}

void MainWindow::changeEvent( QEvent *e )
{
    switch ( e->type() )
    {
    case QEvent::LanguageChange:
        m_ui.retranslateUi( this );
        emit translateDockWidgetTitle();
        break;
    default:
        break;
    }
}

void        MainWindow::setupLibrary()
{
    //GUI part :

    MediaLibraryWidget* mediaLibraryWidget = new MediaLibraryWidget( this );

    DockWidgetManager::instance()->addDockedWidget( mediaLibraryWidget,
                                                    tr( "Media Library" ),
                                                    Qt::AllDockWidgetAreas,
                                                    QDockWidget::AllDockWidgetFeatures,

                                                    Qt::LeftDockWidgetArea );
    connect( mediaLibraryWidget, SIGNAL( mediaSelected( Media* ) ),
             m_clipPreview->getGenericRenderer(), SLOT( setMedia( Media* ) ) );

    connect( Library::getInstance(), SIGNAL( mediaRemoved( const QUuid& ) ),
             m_clipPreview->getGenericRenderer(), SLOT( mediaUnloaded( QUuid ) ) );
}

void    MainWindow::on_actionSave_triggered()
{
    QString outputFileName =
            QFileDialog::getSaveFileName( NULL, "Enter the output file name",
                                          QString(), "VLMC project file(*.vlmc)" );
    if ( outputFileName.length() == 0 )
        return ;
    else
    {
        //Project manager will destroy itself.
        QStringList list = outputFileName.split( "." );
        if ( list.at( list.size() - 1 ) != "vlmc" )
        {
            list.append( "vlmc" );
            outputFileName = list.join(".");
        }

        ProjectManager* pm = new ProjectManager( outputFileName );
        pm->saveProject();
    }
}

void    MainWindow::on_actionLoad_Project_triggered()
{
    QString outputFileName =
            QFileDialog::getOpenFileName( NULL, "Enter the output file name",
                                          QString(), "VLMC project file(*.vlmc)" );
    if ( outputFileName.length() == 0 )
        return ;
    else
    {
        //Project manager will destroy itself.
        ProjectManager* pm = new ProjectManager( outputFileName );
        pm->loadProject();
    }
}

void MainWindow::createStatusBar()
{
    // Mouse (default) tool
    QToolButton* mouseTool = new QToolButton( this );
    mouseTool->setCheckable( true );
    mouseTool->setIcon( QIcon( ":/images/mouse" ) );
    m_ui.statusbar->addPermanentWidget( mouseTool );

    // Cut/Split tool
    QToolButton* splitTool = new QToolButton( this );
    splitTool->setCheckable( true );
    splitTool->setIcon( QIcon( ":/images/editcut" ) );
    m_ui.statusbar->addPermanentWidget( splitTool );

    // Group the two previous buttons
    QButtonGroup* toolButtonGroup = new QButtonGroup( this );
    toolButtonGroup->addButton( mouseTool, TOOL_DEFAULT);
    toolButtonGroup->addButton( splitTool, TOOL_CUT );
    toolButtonGroup->setExclusive( true );
    mouseTool->setChecked( true );

    connect( toolButtonGroup, SIGNAL( buttonClicked( int ) ),
             this, SLOT( toolButtonClicked( int ) ) );

    // Spacer
    QWidget* spacer = new QWidget( this );
    spacer->setFixedWidth( 20 );
    m_ui.statusbar->addPermanentWidget( spacer );

    // Zoom Out
    QToolButton* zoomOutButton = new QToolButton( this );
    zoomOutButton->setIcon( QIcon( ":/images/zoomout" ) );
    m_ui.statusbar->addPermanentWidget( zoomOutButton );
    connect( zoomOutButton, SIGNAL( clicked() ),
             this, SLOT( zoomOut() ) );

    // Zoom slider
    m_zoomSlider = new QSlider( this );
    m_zoomSlider->setOrientation( Qt::Horizontal );
    m_zoomSlider->setTickInterval( 1 );
    m_zoomSlider->setSingleStep( 1 );
    m_zoomSlider->setPageStep( 1 );
    m_zoomSlider->setMinimum( 0 );
    m_zoomSlider->setMaximum( 13 );
    m_zoomSlider->setValue( 10 );
    m_zoomSlider->setFixedWidth( 80 );
    m_zoomSlider->setInvertedAppearance( true );
    m_ui.statusbar->addPermanentWidget( m_zoomSlider );

    // Zoom IN
    QToolButton* zoomInButton = new QToolButton( this );
    zoomInButton->setIcon( QIcon( ":/images/zoomin" ) );
    m_ui.statusbar->addPermanentWidget( zoomInButton );
    connect( zoomInButton, SIGNAL( clicked() ),
             this, SLOT( zoomIn() ) );
}

void MainWindow::initializeDockWidgets( void )
{
    WorkflowRenderer*    workflowRenderer = new WorkflowRenderer();
    m_timeline = new Timeline( workflowRenderer, this );
    m_timeline->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    m_timeline->show();
    setCentralWidget( m_timeline );

    DockWidgetManager *dockManager = DockWidgetManager::instance();

    m_clipPreview = new PreviewWidget( new ClipRenderer, this );
    dockManager->addDockedWidget( m_clipPreview,
                                  tr( "Clip Preview" ),
                                  Qt::AllDockWidgetAreas,
                                  QDockWidget::AllDockWidgetFeatures,
                                  Qt::TopDockWidgetArea );
    QShortcut*  clipShortcut = new QShortcut( QKeySequence( tr( "Ctrl+Return", "Start clip preview" ) ), this );
    connect( clipShortcut, SIGNAL( activated() ), m_clipPreview, SLOT( on_pushButtonPlay_clicked() ) );

    m_projectPreview = new PreviewWidget( workflowRenderer, this );
    dockManager->addDockedWidget( m_projectPreview,
                                  tr( "Project Preview" ),
                                  Qt::AllDockWidgetAreas,
                                  QDockWidget::AllDockWidgetFeatures,
                                  Qt::TopDockWidgetArea );
    QShortcut*  renderShortcut = new QShortcut( QKeySequence( tr( "Space", "Start render preview" ) ), this );
    connect( renderShortcut, SIGNAL( activated() ), m_projectPreview, SLOT( on_pushButtonPlay_clicked() ) );

    dockManager->addDockedWidget( UndoStack::getInstance( this ),
                                  tr( "History" ),
                                  Qt::AllDockWidgetAreas,
                                  QDockWidget::AllDockWidgetFeatures,
                                  Qt::LeftDockWidgetArea );
    setupLibrary();
<<<<<<< HEAD
=======
    setupEffectBox();
    m_metaDataManager = MetaDataManager::getInstance();
>>>>>>> kri5_EffectBox
}

void        MainWindow::createGlobalPreferences()
{
    m_globalPreferences = new Settings( false, "VLMC", this );
    m_globalPreferences->addWidget("VLMC",
                                   new VLMCPreferences( m_globalPreferences ),
                                   "../images/vlmc.png",
                                   "VLMC settings");
    m_globalPreferences->addWidget("Language preferences",
                                   new LanguagePreferences( m_globalPreferences ),
                                   "../images/vlmc.png",
                                   "Langage settings");
    m_globalPreferences->build();
}

void	    MainWindow::createProjectPreferences()
{
    m_projectPreferences = new Settings( false, "project", this );
    m_projectPreferences->addWidget("Project",
                                   new ProjectPreferences,
                                   ":/images/images/vlmc",
                                   "Project settings" );
    m_projectPreferences->addWidget( "Video",
                                   new VideoProjectPreferences,
                                   ":/images/images/video",
                                   "Video settings" );
    m_projectPreferences->addWidget( "Audio",
                                   new AudioProjectPreferences,
                                   ":/images/images/audio",
                                   "Audio settings" );
    m_projectPreferences->build();
}


//Private slots definition

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionPreferences_triggered()
{
   m_globalPreferences->show();
}

void MainWindow::on_actionAbout_triggered()
{
    About::instance()->exec();
}

void MainWindow::on_actionTranscode_triggered()
{
    QMessageBox::information( this, tr( "Sorry" ),
                              tr( "This feature is currently disabled." ) );
    //Transcode::instance( this )->exec();
}

void    MainWindow::on_actionRender_triggered()
{
    if ( MainWorkflow::getInstance()->getLengthFrame() <= 0 )
    {
        QMessageBox::warning( NULL, "VLMC Renderer", "There is nothing to render." );
        return ;
    }
    QString outputFileName =
            QFileDialog::getSaveFileName( NULL, "Enter the output file name",
                                          QDir::currentPath(), "Videos(*.avi *.mpg)" );
    if ( outputFileName.length() == 0 )
        return ;
    else
    {
        if ( m_renderer )
            delete m_renderer;
        m_renderer = new WorkflowFileRenderer( outputFileName );
        m_renderer->run();
    }
}

void MainWindow::on_actionNew_Project_triggered()
{
    //TODO : clear the library, the timeline, and show the configuration box
    //of the newly created project

    m_projectPreferences->show();
}

void    MainWindow::on_actionHelp_triggered()
{
    QDesktopServices::openUrl( QUrl( "http://vlmc.org" ) );
}

void    MainWindow::on_actionImport_triggered()
{
    //Import* import = new Import( );
    //import->exec();
}

void    MainWindow::zoomIn()
{
    m_zoomSlider->setValue( m_zoomSlider->value() - 1 );
}

void    MainWindow::zoomOut()
{
    m_zoomSlider->setValue( m_zoomSlider->value() + 1 );
}

void    MainWindow::on_actionFullscreen_triggered( bool checked )
{
    if ( checked )
        showFullScreen();
    else
        showNormal();
}

void    MainWindow::registerWidgetInWindowMenu( QDockWidget* widget )
{
    m_ui.menuWindow->addAction( widget->toggleViewAction() );
}

void    MainWindow::toolButtonClicked( int id )
{
    emit toolChanged( (ToolButtons)id );
}

void MainWindow::on_actionBypass_effects_engine_toggled(bool toggled)
{
    EffectsEngine*  ee;

    ee = MainWorkflow::getInstance()->getEffectsEngine();
    if (toggled)
        ee->enable();
    else
       ee->disable();
    return ;
}

void MainWindow::on_actionProject_Preferences_triggered()
{
  m_projectPreferences->show( "project" );
}

void MainWindow::on_actionProject_Wizard_triggered()
{
    m_pWizard->show();
}

void MainWindow::setupEffectBox()
{
    EffectBoxView* effectBoxView = EffectBoxView::getInstance();
    DockWidgetManager::instance()->addDockedWidget( effectBoxView,
                                  tr( "Effect Box" ),
                                  Qt::AllDockWidgetAreas,
                                  QDockWidget::AllDockWidgetFeatures,
                                  Qt::BottomDockWidgetArea );
    EffectBoxScene* effectBoxScene = new EffectBoxScene( this );
    effectBoxScene->addEffect( "test" );
    effectBoxView->setScene( effectBoxScene );
}
