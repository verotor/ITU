#include "starmenu_plugin.h"
#include "starmenu.h"
#include "staritem.h"

#include <QtDeclarative/qdeclarative.h>

void StarMenuPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<StarMenu>(uri, 1, 0, "StarMenu");
    qmlRegisterType<StarItem>(uri, 1, 0, "StarItem");
}

Q_EXPORT_PLUGIN2(StarMenu, StarMenuPlugin)

