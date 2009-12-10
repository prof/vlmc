//DEBUG
#include <QtDebug>
//!DEBUG

#include "EffectEditorWidget.h"
#include "EffectEditorScene.h"

EffectEditorWidget::EffectEditorWidget( QWidget* parent )
    : QTabWidget( parent )
{
    m_view = new EffectEditorView();
    setTabsClosable( true );
    EffectEditorScene* effectEditorScene = new EffectEditorScene( this );
    //TODO : plug the effect Engine here
    effectEditorScene->addEffect( "test" );
    //!TODO
    m_view->setScene( effectEditorScene );
    addTab( m_view, tr( "Effect Editor" ) );
}

EffectEditorWidget::~EffectEditorWidget()
{
}
