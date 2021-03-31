#include "XTableViewModel.h"

//XTableViewModel::XTableViewModel(QObject *parent) : QAbstractTableModel(parent)
//{

//}

////优化:
////添加数据前：
////TableView->setUpdatesEnabled(false);  //暂停界面刷新
////添加大量数据过程。。。。
////添加完毕后：
////TableView->setUpdatesEnabled(true);  //恢复界面刷新


//#include <QSqlQuery>

////自定义模型类
//class XSqlTableModel : public QAbstractTableModel
//{
//    Q_OBJECT

//public:
//    XSqlTableModel(const QStringList &hreadList, QObject *parent = nullptr)
//    {
//        Q_UNUSED(parent)
//        mRoleList = hreadList;
//    }

//    ~XSqlTableModel() override
//    {

//    }

//    int rowCount(const QModelIndex & = QModelIndex()) const override
//    {
//        return mRow;
//    }

//    int columnCount(const QModelIndex & = QModelIndex()) const override
//    {
//        return mColumn;
//    }

//    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
//    {
//        if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
//        {
//            //qDebug() << "[headerData]" << section << mRoleList.size();
//            if (section < mRoleList.size())
//            {
//                return mRoleList[section];
//            }
//        }

//        return QAbstractTableModel::headerData(section, orientation, role);
//    }

//    QVariant data(const QModelIndex &index, int role) const override
//    {
//        if(!index.isValid())
//        {
//            return QVariant();
//        }

//        switch (role)
//        {
//        case Qt::TextColorRole:
//            return QColor(Qt::black);
//        case Qt::TextAlignmentRole:
//            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
//        case Qt::DisplayRole:
//        {
//            if (mResult.size() > index.row())
//            {
//                if (mResult[0].size() > index.column())
//                {
//                    if (index.column() == 7) //状态
//                    {
//                        return mResult[index.row()][index.column()] == "1" ? "有效" : "无效";
//                    }
//                    else if (index.column() == 8) //编辑
//                    {
//                        return "编辑";
//                    }
//                    else if (index.column() == 9) //删除
//                    {
//                        return "删除";
//                    }
//                    else
//                    {
//                        return mResult[index.row()][index.column()];
//                    }
//                }
//            }
//            //qDebug() << role << index.column() << index.row();

//            return QVariant();
//        }
//        case Qt::CheckStateRole:
//        {
//            return QVariant(); // 返回勾选框，如果返回QVariant()就没有勾选框
//        }
//        default:
//            return QVariant();
//        }
//    }

//    QVector<QString> getRowData(int row)
//    {
//        if (row < 0 || row >= mResult.size())
//        {
//            QVector<QString> result(0);
//            return result;
//        }

//        return mResult[row];
//    }

//    void clear()
//    {
//        mResult.clear();
//        mRow = 0;
//        mColumn = 0;
//    }

//signals:
//    void signalRecvhandleResult();

//public slots:
//    void handleResults(const QVector<QVector<QString> > &result)
//    {
//        emit signalRecvhandleResult();
//        beginResetModel(); // 开始刷新模型
//        mResult = result;
//        mRow = mResult.size();
//        mColumn = mRow > 0 ? mResult[0].size() : 0;
//        endResetModel(); // 结束刷新模型
//    }

//private:
//    int mRow;
//    int mColumn;
//    QStringList mRoleList;
//    QVector< QVector<QString> > mResult;

//};

//QT_BEGIN_NAMESPACE
//namespace Ui { class Widget; }
//QT_END_NAMESPACE

//class Widget : public QWidget
//{
//    Q_OBJECT

//public:
//    Widget(QWidget *parent = nullptr);
//    ~Widget();

//    void updateTableData(const QVector< QVector<QString> > &result);

//private slots:
//    void on_pushButton_clicked();

//private:
//    Ui::Widget *ui;
//};

////显示表格数据函数
//void Widget::updateTableData(const QVector<QVector<QString> > &result)
//{
//    if (result.size() == 0)
//        return;

//   // XSqlTableModel* model = ((QStandardItemModel*)ui->tableView->model());
//     XSqlTableModel* model = new XSqlTableModel();
//    ui->tableView->setModel(model);

//    int start = model->rowCount();
//    int col = result[0].size();
//    int row = result.size();
//    QIcon iconEdit(":/new/prefix1/images/edit.png");
//    QIcon iconDel(":/new/prefix1/images/edit_remove.png");

//    for (int i = 0; i<row; ++i)
//    {
//        QList<QStandardItem*> list;
//        for (int j = 0; j<col; ++j)
//        {
//            if (j == col - 3)
//            {
//                list << new QStandardItem(result[i][j] == "1" ? "有效" : "无效");
//            }
//            else if (j == col - 2)
//            {
//                list << new QStandardItem(iconEdit, "编辑");
//            }
//            else if (j == col - 1)
//            {
//                list << new QStandardItem(iconDel, "删除");
//            }
//            else
//            {
//                list << new QStandardItem(result[i][j]);
//            }
//        }
//        model->insertRow(i + start, list);
//    }
//}

////生成表格数据函数
//void Widget::on_pushButton_clicked()
//{
//    QVector< QVector<QString> > result;
//    for (int i = 0; i < 1000000; ++i)
//    {
//        QVector<QString> tmp(10);
//        tmp[0] = QString::number(i+1);
//        tmp[1] = QString("test%1").arg(i+1);
//        tmp[2] = tmp[1];
//        tmp[7] = "1";
//        tmp[8] = "";
//        tmp[9] = "";
//        result.push_back(tmp);
//    }
//    updateTableData(result);
//}
