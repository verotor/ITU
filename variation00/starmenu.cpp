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

#include "starmenu.h"
#include "staritem.h"

#include <QtDeclarative/qdeclarative.h>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QApplication>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPen>
#include <QPainter>
#include <QEvent>
#include <QDebug>


QHash< int , QList<StarMenu *> > StarMenu::listOpenedStar = QHash< int , QList<StarMenu *> >();
int StarMenu::rootCount=0;
QDeclarativeContext *StarMenu::rootContext=0;

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
/// StarMenu
StarMenu::StarMenu(QDeclarativeItem *parent):
    QDeclarativeItem(parent),
    m_visualParent(0),
    m_color(QColor(0,0,0,0)),
    m_center(QPointF(0,0)),
    m_shape(ShapeCircle),
    m_spacingH(15),
    m_spacingV(15),
    m_columns(4),
    m_radius_h(100),
    m_radius_v(100),
    m_radiusIncrement(10),
    m_starCenterVisible(true),
    m_delayChangePos(0),
    m_useParentMenuCenter(false),
    m_open(false),
    m_startingAngle(0),
    starItemChildrensCount(0),
    realParent(0),
    m_parentVisible(false)
{
//    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemHasNoContents, !m_starCenterVisible);
    setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);

    setAcceptedMouseButtons (Qt::LeftButton | Qt::RightButton | Qt::MidButton);
    setAcceptTouchEvents (true);
}

StarMenu::~StarMenu()
{
}


void StarMenu::componentComplete()
{
    // If this->parent isn't a StarItem, this StarMenu must be considered as root menu
    if (this->parent() && static_cast<QDeclarativeItem *>(this->parent())->type() != UserType + 2) {
        rootIndex=rootCount;
        StarMenu::rootCount++;

        // Start with this and all childs hidden
        setChildsOpacity (0, true);
    }

    // Connect all StarItems visible property to update
    foreach (QGraphicsItem *item, childItems())
        if (item->type() == item->UserType+2)
            connect( static_cast<QDeclarativeItem *>(item) , SIGNAL(visibleChanged()),
                     this, SLOT(updateStarItemPos()) );


    QDeclarativeItem::componentComplete();
}


void StarMenu::calcStarShape()
{
    // Compose the star
    qreal radiusMax = 100;
    qreal radiusMin = 30;
    int i = calculateNumberOfItems() << 1;
    for (int n=0; n<i; n+=2) {
        starShape.append (QPointF(
                              cos(2*M_PI/i*n-m_rotation)*radiusMax,
                              sin(2*M_PI/i*n-m_rotation)*radiusMax
                              )*(qMin( m_radius_h , m_radius_v ) /100)/2+startMousePos - scenePosInPortrait(scenePos()));
        starShape.append (QPointF(
                              cos(2*M_PI/i*(n+1)-m_rotation)*radiusMin,
                              sin(2*M_PI/i*(n+1)-m_rotation)*radiusMin
                              )*(qMin( m_radius_h , m_radius_v ) /100)/2+startMousePos - scenePosInPortrait(scenePos()));
    }
}

// paint() is called only if color or starCenterVisible are set
void StarMenu::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if (!m_open) return;
    if (startMousePos.isNull ()) return;

    // Fill the backgroung if color is set
    if (m_color!=QColor(0,0,0,0))
        painter->fillRect(this->boundingRect(), m_color);

    if (!m_starCenterVisible) return;

    if (starShape.isEmpty ())
        calcStarShape();

    QRadialGradient radialGrad(starShape.boundingRect().center(), qMin( m_radius_h , m_radius_v )/2);
    radialGrad.setColorAt(0, Qt::white);
    radialGrad.setColorAt(0.3, Qt::blue);
    radialGrad.setColorAt(1, QColor(0,0,100,0.5));

    QBrush starBrush(radialGrad);
    painter->setBrush (starBrush);
    painter->setPen (QPen(QColor(Qt::black),0));

    if (smooth()) painter->setRenderHint(QPainter::Antialiasing, true);

    painter->drawPolygon ( starShape );
}


#ifdef MEEGO_EDITION_HARMATTAN
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusMessage>
#include <QDesktopWidget>

