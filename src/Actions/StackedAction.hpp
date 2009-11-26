/*****************************************************************************
 * StackedAction.hpp: Stores an action to be executed later
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

#ifndef STACKEDACTION_H
#define STACKEDACTION_H

#include <QUuid>

#include "MainWorkflow.h"

namespace Action
{
    class   Generic
    {
        public:
            enum    Type
            {
                Pause,
                Unpause,
                Resize,
                Remove,
                Add,
            };
            Generic( Type type ) : m_type( type ) {}
            virtual ~Generic(){}
            virtual void    execute() = 0;
            virtual bool    isOpposite( const Generic* ) const
            {
                return false;
            }
            Type    getType() const
            {
                return m_type;
            }
        private:
            Type    m_type;
    };

    class   Workflow : public Generic
    {
        public:
            Workflow( MainWorkflow* mainWorkflow, Type type ) : Generic( type ), m_mainWorkflow( mainWorkflow ) {}
        protected:
            MainWorkflow*   m_mainWorkflow;
    };

    class   Track : public Workflow
    {
        public:
            Track( MainWorkflow* mainWorkflow, uint32_t trackId, MainWorkflow::TrackType trackType, Type type ) :
                Workflow( mainWorkflow, type ), m_trackId( trackId ), m_trackType( trackType )
            {
            }
        protected:
            uint32_t                    m_trackId;
            MainWorkflow::TrackType     m_trackType;
    };

    class   AddClip : public Track
    {
        public:
            AddClip( MainWorkflow* mainWorkflow, uint32_t trackId, MainWorkflow::TrackType trackType,
                           Clip* clip, qint64 startingPos ) : Track( mainWorkflow, trackId, trackType, Add ),
                                                            m_clip( clip ), m_startingPos( startingPos )
            {
            }
            void        execute()
            {
                m_mainWorkflow->addClip( m_clip, m_trackId, m_startingPos, m_trackType );
            }
        protected:
            Clip*       m_clip;
            qint64      m_startingPos;
    };

    class   RemoveClip : public Track
    {
        public:
            RemoveClip( MainWorkflow* mainWorkflow, uint32_t trackId, MainWorkflow::TrackType trackType,
                           const QUuid& uuid ) : Track( mainWorkflow, trackId, trackType, Remove ),
                                                m_uuid( uuid )
            {
            }
            void        execute()
            {
                m_mainWorkflow->removeClip( m_uuid, m_trackId, m_trackType );
            }
        protected:
            QUuid       m_uuid;
    };

    class   ResizeClip : public Generic
    {
        public:
            ResizeClip( Clip* clip, qint64 newBegin, qint64 newEnd ) : Generic( Resize ),
                    m_clip( clip ), m_newBegin( newBegin ), m_newEnd( newEnd )
            {
            }
            void    execute()
            {
                m_clip->setBoundaries( m_newBegin, m_newEnd );
            }
        protected:
            Clip*   m_clip;
            qint64  m_newBegin;
            qint64  m_newEnd;
    };

    class   Pause : public Workflow
    {
        public:
            Pause( MainWorkflow* mainWorkflow ) : Workflow( mainWorkflow, Generic::Pause )
            {
            }
            void    execute()
            {
                m_mainWorkflow->pause();
            }
            bool    isOpposite( const Generic* act ) const
            {
                return ( act->getType() == Unpause );
            }
    };

    class   Unpause : public Workflow
    {
        public:
            Unpause( MainWorkflow* mainWorkflow ) : Workflow( mainWorkflow, Generic::Unpause )
            {
            }
            void    execute()
            {
                m_mainWorkflow->unpause();
            }
            bool    isOpposite( const Generic* act ) const
            {
                return ( act->getType() == Generic::Pause );
            }
    };
}
#endif // STACKEDACTION_H
