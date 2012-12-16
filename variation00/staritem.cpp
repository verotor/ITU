/***************************************************************************
*  StarMenu QML plugin v 1.0.3                                             *
*                                                                          *
*  Copyright (C) 2011-2012  Marco Bavagnoli - lil.deimos@gmail.com         *
*                                                                          *
*  This program is free software: you can redistribute it and/or modify    *
*  it under the terms of the GNU General Public License as published by    *
*  the Free Software Foundation, either version 3 of the License, or       *
*  (at your option) any later version.                                     *
*                                                                          *
*  This program is distributed in the hope that it will be useful,         *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*  GNU General Public License for more details.                            *
*                                                                          *
*  You should have received a copy of the GNU General Public License       *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
***************************************************************************/

#include "staritem.h"
#include "starmenu.h"

#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QDebug>

StarItem::StarItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    m_delayChangePos(0),
    m_zoomHoverValue(1),
    startingScaleValue(-1)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setFlag(QGraphicsItem::ItemHasNoContents, true);
    setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, false);
    setAcceptedMouseButtons (Qt::LeftButton | Qt::RightButton | Qt::MidButton);
    setAcceptHoverEvents (true);
    setAcceptTouchEvents (true);
}


bool StarItem::sceneEvent ( QEvent * event )
{
    switch (event->type()) {
        case QEvent::GraphicsSceneHoverEnter:
        {
            if (startingScaleValue == -1)
                startingScaleValue = property ("scale").toFloat ();
            // Send the hoverItem signal only if parent is a StarMenu and it is opened
            if (static_cast<QGraphicsObject*>(this->parent())->type () == UserType+1 &&
                static_cast<StarMenu*>(this->parent())->open() ) {
                emitHoverItem();
            }
            break;
        }
        case QEvent::GraphicsSceneHoverLeave:
        {
            // Send the hoverItem signal only if parent is a StarMenu and it is opened
            if (static_cast<QGraphicsObject*>(this->parent())->type () == UserType+1 &&
                static_cast<StarMenu*>(this->parent())->open() ) {
                emitLeaveItem();
            }
            break;
        }
        case QEvent::GraphicsSceneMousePress:
        {
            resetScale();
            emit clicked();
            break;
        }
        default:
            return QGraphicsItem::sceneEvent(event);
    }
    return true;
}


void StarItem::componentComplete()
{
    startingScaleValue = property("scale").toReal();
    QDeclarativeItem::componentComplete();
}


void StarItem::setDelayedPos(qreal x, qreal y, int delay)
{
    m_x = x;
    m_y = y;
    // If the delay is been set use this, else use the value passed
    if (m_delayChangePos != 0) delay = m_delayChangePos;
    timer.singleShot ( delay, this, SLOT(onTimerTriggered()) );
}

void StarItem::stopTimer()
{
    timer.stop();
}

void StarItem::onTimerTriggered()
{
    setProperty ("x", m_x);
    setProperty ("y", m_y);
}


void StarItem::emitHoverItem()
{
    if (isHover) return;
    if (opacity ()==0) return;
    isHover = true;
    m_z = zValue ();
    setZValue (9999);
    if (m_zoomHoverValue != startingScaleValue)
        setProperty ("scale", m_zoomHoverValue);
    emit hoverItem ();
}


void StarItem::emitLeaveItem()
{
    setZValue (m_z);
    if (m_zoomHoverValue != startingScaleValue)
        setProperty ("scale", startingScaleValue);
    isHover = false;
    emit leaveItem ();
}


void StarItem::setZoomHoverValue( qreal zoom )
{
    m_zoomHoverValue = zoom;
    emit zoomHoverValueChanged();
}


void StarItem::resetScale()
{
    if (m_zoomHoverValue != startingScaleValue) {
        setProperty("scale", startingScaleValue);
        setScale(startingScaleValue);
    }
    else {
        setProperty("scale", 1);
        setScale(1);
    }
}