int StarMenu::getOrientation()
{
    int orientation = -1;

#if !defined(QT_NO_DBUS)
    QDBusMessage reply = QDBusConnection::systemBus().call(
                             QDBusMessage::createMethodCall("com.nokia.SensorService", "/org/maemo/contextkit/Screen/TopEdge",
                                                            "org.maemo.contextkit.Property", "Get"));
    if (reply.type() != QDBusMessage::ErrorMessage) {
        QList<QVariant> args;
        qvariant_cast<QDBusArgument>(reply.arguments().at(0)) >> args;
        if (args.count() == 0) {
            return orientation;
        }
        QString nativeOrientation = args.at(0).toString();
        if (nativeOrientation == "top") {
            orientation = 1;    // Landscape
        } else if (nativeOrientation == "left") {
            orientation = 2;    // Portrait
        } else if (nativeOrientation == "bottom") {
            orientation = 3;    //InvertedLandscape
        } else if (nativeOrientation == "right") {
            orientation = 4;    // InvertedPortrait
        }
    }
#endif
    return orientation;
}
#endif // MEEGO_EDITION_HARMATTAN

QPointF StarMenu::scenePosInPortrait(QPointF p)
{
    QPointF ret = p;

#ifdef MEEGO_EDITION_HARMATTAN
    int orientation = getOrientation();
    if ( orientation == 2 ) {
        ret.setX( QApplication::desktop()->height() -p.y() );
        ret.setY( p.x() );
    }
#endif

    return ret;
}

bool StarMenu::sceneEvent ( QEvent * event )
{

    switch (event->type()) {

        case QEvent::GraphicsSceneMousePress: break;
        case QEvent::GraphicsSceneMouseRelease:
        {
            if (static_cast<QGraphicsSceneMouseEvent*>(event)->button() == Qt::LeftButton) {

                startMousePos =  scenePosInPortrait(static_cast<QGraphicsSceneMouseEvent*>(event)->scenePos());

                // Use parent center if useParentMenuCenter
                if (m_useParentMenuCenter) {
                    if (listOpenedStar[rootIndex].count()>0) {
                        // It is sure that the parent is a StarItem and this StarMenu is his child
                        startMousePos = listOpenedStar[rootIndex].last()->startMousePos;
                    }
                }
                // Use center if it is set
                if (!m_center.isNull())
                    startMousePos = mapToScene(m_center)-scenePosInPortrait(scenePos());

                if (m_open)
                    onStarMenuClosing ();
                else
                    onStarMenuOpening ();
            }
            break;
        }
        default:
        return QGraphicsItem::sceneEvent(event);
    }
    return true;
}






void StarMenu::onStarMenuClosing()
{
//    qDebug() << "CLOSED1:"; debugList();

    // Check if there are animations running, if so don't open the menu
    if (checkChildsAnimationsRunning( this ) )
        return;
    for (int i=0; i < listOpenedStar[rootIndex].count(); i++)
        if (checkChildsAnimationsRunning( listOpenedStar[rootIndex].at(i) ) )
            return;

    int i = listOpenedStar[rootIndex].indexOf (this);

    // Show parent menu if there is one (not needed if m_parentVisible is true)
    if (i>0 && !m_parentVisible)
        listOpenedStar[rootIndex].at(i-1)->setChildsOpacity (1, false);

    if (i>=0) {
        // Hide this menu and all childs
        listOpenedStar[rootIndex].at(i)->setChildsOpacity (0,true);

        StarMenu *sm;
        for ( int n=i; n<listOpenedStar[rootIndex].count (); n++) {
            sm = listOpenedStar[rootIndex].at(n);
            sm->centerChildItems();
            sm->starShape.clear();
            // If set visualParent , restore it back to the parent
            if (sm->m_visualParent != 0 )
                sm->setParentItem( sm->realParent );
            emit sm->starMenuClosing ();
            sm->m_open = false;
            emit sm->openChanged();
        }
        m_open = false;
        emit openChanged();

        // remove child StarMenu pointers in the list
        int k = listOpenedStar[rootIndex].count ();
        for ( int n=i; n<k; n++)
            listOpenedStar[rootIndex].removeLast ();

        //    qDebug() << "CLOSED2:"; debugList();
    }
}

