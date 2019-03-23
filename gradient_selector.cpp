#include "gradient_selector.hpp"

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
      mMagrin( 10 )
{
    this->addGradientPoint( 0.0, left );
    this->addGradientPoint( 1.0, right );
}

void GradientSelector::addGradientPoint( const qreal pos, const QColor& color )
{
    mGradientPositions.append( qMakePair( pos, color ) );
}

void GradientSelector::paintEvent( QPaintEvent* event )
{
    QRect gradientRect(
                event->rect().left() + this->magrin(),
                event->rect().top() + ( event->rect().height() - this->height() ) / 2,
                event->rect().width() - 2 * this->magrin(), this->height() );

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
    auto newColor = QColorDialog::getColor( Qt::white, this );
    this->addGradientPoint( qreal( event->x() ) / this->height(), newColor );
    this->repaint();
}

QSize GradientSelector::minimumSizeHint() const
{
    return QSize( this->magrin() * 4, this->magrin() * 2 + this->height() );
}

qint32 GradientSelector::height() const
{
    return mHeight;
}

void GradientSelector::setHeight( const qint32& height )
{
    mHeight = height;
}

qint32 GradientSelector::magrin() const
{
    return mMagrin;
}

void GradientSelector::setMagrin( const qint32& magrin )
{
    mMagrin = magrin;
}

