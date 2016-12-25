/**
* Project        CVNAR
* Copyright (C)  2010-2016
* Company        Luxoft
*                All rights reserved
* Secrecy Level  STRICTLY CONFIDENTIAL
*
* @file     ComboDelegate.hpp
* @author   Maxim Bondarenko
* @date     24.12.2016
*/

#pragma once

#include <QItemDelegate>

class ComboDelegate : public QItemDelegate
{
   Q_OBJECT

public:
   ComboDelegate(QObject *parent = 0);

   QWidget *createEditor(
         QWidget *parent,
         const QStyleOptionViewItem &option,
         const QModelIndex &index) const;

   void setEditorData(QWidget *editor, const QModelIndex &index) const;

   void setModelData(
         QWidget *editor,
         QAbstractItemModel *model,
         const QModelIndex &index) const;

   void updateEditorGeometry(
         QWidget *editor,
         const QStyleOptionViewItem &option,
         const QModelIndex &index) const;
};