// Debug purpose function
void StarMenu::debugList()
{
    QHash<int , QList<StarMenu *> >::const_iterator i = listOpenedStar.constBegin();
    QList<StarMenu *>menuList;
    StarMenu *menu;

     while (i != listOpenedStar.constEnd()) {
         menuList = i.value();
         foreach (menu , menuList)
             qDebug() << i.key() << ": " << menu->property("objectName").toString() << "OPEN:" << menu->open();
         ++i;
     }
     qDebug() << "\n";
}


void StarMenu::onStarMenuOpening()
{
    // Check if there are animations running, if so don't open the menu
//    if (checkChildsAnimationsRunning( this ) )
//        return;0r
//    for (int i=0; i < listOpenedStar[rootIndex].count(); i++)
//        if (checkChildsAnimationsRunning( listOpenedStar[rootIndex].at(i) ) )
//            return;



//    // Set transformOringin to mouse press coords for an eventual rotation animation
//    if (!m_center.isNull())
//        static_cast<QGraphicsObject *>(this)->setTransformOriginPoint ( scenePosInPortrait(mapFromScene( m_center )) );
//    else
//        static_cast<QGraphicsObject *>(this)->setTransformOriginPoint ( startMousePos - scenePosInPortrait(scenePos()) );

//    qDebug() << "onStarMenuOpening arrangeCircle" << startMousePos;

    // Set sensible area to to the given visualParent if set.
    if (m_visualParent != 0 ) {
        realParent = this->parentItem();
        setParentItem(m_visualParent);
    }

    listOpenedStar[rootIndex].append(this);

    setParentClosedProperties();
    centerChildItems();
    updateStarItemPos();
    setChildsOpacity (1,false);
    m_open = true;
    emit openChanged();

//    qDebug() << "OPENED:"; debugList();
    emit starMenuOpening ();
}


void StarMenu::stopAllChildAnimations(StarMenu *target)
{
    QGraphicsItem *item;
    QObject *obj, *anim;

    foreach (item, target->childItems() ) {
        foreach (obj, static_cast<QDeclarativeItem *>(item)->children() ) {

            if (obj->inherits("QDeclarativeAbstractAnimation")) {
                QMetaObject::invokeMethod(anim, "complete");
            }

            if (obj->inherits("QDeclarativeBehavior") ) {
                foreach (anim, obj->children()) {
                    if (anim->property("running").toBool())
                        QMetaObject::invokeMethod(anim, "complete");
                }
            }
        }
    }
}

bool StarMenu::checkChildsAnimationsRunning(StarMenu *target)
{
    QGraphicsItem *item;
    QObject *obj, *anim;


    // TODO: optimize this function to check StarMenu childs items in one foreach

    // Check if target StarMenu has animations
    foreach (obj, target->children() ) {

        if (obj->inherits("QDeclarativeAbstractAnimation")) {
            if (obj->property("running").toBool()) return true;
        }

        if (obj->inherits("QDeclarativeBehavior") ) {
            foreach (anim, obj->children()) {
                if (anim->property("running").toBool()) {
                    return true;
                }
            }
        }
    }

    // Check StarMenu childs items ( Behavior and NumberAnimation for example are not QDeclarativeItems
    foreach (item, target->childItems() ) {
        foreach (obj, static_cast<QDeclarativeItem *>(item)->children() ) {

            if (obj->inherits("QDeclarativeAbstractAnimation")) {
                if (obj->property("running").toBool()) return true;
            }

            // if a child is a QDeclarativeBehavior, get the animation object for ex:
            // Behavior on x  { NumberAnimation { duration: 1500 } }
            // anim will be NumberAnimation pointer and could be checked.
            // Instead obj->property("animation.running").toBool() doesn't work
            if (obj->inherits("QDeclarativeBehavior") ) {
                foreach (anim, obj->children()) {
                    if (anim->property("running").toBool()) {
//                        qDebug() << "ANIMATION still RUNNING";
                        return true;
                    }
                }
            }
        }
    }
    return false;
}



//////////////////////////////////////////////////////////
// Manage StarItems

