#ifndef EFFECTEDITORWIDGET
#define EFFECTEDITORWIDGET

#include <QWidget>
#include <QTabWidget>

#include "EffectEditorView.h"

class   EffectEditorWidget : public QTabWidget
{
    //Q_OBJECT
    //Q_DISABLE_COPY( EffectEditorWidget )
    public:
        explicit EffectEditorWidget( QWidget* parent = 0 );
        ~EffectEditorWidget();

    private:
        EffectEditorView*       m_view;
};

#endif
