#ifndef PRICEFORMATTER_H
#define PRICEFORMATTER_H

#include <QString>

class PriceFormatter {
public:
    static QString format(double price) {
        return QString("%1 руб.").arg(price, 0, 'f', 0);
    }
};

#endif