int StarMenu::calculateNumberOfItems()
{
    if (starItemChildrensCount!=0) return starItemChildrensCount;
    QGraphicsItem *item;
    foreach (item, childItems())
        if (item->type() == item->UserType+2 && static_cast<QDeclarativeItem *>(item)->isVisible())
            starItemChildrensCount++;
    return starItemChildrensCount;
}

void StarMenu::updateStarItemPos()
{
    if (!isComponentComplete()) return;

    starItemChildrensCount=0;
    if (m_shape==ShapeCircle)  arrangeCircleShape();
    if (m_shape==ShapeLine)    arrangeLineShape();
    if (m_shape==ShapeGrid)    arrangeGridShape();
    if (m_shape==ShapeSpiral)  arrangeSpiralShape();
}

// Arrange StarItems in a circle shape
void StarMenu::arrangeCircleShape()
{
    QGraphicsItem *item;
    int n_items = calculateNumberOfItems();
    int n = 0;
    qreal halfWidth;
    qreal halfHeight;
    foreach (item, childItems())
        if (item->type() == item->UserType+2 && static_cast<QDeclarativeItem *>(item)->isVisible())
        {
            halfWidth  = static_cast<QDeclarativeItem *>(item)->width() / 2;
            halfHeight = static_cast<QDeclarativeItem *>(item)->height() / 2;

            // Set x and y with setPos() will not send onXChanged and onYChanged event to QML
            static_cast<StarItem *>(item)->setDelayedPos (startMousePos.x() + cos(2*M_PI/n_items*n+m_rotation)*m_radius_h - halfWidth  - scenePosInPortrait(scenePos()).x() ,
                                                          startMousePos.y() - sin(2*M_PI/n_items*n+m_rotation)*m_radius_v - halfHeight - scenePosInPortrait(scenePos()).y() ,
                                                          n*m_delayChangePos);
            n++;
        }
}

// Arrange StarItems in a line shape
void StarMenu::arrangeLineShape()
{
    QGraphicsItem *item;
    int n = 0;
    qreal itemDiameter;
    qreal firstItemHalfWidth = 0;
    qreal firstItemHalfHeight = 0;
    qreal radius = m_startingRadius;
    qreal halfWidth;
    qreal halfHeight;
    qreal secH, secV;
    foreach (item, childItems())
        if (item->type() == item->UserType+2 && static_cast<QDeclarativeItem *>(item)->isVisible())
        {
            halfWidth  = static_cast<QDeclarativeItem *>(item)->width() / 2.0;
            halfHeight = static_cast<QDeclarativeItem *>(item)->height() / 2.0;

            // itemDiameter is calculated by the segment lenght that intersect the item bounding box
            // passing throught the center of the item and intersect the opposite edges by m_startingAngle angle
            secH = abs(halfHeight / sin(m_startingAngle));
            secV = abs(halfWidth /  sin(M_PI/2-m_startingAngle));
            // checks if the line intersect horizontal or vertical edges
            if (secH < secV) itemDiameter = secH*2.0;
            else itemDiameter = secV*2.0;

            // These checks if the line is vertical or horizontal
            if ( (m_startingAngle+M_PI_2)/M_PI == ceil((m_startingAngle+M_PI_2)/M_PI))
                itemDiameter=halfHeight*2;
            if ( m_startingAngle/M_PI == ceil(m_startingAngle/M_PI))
                itemDiameter=halfWidth*2;

//            itemDiameter = qMax(static_cast<QDeclarativeItem *>(item)->width(),
//                              static_cast<QDeclarativeItem *>(item)->height());

            if (n==0) { firstItemHalfWidth = halfWidth; firstItemHalfHeight=halfHeight; }

            // Set x and y with setPos() will not send onXChanged and onYChanged event to QML
            static_cast<StarItem *>(item)->setDelayedPos (startMousePos.x() - scenePosInPortrait(scenePos()).x() + cos(m_startingAngle)*(radius + halfWidth - firstItemHalfWidth) - halfWidth,
                                                          startMousePos.y() - scenePosInPortrait(scenePos()).y() + sin(m_startingAngle)*(radius + halfHeight - firstItemHalfHeight) - halfHeight,
                                                          n*m_delayChangePos);
            radius += itemDiameter + m_radiusIncrement;
            n++;
        }
}

