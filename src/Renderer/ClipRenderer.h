/*****************************************************************************
 * ClipRenderer.h: Preview widget
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
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

#ifndef CLIPRENDERER_H
#define CLIPRENDERER_H

class   Clip;
class   Media;
class   QUuid;

namespace LibVLCpp
{
    class   Media;
}

#include "GenericRenderer.h"

#include <QObject>

class ClipRenderer : public GenericRenderer
{
    Q_OBJECT
    Q_DISABLE_COPY( ClipRenderer )

public:
    explicit ClipRenderer();
    virtual ~ClipRenderer();

    virtual void            togglePlayPause( bool forcePause );
    virtual void            stop();
    virtual void            nextFrame();
    virtual void            previousFrame();
    virtual qint64          getLength() const;
    virtual qint64          getLengthMs() const;
    virtual qint64          getCurrentFrame() const;
    virtual float           getFps() const;
    virtual Media*          getMedia() const;

private:
    void                    startPreview();

private:
    bool                    m_clipLoaded;
    LibVLCpp::Media*        m_vlcMedia;
    Media*                  m_selectedMedia;
    qint64                  m_begin;
    qint64                  m_end;
    /**
     *  \brief  This flags is used to know if a new media has been selected in the
     * library. If so, we must relaunch the render if the play button is clicked again.
     */
    bool                    m_mediaChanged;

public slots:
    virtual void            setClip( Clip* clip );
    virtual void            setMedia( Media* media );
    virtual void            mediaUnloaded( const QUuid& );
    virtual void            previewWidgetCursorChanged( qint64 newFrame );

    /**
     *  \brief      Triggered at every libvlc_MediaPlayerTimeChanged event.
     *
     *  This slot will compute a frame number based on the time and the clip's FPS.
     *  Once computed, it will emit a frameChanged signal, with the reason Renderer.
     *  \warning    The frame number computed may be unaccurate.
     *  \sa         frameChanged();
     */
    void                    __timeChanged( qint64 time );
    void                    __endReached();
    void                    __videoStopped();
};

#endif // CLIPRENDERER_H
