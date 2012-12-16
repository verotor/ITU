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

#ifndef STARMENU_H
#define STARMENU_H

#include "staritem.h"

#include <QtDeclarative/QDeclarativeItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QGradient>
#include <QHash>
#include <QList>
#include <math.h>
#include <QDeclarativeContext>


// TODO: force items inside screen
class StarMenu : public QDeclarativeItem
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
//    Q_DISABLE_COPY(StarMenu)
    Q_ENUMS(ShapeKind)
    Q_PROPERTY (bool              open                READ open WRITE setOpen NOTIFY openChanged)
    Q_PROPERTY (QDeclarativeItem* visualParent        READ visualParent WRITE setVisualParent NOTIFY visualParentChanged)
    Q_PROPERTY (QColor            color               READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY (QPointF           center              READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY (ShapeKind         shape               READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY (qreal             spacingH            READ spacingH WRITE setSpacingH NOTIFY spacingHChanged)
    Q_PROPERTY (qreal             spacingV            READ spacingV WRITE setSpacingV NOTIFY spacingVChanged)
    Q_PROPERTY (int               columns             READ columns WRITE setColumns NOTIFY columnsChanged)
    Q_PROPERTY (qreal             radiusH             READ radiusH WRITE setRadiusH NOTIFY radiusHChanged)
    Q_PROPERTY (qreal             radiusV             READ radiusV WRITE setRadiusV NOTIFY radiusVChanged)
    Q_PROPERTY (qreal             rotation            READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY (bool              starCenterVisible   READ starCenterVisible WRITE setStarCenterVisible NOTIFY starCenterVisibleChanged)
    Q_PROPERTY (int               delayChangePos      READ delayChangePos WRITE setDelayChangePos NOTIFY delayChangePosChanged)
    Q_PROPERTY (bool              useParentMenuCenter READ useParentMenuCenter WRITE setUseParentMenuCenter NOTIFY useParentMenuCenterChanged)
    Q_PROPERTY (qreal             startingRadius      READ startingRadius WRITE setStartingRadius NOTIFY startingRadiusChanged)
    Q_PROPERTY (qreal             radiusIncrement     READ radiusIncrement WRITE setRadiusIncrement NOTIFY radiusIncrementChanged)
    Q_PROPERTY (qreal             startingAngle       READ startingAngle WRITE setStartingAngle NOTIFY startingAngleChanged)
    Q_PROPERTY (bool              parentVisible       READ parentVisible WRITE setParentVisible NOTIFY parentVisibleChanged)

public:
    enum { Type = UserType + 1 };
    enum ShapeKind { ShapeCircle, ShapeGrid, ShapeSpiral, ShapeLine };
    StarMenu(QDeclarativeItem *parent = 0);
    ~StarMenu();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    int type() const { return Type; }
    void componentComplete();
    Q_INVOKABLE void centerChildItems();

    QPointF scenePosInPortrait(QPointF p);

#ifdef MEEGO_EDITION_HARMATTAN
    int getOrientation();
#endif

    QPointF startMousePos;
    static QDeclarativeContext *rootContext;


    ////////////////////////////////////////////////////
    /// Properties
    bool open() { return m_open; }
    void setOpen(bool b) {
        if (m_open && !b) onStarMenuClosing();
        if (!m_open && b) {  onStarMenuOpening(); updateStarItemPos(); }
        m_open = b;
//        emit openChanged();
    }
    bool m_open;

    QDeclarativeItem * visualParent() { return m_visualParent; }
    void setVisualParent(QDeclarativeItem *vParent) { m_visualParent = vParent; emit visualParentChanged(); }
    QDeclarativeItem *m_visualParent;

    QColor color() { return m_color; }
    void setColor(QColor c) {
        m_color = c;
        setFlag(QGraphicsItem::ItemHasNoContents, !m_starCenterVisible && m_color.alpha()==0 );
        update();
        emit colorChanged();
    }
    QColor m_color;

    QPointF center() { if (!m_center.isNull()) return m_center; else return startMousePos; }
    void setCenter(QPointF c) {
        startMousePos = c;
        m_center = c;
        starShape.clear();
        updateStarItemPos();
        emit centerChanged();
    }
    QPointF m_center;

    ShapeKind shape() { return m_shape; }
    void setShape(ShapeKind shape) { m_shape = shape; updateStarItemPos(); emit shapeChanged(); }
    ShapeKind m_shape;

    qreal rotation() { return m_rotation; }
    void setRotation(qreal r) {
        // Here some attempts to rotate the menu around its center.
        // I give up and ended to rotate the items instead the QGraphicsItem :( (circle and spiral shape only)
        m_rotation = r;
        m_rotation = M_PI*2*r/360;
        starShape.clear();
        updateStarItemPos();

//        QPointF c = static_cast<QGraphicsObject *>(this)->transformOriginPoint();
//        qDebug() << m_center << c;

//        static_cast<QGraphicsObject *>(this)->setTransformOriginPoint( startMouseItemPos.x(),startMouseItemPos.y() );
//        static_cast<QGraphicsObject *>(this)->resetTransform();
//        static_cast<QGraphicsObject *>(this)->setRotation(m_rotation);
//        static_cast<QGraphicsObject *>(this)->setTransformOriginPoint( startMousePos.x(), startMousePos.y() );

//        QPointF cr = m_center;
//        setCenter(startMouseItemPos);
//        static_cast<QGraphicsObject *>(this)->resetTransform();
//        static_cast<QGraphicsObject *>(this)->rotate(m_rotation);
//        setCenter(cr);


//        static_cast<QGraphicsItem *>(this)->setTransform( QTransform().
//                                                          translate( -m_center.x(), -m_center.y() ).
//                                                          rotate(m_rotation).
//                                                          translate( m_center.x(), m_center.y() )  );

        emit rotationChanged();
    }
    qreal m_rotation;

    qreal spacingH() { return m_spacingH; }
    void setSpacingH(qreal spacing) { m_spacingH = spacing; updateStarItemPos(); emit spacingHChanged(); }
    qreal m_spacingH;

    qreal spacingV() { return m_spacingV; }
    void setSpacingV(qreal spacing) { m_spacingV = spacing; updateStarItemPos(); emit spacingVChanged(); }
    qreal m_spacingV;

    int columns() { return m_columns; }
    void setColumns(int col) { m_columns = col; updateStarItemPos(); emit columnsChanged(); }
    qreal m_columns;

    qreal radiusH() { return m_radius_h; }
    void setRadiusH(qreal radius_h) { m_radius_h = radius_h; starShape.clear(); updateStarItemPos(); emit radiusHChanged(); }
    qreal m_radius_h;

    qreal radiusV() { return m_radius_v; }
    void setRadiusV(qreal radius_v) { m_radius_v = radius_v; starShape.clear(); updateStarItemPos(); emit radiusVChanged(); }
    qreal m_radius_v;

    qreal radiusIncrement() { return m_radiusIncrement; }
    void setRadiusIncrement(qreal increment) { m_radiusIncrement = increment; updateStarItemPos(); emit radiusIncrementChanged(); }
    qreal m_radiusIncrement;

    qreal startingRadius() { return m_startingRadius; }
    void setStartingRadius(qreal radius) { m_startingRadius = radius; updateStarItemPos(); emit startingRadiusChanged(); }
    qreal m_startingRadius;

    int delayChangePos() { return m_delayChangePos; }
    void  setDelayChangePos( int delay ) { m_delayChangePos = delay; emit delayChangePosChanged(); }
    int m_delayChangePos;

    bool starCenterVisible() { return m_starCenterVisible; }
    void setStarCenterVisible(bool visible) {
        m_starCenterVisible = visible;
        setFlag(QGraphicsItem::ItemHasNoContents, !m_starCenterVisible && m_color.alpha()==0 );
        emit starCenterVisibleChanged();
    }
    bool m_starCenterVisible;

    bool useParentMenuCenter() { return m_useParentMenuCenter; }
    void setUseParentMenuCenter(bool sameCenter) {
        m_useParentMenuCenter = sameCenter;
        emit useParentMenuCenterChanged();
    }
    bool m_useParentMenuCenter;

    qreal startingAngle() { return m_startingAngle; }
    void setStartingAngle(qreal a) { m_startingAngle = -2*M_PI*a/360; updateStarItemPos(); emit startingAngleChanged(); }
    qreal m_startingAngle;

    bool parentVisible() { return m_parentVisible; }
    void setParentVisible(bool visible) {
        m_parentVisible = visible;
        emit parentVisibleChanged();
    }
    bool m_parentVisible;


private:
    void calcStarShape();
    int  calculateNumberOfItems();
    void arrangeLineShape();
    void arrangeCircleShape();
    void arrangeGridShape();
    void arrangeSpiralShape();
    void setChildsOpacity(qreal o, bool propagate);
    void setParentClosedProperties();
    void debugList();
    void stopAllChildAnimations(StarMenu *target);
    bool checkChildsAnimationsRunning(StarMenu *mainItem);

    // if visualParent is given, this is used to restore the original parentItem when closing the menu
    QDeclarativeItem *realParent;
    static QHash< int , QList<StarMenu *> >  listOpenedStar;
    static int rootCount;
    int rootIndex;
    int starItemChildrensCount;
    QPolygonF starShape;

protected:
    bool sceneEvent ( QEvent * event );

public Q_SLOTS:
    void onStarMenuOpening();
    void onStarMenuClosing();
    Q_INVOKABLE void updateStarItemPos();

Q_SIGNALS:
    void starMenuOpening();
    void starMenuClosing();

    ////////////////////////////////////////////////////
    /// Properties signals
    void openChanged();
    void visualParentChanged();
    void colorChanged();
    void centerChanged();
    void rotationChanged();
    void spacingHChanged();
    void spacingVChanged();
    void columnsChanged();
    void radiusHChanged();
    void radiusVChanged();
    void startingRadiusChanged();
    void shapeChanged();
    void radiusIncrementChanged();
    void delayChangePosChanged();
    void useParentMenuCenterChanged();
    void starCenterVisibleChanged();
    void startingAngleChanged();
    void parentVisibleChanged();
};

QML_DECLARE_TYPE(StarMenu)



#endif // STARMENU_H

