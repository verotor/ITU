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

#ifndef STARITEM_H
#define STARITEM_H

#include <QDeclarativeItem>
#include <QGraphicsSceneHoverEvent>
#include <QTimer>

class StarItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY (qreal  zoomHoverValue READ zoomHoverValue WRITE setZoomHoverValue NOTIFY zoomHoverValueChanged)
    // if delayChangePos is not set ( default 0 ),
    Q_PROPERTY (int    delayChangePos READ delayChangePos WRITE setDelayChangePos NOTIFY delayChangePosChanged)

public:
    enum { Type = UserType + 2 };

    StarItem(QDeclarativeItem *parent = 0);
    int type() const { return Type; }
    void componentComplete();
    void setDelayedPos(qreal x, qreal y, int delay);
    void emitHoverItem();
    void emitLeaveItem();
    void resetScale();
    void stopTimer();

    ////////////////////////////////////////////////////
    // Properties
    int delayChangePos() { return m_delayChangePos; }
    void setDelayChangePos(int delay) { m_delayChangePos = delay; emit delayChangePosChanged(); }
    int m_delayChangePos;

    qreal zoomHoverValue() { return m_zoomHoverValue; }
    void  setZoomHoverValue( qreal zoom );
    qreal m_zoomHoverValue;


private:
    qreal m_x, m_y, m_z;
    qreal startingScaleValue;
    bool isHover;
    QTimer timer;


protected:
    bool sceneEvent ( QEvent * event );

public Q_SLOTS:
    void onTimerTriggered();


Q_SIGNALS:
    void hoverItem();
    void leaveItem();
    void clicked();

    ////////////////////////////////////////////////////
    // Properties signals
    void enabledChanged();
    void delayChangePosChanged();
    void zoomHoverValueChanged();
};

QML_DECLARE_TYPE(StarItem)

#endif // STARITEM_H
