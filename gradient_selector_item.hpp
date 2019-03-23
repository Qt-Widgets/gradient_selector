#ifndef GRADIENT_SELECTOR_ITEM_HPP
#define GRADIENT_SELECTOR_ITEM_HPP

#include <QWidget>

class GradientSelectorItem : public QWidget
{
    Q_OBJECT
public:
    explicit GradientSelectorItem( QWidget *parent = Q_NULLPTR );
    virtual ~GradientSelectorItem() override = default;

signals:

public slots:
};

#endif // GRADIENT_SELECTOR_ITEM_HPP