// Arrange StarItems in a grid shape
void StarMenu::arrangeGridShape()
{
    int n = 1;
    qreal h;
    qreal x=0;
    qreal y=0;
    QList<QPointF> itemPos;
    int j=0;
    int k=0;
    qreal hMax = 0;
    qreal wMax = 0;
    int count = 0;
    QList<qreal> rowsWidth;
    QList<QDeclarativeItem*> items;
    int nItems = calculateNumberOfItems();

    foreach (QGraphicsItem *item, childItems())
        if (item->type() == item->UserType+2 && static_cast<QDeclarativeItem *>(item)->isVisible())
            items.append(static_cast<QDeclarativeItem *>(item));

    // Calculating the grid
    while ( j*m_columns+k < items.count() )
    {
        count = 0;
        hMax = 0;
        x = 0;
        // Calculate x position for all rows
        while ( k < m_columns && j*m_columns+k < items.count() )
        {
            itemPos.append ( QPointF(x,y) );
            // hMax = max height of the item in this row
            hMax = qMax( hMax , items.at(j*m_columns+k)->height() );
            x += items.at(j*m_columns+k)->width() + m_spacingH;
            count++;
            k++;
        }
        // wMax = max width of all rows
        wMax = qMax (wMax , x);
        rowsWidth.append(x-m_spacingH);
        // Calculate y position
        for (int i=0; i<count; i++)
        {
            h = items.at(j*m_columns+k-i-1)->height();
            itemPos[ itemPos.count()-i-1 ].setY( y+(hMax-h)/2 );
        }
        y += hMax+m_spacingV;
        k = 0;
        j++;
    }
    y -= m_spacingV;
    // At this point:
    //    y = grid height
    //    wMax = grid width
    //    rowsWidth = list cointains all the rows width
    //    so it's possible to center rows and center grid in startMousePos
    n=0;
    int dx;
    while ( n < items.count() )
    {
        dx = ( wMax - rowsWidth.at(n/m_columns) ) / 2;
        itemPos[ n ].setX( itemPos[ n ].x()+dx - wMax/2);
        itemPos[ n ].setY( itemPos[ n ].y()    - y/2);
        n++;
    }

    // Set items position with the calculated grid coordinates
    n=0;
    foreach (QDeclarativeItem *item, items)
    {
        // Set x and y with the above method (instead of setPos()) will send onXChanged and onYChanged event to QML
        static_cast<StarItem *>(item)->setDelayedPos (startMousePos.x() + itemPos.at(n).x() - scenePosInPortrait(scenePos()).x() ,
                                                      startMousePos.y() + itemPos.at(n).y() - scenePosInPortrait(scenePos()).y() ,
                                                      n*m_delayChangePos);
        n++;
    }
}

// Arrange StarItems in a spiral shape
void StarMenu::arrangeSpiralShape()
{
    QGraphicsItem *item;
    int n = 0;
    qreal halfWidth;
    qreal halfHeight;
    qreal r = m_startingRadius;
    qreal angle = 0;
    qreal itemRadians;
    qreal itemRadius;
    foreach (item, childItems())
        if (item->type() == item->UserType+2 && static_cast<QDeclarativeItem *>(item)->isVisible())
        {
            halfWidth  = static_cast<QDeclarativeItem *>(item)->width() / 2;
            halfHeight = static_cast<QDeclarativeItem *>(item)->height() / 2;

            // Calculate how much angle should be incremented taking care of StarItem size
            itemRadius  = sqrt( pow(static_cast<QDeclarativeItem *>(item)->width(),2) + pow(static_cast<QDeclarativeItem *>(item)->height(),2) );
            itemRadians = atan(itemRadius/r);
            // Set x and y with the above method (instead of setPos()) will send onXChanged and onYChanged event to QML
            static_cast<StarItem *>(item)->setDelayedPos (startMousePos.x() + cos(angle+(itemRadians/2)+m_rotation)*r - halfWidth  - scenePosInPortrait(scenePos()).x() ,
                                                          startMousePos.y() - sin(angle+(itemRadians/2)+m_rotation)*r - halfHeight - scenePosInPortrait(scenePos()).y() ,
                                                          n*m_delayChangePos);
            angle += itemRadians;
            r += m_radiusIncrement;
            n++;
        }
}


