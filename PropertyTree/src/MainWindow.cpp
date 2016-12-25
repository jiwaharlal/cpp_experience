#include <QFileSystemModel>
#include <QTreeView>
#include <QLayout>

#include "MainWindow.hpp"
#include "ComboDelegate.hpp"

MainWindow::MainWindow(const char* path)
{
   QString pathStr(path);

   QFileSystemModel *model = new QFileSystemModel;

   model->setRootPath(QDir::currentPath());

   QTreeView *treeView = new QTreeView(this);
   treeView->setModel(model);
   treeView->resize(800, 400);

   ComboDelegate* delegate = new ComboDelegate();
   treeView->setItemDelegateForColumn(1, delegate);

   layout()->addWidget(treeView);
}
