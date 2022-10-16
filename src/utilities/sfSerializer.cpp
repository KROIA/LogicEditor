#include "sfSerializer.h"
#include <QRegularExpression>

QString sfVector2fToString(const sf::Vector2f &v)
{
    return "("+QString::number(v.x)+","+QString::number(v.y)+")";
}
sf::Vector2f stringToSfVector2f(const QString &str)
{
    sf::Vector2f v;
    QRegularExpression re("\\(([+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)),([+-]?([0-9]+([.][0-9]*)?|[.][0-9]+))\\)");
    QRegularExpressionMatch match = re.match(str);
    bool hasMatch = match.hasMatch(); // true
    if(hasMatch)
    {
        v.x = match.captured(1).toFloat();
        v.y = match.captured(4).toFloat();
       /* for(int i=0; i<match.capturedLength(); ++i)
        {
            qDebug() << "["<<i<<"] "<<match.captured(i);
        }*/
    }
    return v;
}
