
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
  // �������������Ϳ�����ʾ������
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
  QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};
BarDelegate::BarDelegate( QObject *parent ) : QAbstractItemDelegate( parent ) { }

void BarDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  if( option.state & QStyle::State_Selected )
    painter->fillRect( option.rect, option.palette.highlight() );

  // �����Ǵ洢��QStandardItemModel��QStandardItem�����Զ����ݵ�ǰ�н���ƥ�䣨����Ϊ��
  int value = index.model()->data( index, Qt::DisplayRole ).toInt(); // ��䣬ȡ�õ�ǰ�е�����
  qDebug() << value;
  double factor = double(value)/100.0; // �����������

  painter->save(); // ����ɻ��壨����Ϊ��
  // ����������ɫ
  if( factor > 0.8 )
  {
    painter->setBrush( Qt::red ); // ����0.8������ɫ
    factor = 1;
  }
  else
    painter->setBrush( QColor( 0, int(factor*255), 255-int(factor*255) ) ); // ������ɫ���α䵭

  painter->setPen( Qt::black ); // ������ɫ������û�õ�������Ϊ��
  // ǰ�涼��׼���������������������������
  painter->drawRect( option.rect.x()+2, option.rect.y()+2, int(factor*(option.rect.width()-5)), option.rect.height()-5 );
  painter->restore(); // �ָ��»��壨����Ϊ��
}

QSize BarDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  return QSize( 45, 15 ); // ���ģ���Ӱ�죨����Ϊ��
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

  table.setModel( &model ); // ��������ģ�ͣ�û���κ�����֮��

  BarDelegate delegate;
  table.setItemDelegateForColumn( 1, &delegate ); // ���õ�һ��Ϊ����
  table.show();

  return app.exec();
}