// Positioning items where user pressed mouse button
void StarMenu::centerChildItems()
{
    stopAllChildAnimations(this);
    QGraphicsItem *item;
    int n = 0;
    qreal halfWidth;
    qreal halfHeight;
    foreach (item, childItems())
        if (item->type() == UserType+2 )
        {
            halfWidth  = static_cast<QDeclarativeItem *>(item)->width() / 2;
            halfHeight = static_cast<QDeclarativeItem *>(item)->height() / 2;
            // Set at first the properties of QDeclarativeItem, then the properties of QML to stop animations
            // and prevent the animation to be continued when the menu is opened again in few time

            static_cast<QDeclarativeItem *>(item)->setOpacity (0);
            static_cast<QDeclarativeItem *>(item)->resetTransform();
            static_cast<QDeclarativeItem *>(item)->setPos ( startMousePos.x() - halfWidth  - scenePosInPortrait(scenePos()).x() ,
                                                            startMousePos.y() - halfHeight - scenePosInPortrait(scenePos()).y());
//            static_cast<QDeclarativeItem *>(item)->setProperty ("x", startMousePos.x() - halfWidth  - scenePos().x() );
//            static_cast<QDeclarativeItem *>(item)->setProperty ("y", startMousePos.y() - halfHeight - scenePos().y() );
//            static_cast<QDeclarativeItem *>(item)->setProperty ("opacity", 0 );
//            static_cast<QDeclarativeItem *>(item)->setProperty ("rotation", 0 );
            n++;
        }


}


// Make all items opacity=o
void StarMenu::setChildsOpacity(qreal o, bool propagate)
{
    QGraphicsItem *item;
    QGraphicsItem *subItem;
    foreach (item, childItems()) {

        if (static_cast<QDeclarativeItem*>(item)->type() <= UserType ) {
            static_cast<QGraphicsObject*>(item)->setProperty( "opacity" , o );
            static_cast<QGraphicsObject*>(item)->setOpacity(o);
        }

        if (static_cast<QDeclarativeItem*>(item)->type() == UserType+2 ) { // StarItems
            static_cast<QDeclarativeItem*>(item)->setProperty( "opacity" , o );

            // Check if there is some other child
            foreach (subItem, static_cast<QGraphicsObject*>(item)->childItems()) {
                static_cast<QDeclarativeItem*>(subItem)->setProperty( "opacity" , o );
                if (static_cast<QDeclarativeItem*>(subItem)->type() == UserType+1 )  // Another StarMenu !
                {
                   if (propagate) static_cast<StarMenu*>(subItem)->setChildsOpacity( o , propagate );
                   // Set also rootIndex of the submenu
                   static_cast<StarMenu*>(subItem)->rootIndex = rootIndex;
                }
            }


        }

        if (item->type() == item->UserType+1 ) {  // StarMenus
            if (propagate) static_cast<StarMenu*>(item)->setChildsOpacity( o , propagate );
            static_cast<QDeclarativeItem*>(item)->setProperty( "opacity" , o );
        }

    }
}

// Set parent opacity
void StarMenu::setParentClosedProperties()
{
    // if this is the root menu exit
    if ( listOpenedStar[rootIndex].count()<=1 ) return;
    QGraphicsItem *item;
    int n;
    n = listOpenedStar[rootIndex].indexOf (this) - 1;

    if (n>=0) {
//        stopAllChildAnimations(listOpenedStar[rootIndex].at(n));
        foreach ( item, static_cast<QDeclarativeItem*>(listOpenedStar[rootIndex].at(n))->childItems() ) {

            if (!m_parentVisible) static_cast<QDeclarativeItem*>(item)->setOpacity(0);
//            if (!m_parentVisible) static_cast<QDeclarativeItem*>(item)->setProperty("opacity",0);
            static_cast<QDeclarativeItem*>(item)->rotate(0);
            // resetScale only for StarItems
            if ( static_cast<QDeclarativeItem*>(item)->type() == UserType +2) {
                static_cast<StarItem*>(item)->stopTimer();
                static_cast<StarItem*>(item)->resetScale();
            }
        }
    }
}


