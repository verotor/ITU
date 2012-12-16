#ifndef STARMENU_PLUGIN_H
#define STARMENU_PLUGIN_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

class StarMenuPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);
};

#endif // STARMENU_PLUGIN_H

