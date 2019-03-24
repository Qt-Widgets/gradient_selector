#include "gradient_selector.hpp"
#include "gradient_selector_item.hpp"

#include <QPainter>
#include <QLinearGradient>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVector>
#include <QPair>
#include <QColor>
#include <QColorDialog>

GradientSelector::GradientSelector(
        const QColor& left, const QColor& right, QWidget* parent )
    : QWidget( parent ),
      mHeight( 25 ),
      mMargin( 10 )
{
    this->addGradientPoint( 0.0, left );
    this->addGradientPoint( 1.0, right );
}

void GradientSelector::addGradientPoint( const qreal pos, const QColor& color )
{
    mGradientPositions.append( qMakePair( pos, color ) );
}

QRect GradientSelector::getGradientRect() const
{
    return getGradientRect( this->rect() );
}

QRect GradientSelector::getGradientRect( const QRect& rect ) const
{
    return QRect( rect.left() + this->margin(),
                  rect.top() + ( rect.height() - this->height() ) / 2,
                  rect.width() - 2 * this->margin(), this->height() );
}

void GradientSelector::paintEvent( QPaintEvent* event )
{
    QRect gradientRect = this->getGradientRect( event->rect() );
    QLinearGradient gradient( gradientRect.topLeft(), gradientRect.topRight() );
    for( const auto& item : mGradientPositions )
    {
        gradient.setColorAt( item.first, item.second );
    }

    QPainter painter( this );
    painter.fillRect( gradientRect, gradient );
}

void GradientSelector::mousePressEvent( QMouseEvent* event )
{
    QRect gradientRect = this->getGradientRect();
    if( !gradientRect.contains( event->pos() ) ) return;

    auto newColor = QColorDialog::getColor( Qt::white, this );
    if( !newColor.isValid() ) return;

    this->addGradientPoint(
                qreal( event->x() - gradientRect.x() ) / gradientRect.width(), newColor );
    this->repaint();
}

QSize GradientSelector::minimumSizeHint() const
{
    return QSize( this->margin() * 4, this->margin() * 2 + this->height() );
}

qint32 GradientSelector::margin() const
{
    return mMargin;
}

void GradientSelector::setMargin( const qint32& margin )
{
    mMargin = margin;
}

qint32 GradientSelector::height() const
{
    return mHeight;
}

void GradientSelector::setHeight( const qint32& height )
{
    mHeight = height;
}
