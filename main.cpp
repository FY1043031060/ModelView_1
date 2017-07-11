
#include <QApplication>
#include <QAbstractItemDelegate>
#include <QPainter>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>

class BarDelegate : public QAbstractItemDelegate
{
public:
  BarDelegate( QObject *parent = 0 );
  // 覆盖两个函数就可以显示进度条
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
  QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};
BarDelegate::BarDelegate( QObject *parent ) : QAbstractItemDelegate( parent ) { }

void BarDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  if( option.state & QStyle::State_Selected )
    painter->fillRect( option.rect, option.palette.highlight() );

  // 数据是存储在QStandardItemModel的QStandardItem，会自动根据当前行进行匹配（我认为）
  int value = index.model()->data( index, Qt::DisplayRole ).toInt(); // 这句，取得当前行的数据
  qDebug() << value;
  double factor = double(value)/100.0; // 计算比例因子

  painter->save(); // 保存旧画板（我认为）
  // 进度条背景色
  if( factor > 0.8 )
  {
    painter->setBrush( Qt::red ); // 超过0.8画纯红色
    factor = 1;
  }
  else
    painter->setBrush( QColor( 0, int(factor*255), 255-int(factor*255) ) ); // 否则颜色依次变淡

  painter->setPen( Qt::black ); // 画笔颜色（这里没用到，我认为）
  // 前面都是准备工作，这里才是真正画进度条
  painter->drawRect( option.rect.x()+2, option.rect.y()+2, int(factor*(option.rect.width()-5)), option.rect.height()-5 );
  painter->restore(); // 恢复新画板（我认为）
}

QSize BarDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  return QSize( 45, 15 ); // 随便改，不影响（我认为）
}

int main( int argc, char **argv )
{
  QApplication app( argc, argv );

  QTableView table;

  QStandardItemModel model( 10, 2 );
  for( int r=0; r<10; ++r )
  {
    QStandardItem *item = new QStandardItem( QString("Row %1").arg(r+1) );
    model.setItem( r, 0, item );
    model.setItem( r, 1, new QStandardItem( QString::number((r*30)%100 )) );
  }

  table.setModel( &model ); // 正常设置模型，没有任何特殊之处

  BarDelegate delegate;
  table.setItemDelegateForColumn( 1, &delegate ); // 设置第一列为代理
  table.show();

  return app.exec();
}
